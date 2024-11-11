#include "rgb_depth_aligned.h"
#include <cstdio>
#include <iostream>

#include "utility.hpp"

// Includes common necessary includes for development using depthai library
#include "depthai/depthai.hpp"

// Optional. If set (true), the ColorCamera is downscaled from 1080p to 720p.
// Otherwise (false), the aligned depth is automatically upscaled to 1080p
static std::atomic<bool> downscaleColor{true};
static constexpr int fps = 30;
// The disparity is computed at this resolution, then upscaled to RGB resolution
static constexpr auto monoRes = dai::MonoCameraProperties::SensorResolution::THE_720_P;

static float rgbWeight = 0.4f;
static float depthWeight = 0.6f;

static void updateBlendWeights(int percentRgb, void* ctx) {
    rgbWeight = float(percentRgb) / 100.f;
    depthWeight = 1.f - rgbWeight;
}

int playVideo() {
    while(true){
        cv::VideoCapture caprgb("/home/GLI/erik.khelifa/pr1/live_rgb.avi");
        cv::VideoCapture capdepth("/home/GLI/erik.khelifa/pr1/live_depth.avi");
        if (!caprgb.isOpened() || !capdepth.isOpened()) {
            std::cerr << "Error opening video stream or file" << std::endl;
            return -1;
        }
        cv::Mat frame;
        cv::Mat frame2;
        while (true) {
            caprgb >> frame;
            capdepth >> frame2;
            if (frame.empty() || frame2.empty()) {
                std::cerr << "End of video stream" << std::endl;
                return 0;
            }
            cv::imshow("rgb", frame);
            cv::waitKey(30);
            cv::imshow("depth", frame2);
            cv::waitKey(30);
        }
        cv::destroyAllWindows();

    }
    return 0;
}


int main(int argc, char** argv) {
    // save first argument to int
    int arg = 0;
    if (argc > 1) {
        arg = std::stoi(argv[1]);
    }


    using namespace std;

    // if arg is one show and analyze live video, if arg is two record video and if arg is three analyze recorded video

    if (arg == 3 ){
        playVideo();
        return 0;
    }



    // Create pipeline
    dai::Pipeline pipeline;
    dai::Device device;
    std::vector<std::string> queueNames;

    // Define sources and outputs
    auto camRgb = pipeline.create<dai::node::ColorCamera>();
    auto left = pipeline.create<dai::node::MonoCamera>();
    auto right = pipeline.create<dai::node::MonoCamera>();
    auto stereo = pipeline.create<dai::node::StereoDepth>();

    auto rgbOut = pipeline.create<dai::node::XLinkOut>();
    auto depthOut = pipeline.create<dai::node::XLinkOut>();

    rgbOut->setStreamName("rgb");
    queueNames.push_back("rgb");
    depthOut->setStreamName("depth");
    queueNames.push_back("depth");

    // Properties
    camRgb->setBoardSocket(dai::CameraBoardSocket::CAM_A);
    camRgb->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    camRgb->setFps(fps);
    if (downscaleColor) camRgb->setIspScale(2, 3);
    // For now, RGB needs fixed focus to properly align with depth.
    // This value was used during calibration
    try {
        auto calibData = device.readCalibration2();
        auto lensPosition = calibData.getLensPosition(dai::CameraBoardSocket::CAM_A);
        if (lensPosition) {
            camRgb->initialControl.setManualFocus(lensPosition);
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    left->setResolution(monoRes);
    left->setCamera("left");
    left->setFps(fps);
    right->setResolution(monoRes);
    right->setCamera("right");
    right->setFps(fps);

    stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    // LR-check is required for depth alignment
    stereo->setLeftRightCheck(true);
    stereo->setDepthAlign(dai::CameraBoardSocket::CAM_A);
    // enable Extended disparity by extending disparity range
    stereo->setExtendedDisparity(true);


    // Linking
    camRgb->isp.link(rgbOut->input);
    left->out.link(stereo->left);
    right->out.link(stereo->right);
    stereo->disparity.link(depthOut->input);

    // Connect to device and start pipeline
    device.startPipeline(pipeline);

    // Sets queues size and behavior
    for (const auto &name: queueNames) {
        device.getOutputQueue(name, 4, false);
    }

    std::unordered_map<std::string, cv::Mat> frame;

    auto rgbWindowName = "rgb";
    auto depthWindowName = "depth";
    //auto blendedWindowName = "rgb-depth";
    cv::namedWindow(rgbWindowName);
    cv::namedWindow(depthWindowName);
    //cv::namedWindow(blendedWindowName);
    int defaultValue = (int) (rgbWeight * 100);
    //cv::createTrackbar("RGB Weight %", blendedWindowName, &defaultValue, 100, updateBlendWeights);


    cv::VideoWriter writerrgb;
    cv::VideoWriter writerdepth;
    bool writerInitializedrgb = false;
    bool writerInitializeddepth = false;
    static int fileIndex = 0;

    while (true) {
        std::unordered_map<std::string, std::shared_ptr<dai::ImgFrame>> latestPacket;

        auto queueEvents = device.getQueueEvents(queueNames);
        for (const auto &name: queueEvents) {
            auto packets = device.getOutputQueue(name)->tryGetAll<dai::ImgFrame>();
            auto count = packets.size();
            if (count > 0) {
                latestPacket[name] = packets[count - 1];
            }
        }



        for (const auto &name: queueNames) {
            if (latestPacket.find(name) != latestPacket.end()) {
                if (name == depthWindowName) {
                    frame[name] = latestPacket[name]->getFrame();
                    auto maxDisparity = stereo->initialConfig.getMaxDisparity();
                    // Optional, extend range 0..95 -> 0..255, for a better visualisation
                    frame[name].convertTo(frame[name], CV_8UC1, 255. / maxDisparity);
                    // Optional, apply false colorization
                    cv::applyColorMap(frame[name], frame[name], cv::COLORMAP_JET);
                } else {
                    frame[name] = latestPacket[name]->getCvFrame();
                }
                    cv::imshow(name, frame[name]);


                bool isColor = (frame[name].type() == CV_8UC3);

                if (arg == 2) {
                    if (!writerInitializedrgb || !writerInitializeddepth) {
                        int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
                        double fps = 30.0;                          // framerate of the created video stream
                        //string filename = "./live_" + name + ".avi";  // unique name for each video file
                        if (name == "depth") {
                            writerdepth.open("/home/GLI/erik.khelifa/pr1/live_depth.avi", codec, fps, frame[name].size(), isColor);
                        }
                        if (name == "rgb") {
                            writerrgb.open("/home/GLI/erik.khelifa/pr1/live_rgb.avi", codec, fps, frame[name].size(), isColor);
                        }

                        // check if we succeeded
                        //if (!writerrgb.isOpened()) {
                        //    cerr << "Could not open the output video file for write\n";
                        //    return -1;
                        //}
                        if (name == "depth") {
                            writerInitializeddepth = true;
                        }
                        if (name == "rgb") {
                            writerInitializedrgb = true;
                        }
                        cout << "Press any key to terminate" << endl;
                    }
                    // encode the frame into the videofile stream

                    if (name == "depth") {
                        writerdepth.write(frame[name]);
                    }
                    if (name == "rgb") {
                        writerrgb.write(frame[name]);
                    }

                }
            }
        }

        // Blend when both received
        //if(frame.find(rgbWindowName) != frame.end() && frame.find(depthWindowName) != frame.end()) {
        // Need to have both frames in BGR format before blending
        //  if(frame[depthWindowName].channels() < 3) {
        //     cv::cvtColor(frame[depthWindowName], frame[depthWindowName], cv::COLOR_GRAY2BGR);
        // }
        //cv::Mat blended;
        //cv::addWeighted(frame[rgbWindowName], rgbWeight, frame[depthWindowName], depthWeight, 0, blended);
        //cv::imshow(blendedWindowName, blended);
        // frame.clear();
        //}
        int key = cv::waitKey(1);
        if (key == 'q' || key == 'Q') {
            if (arg==2){
                writerrgb.release();
                writerdepth.release();
            }
            return 0;
        }
    }
    return 0;
}
