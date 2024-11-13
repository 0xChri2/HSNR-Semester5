#include <cstdio>
#include <iostream>
#include <iostream>
#include <depthai/depthai.hpp>

// Optional. If set (true), the ColorCamera is downscaled from 1080p to 720p.
// Otherwise (false), the aligned depth is automatically upscaled to 1080p
static std::atomic<bool> downscaleColor{true};
static constexpr int fps = 30;
// The disparity is computed at this resolution, then upscaled to RGB resolution
static constexpr auto monoRes = dai::MonoCameraProperties::SensorResolution::THE_720_P;

// Funktion zur Wiedergabe der Videos
int playVideo() {
    while(true) {
        cv::VideoCapture caprgb("live_rgb.avi"); // RGB Video
        cv::VideoCapture capdepth("live_depth.avi"); // Depth Video
        
        // Überprüfen ob Videos geöffnet werden konnten
        if (!caprgb.isOpened() || !capdepth.isOpened()) {
            std::cerr << "Error opening video stream or file" << std::endl;
            return -1;
        }

        cv::Mat frame;
        cv::Mat frame2;
        
        while(true) {
            caprgb >> frame; // RGB Frame
            capdepth >> frame2; // Depth Frame
            
            if (frame.empty() || frame2.empty()) {
                std::cerr << "End of video stream" << std::endl;
                return 0;
            }
            
            // Display RGB and Depth Frame
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
    
    // arg == 0,1 -> Live - Preview
    // arg == 2 -> Aufnahme
    // arg == 3 -> Wiedergabe der Videos

    int arg = 0;
    if (argc > 1) {
        arg = std::stoi(argv[1]);
    }

    // arg == 3 -> Wiedergabe der Videos
    if(arg == 3) {
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

    // Camera Settings - RGB, 1080p, 30fps
    camRgb->setBoardSocket(dai::CameraBoardSocket::CAM_A);
    camRgb->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    camRgb->setFps(fps);
    if(downscaleColor) camRgb->setIspScale(2, 3);

    // Camera Calibration
    try {
        auto calibData = device.readCalibration2();
        auto lensPosition = calibData.getLensPosition(dai::CameraBoardSocket::CAM_A);
        if(lensPosition) {
            camRgb->initialControl.setManualFocus(lensPosition);
        }
    } catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    // Mono Camera Settings - Mono, 720p, 30fps
    left->setResolution(monoRes);
    left->setCamera("left");
    left->setFps(fps);
    right->setResolution(monoRes);
    right->setCamera("right");
    right->setFps(fps);

    // Stereo Depth Settings
    stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereo->setLeftRightCheck(true);
    stereo->setDepthAlign(dai::CameraBoardSocket::CAM_A);
    stereo->setExtendedDisparity(true);  // Aktiviere extended disparity

    // Linking
    camRgb->isp.link(rgbOut->input);
    left->out.link(stereo->left);
    right->out.link(stereo->right);
    stereo->disparity.link(depthOut->input);

    // Connect to device and start pipeline
    device.startPipeline(pipeline);

    // Initialize Queue
    for(const auto& name : queueNames) {
        device.getOutputQueue(name, 4, false);
    }

    std::unordered_map<std::string, cv::Mat> frame;

    // Create window
    auto rgbWindowName = "rgb";
    auto depthWindowName = "depth";
    cv::namedWindow(rgbWindowName);
    cv::namedWindow(depthWindowName);

    // Prepare VideoWriter
    cv::VideoWriter writerrgb;
    cv::VideoWriter writerdepth;
    bool writerInitializedrgb = false;
    bool writerInitializeddepth = false;

    while(true) {
        std::unordered_map<std::string, std::shared_ptr<dai::ImgFrame>> latestPacket;

        auto queueEvents = device.getQueueEvents(queueNames);
        for(const auto& name : queueEvents) {
            auto packets = device.getOutputQueue(name)->tryGetAll<dai::ImgFrame>();
            auto count = packets.size();
            if(count > 0) {
                latestPacket[name] = packets[count - 1];
            }
        }

        for(const auto& name : queueNames) {
            if(latestPacket.find(name) != latestPacket.end()) {
                if(name == depthWindowName) {
                    frame[name] = latestPacket[name]->getFrame();
                    auto maxDisparity = stereo->initialConfig.getMaxDisparity();
                    frame[name].convertTo(frame[name], CV_8UC1, 255. / maxDisparity);
                    cv::applyColorMap(frame[name], frame[name], cv::COLORMAP_JET);
                } else {
                    frame[name] = latestPacket[name]->getCvFrame();
                }
                
                cv::imshow(name, frame[name]);

                // arg == 2 -> Record video
                if(arg == 2) {
                    bool isColor = (frame[name].type() == CV_8UC3);
                    
                    if(!writerInitializedrgb || !writerInitializeddepth) {
                        int codec = cv::VideoWriter::fourcc('M','J','P','G');
                        double fps = 30.0;
                        
                        if(name == "depth") {
                            writerdepth.open("live_depth.avi", codec, fps, frame[name].size(), isColor);
                            writerInitializeddepth = true;
                        }
                        if(name == "rgb") {
                            writerrgb.open("live_rgb.avi", codec, fps, frame[name].size(), isColor);
                            writerInitializedrgb = true;
                        }
                        std::cout << "Press q to terminate" << std::endl;
                    }
                    
                    if(name == "depth") {
                        writerdepth.write(frame[name]);
                    }
                    if(name == "rgb") {
                        writerrgb.write(frame[name]);
                    }
                }
            }
        }

        int key = cv::waitKey(1);
        if(key == 'q' || key == 'Q') {
            if(arg == 2) {
                writerrgb.release();
                writerdepth.release();
            }
            return 0;
        }
    }
    
    return 0;
}