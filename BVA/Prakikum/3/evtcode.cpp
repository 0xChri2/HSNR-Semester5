#include <cstdio>
#include <iostream>
#include <atomic>
#include <unordered_map>
#include <memory>
#include <opencv2/opencv.hpp>

// Includes common necessary includes for development using depthai library
#include "depthai/depthai.hpp"

// Optional. If set (true), the ColorCamera is downscaled from 1080p to 720p.
// Otherwise (false), the aligned depth is automatically upscaled to 1080p
static std::atomic<bool> downscaleColor{true};
static constexpr int fps = 30;
// The disparity is computed at this resolution, then upscaled to RGB resolution
static constexpr auto monoRes = dai::MonoCameraProperties::SensorResolution::THE_720_P;

//static float rgbWeight = 0.4f;
//static float depthWeight = 0.6f; //for blending

/*static void updateBlendWeights(int percentRgb, void* ctx) {
    rgbWeight = float(percentRgb) / 100.f;
    depthWeight = 1.f - rgbWeight;
}*/

int main(int argc, char** argv) {
    using namespace std;

    // Check command line arguments
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <mode (1: live, 2: record video, 3: evaluate video)>" << endl;
        return 1;
    }

    int mode = stoi(argv[1]);
    if (mode == 3) {
        cv::VideoCapture videoCapture("output.avi");
        if (!videoCapture.isOpened()) {
            cerr << "Error: Could not open video file" << endl;
            return 1;
        }

        cv::Mat frame;
        while (true) {
            if (!videoCapture.read(frame)) break; // Break if no more frames

            // Here you can implement any evaluation logic on the frame
            cv::imshow("Video Evaluation", frame);
            if (cv::waitKey(30) == 'q') break; // Wait for 30ms or break on 'q'
        }
        return 0;
    }

    // Create pipeline
    dai::Pipeline pipeline;
    dai::Device device;
    //std::vector<std::string> queueNames;

    // Define sources and outputs
    auto camRgb = pipeline.create<dai::node::ColorCamera>();
    auto left = pipeline.create<dai::node::MonoCamera>();
    auto right = pipeline.create<dai::node::MonoCamera>();
    auto stereo = pipeline.create<dai::node::StereoDepth>();

    stereo->setExtendedDisparity(false); //Praktikum A3.2 (Closer-in minimum depth, disparity range is doubled)

    auto rgbOut = pipeline.create<dai::node::XLinkOut>();
    auto depthOut = pipeline.create<dai::node::XLinkOut>();

    rgbOut->setStreamName("rgb");
    //queueNames.push_back("rgb");
    depthOut->setStreamName("depth");
    //queueNames.push_back("depth");

    // Camera properties
    camRgb->setBoardSocket(dai::CameraBoardSocket::CAM_A);
    camRgb->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    camRgb->setFps(fps);
    if(downscaleColor) camRgb->setIspScale(2, 3);
    // For now, RGB needs fixed focus to properly align with depth.
    // This value was used during calibration
    /*try {
        auto calibData = device.readCalibration2();
        auto lensPosition = calibData.getLensPosition(dai::CameraBoardSocket::CAM_A);
        if (lensPosition) {
            camRgb->initialControl.setManualFocus(lensPosition);
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }*/


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

    // Linking
    camRgb->isp.link(rgbOut->input);
    left->out.link(stereo->left);
    right->out.link(stereo->right);
    stereo->disparity.link(depthOut->input);

    // Connect to device and start pipeline
    device.startPipeline(pipeline);

    // Sets queues size and behavior
    /*for (const auto &name: queueNames) {
        device.getOutputQueue(name, 4, false);
    }*/

    std::unordered_map<std::string, cv::Mat> frame;

    auto rgbQueue = device.getOutputQueue("rgb", 4, false);
    auto depthQueue = device.getOutputQueue("depth", 4, false);
    //auto blendedWindowName = "rgb-depth"; //needed for blendedWindow line 99
    cv::namedWindow("rgb");
    cv::namedWindow("depth");
    //cv::namedWindow(blendedWindowName);
    //int defaultValue = (int) (rgbWeight * 100);
    //cv::createTrackbar("RGB Weight %", blendedWindowName, &defaultValue, 100, updateBlendWeights);


    if (mode == 2) {
        // Initialize video writer
        cv::VideoWriter videoWriter("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), fps, cv::Size(1280, 720));
        if (!videoWriter.isOpened()) {
            cerr << "Error: Could not open video writer" << endl;
            return 1;
        }

        while (true) {
            auto rgbPacket = rgbQueue->get<dai::ImgFrame>();
            auto depthPacket = depthQueue->get<dai::ImgFrame>();

            cv::Mat rgbFrame = rgbPacket->getCvFrame();
            cv::Mat depthFrame = depthPacket->getFrame();
            depthFrame.convertTo(depthFrame, CV_8UC1, 255. / 95);  // Set to 255 for better visibility
            cv::applyColorMap(depthFrame, depthFrame, cv::COLORMAP_JET); // Apply JET colormap

            // Show frames
            cv::imshow("rgb", rgbFrame);
            cv::imshow("depth", depthFrame);
            videoWriter.write(rgbFrame);  // Write frame to video
            videoWriter.write(depthFrame); //Write depth to video

            if (cv::waitKey(1) == 'q') break;
        }

        videoWriter.release();
    } else if (mode == 1) {
        while (true) {
            auto rgbPacket = rgbQueue->get<dai::ImgFrame>();
            auto depthPacket = depthQueue->get<dai::ImgFrame>();

            cv::Mat rgbFrame = rgbPacket->getCvFrame();
            cv::Mat depthFrame = depthPacket->getFrame();
            depthFrame.convertTo(depthFrame, CV_8UC1, 255. / 95);  // Set to 255 for better visibility
            cv::applyColorMap(depthFrame, depthFrame, cv::COLORMAP_JET); // Apply JET colormap

            cv::imshow("rgb", rgbFrame);
            cv::imshow("depth", depthFrame);

            if (cv::waitKey(1) == 'q') break;
        }
    }

    // Blend when both received
    /*if(frame.find(rgbWindowName) != frame.end() && frame.find(depthWindowName) != frame.end()) {
        // Need to have both frames in BGR format before blending
        if(frame[depthWindowName].channels() < 3) {
            cv::cvtColor(frame[depthWindowName], frame[depthWindowName], cv::COLOR_GRAY2BGR);
        }
        cv::Mat blended;
        cv::addWeighted(frame[rgbWindowName], rgbWeight, frame[depthWindowName], depthWeight, 0, blended);
        cv::imshow(blendedWindowName, blended);
        frame.clear();
    }*/

    return 0;
}