#include <cstdio>
#include <iostream>

//#include "utility.hpp"
#include "opencv2/opencv.hpp"

// Includes common necessary includes for development using depthai library
//#include "depthai/depthai.hpp"

// Optional. If set (true), the ColorCamera is downscaled from 1080p to 720p.
// Otherwise (false), the aligned depth is automatically upscaled to 1080p
static std::atomic<bool> downscaleColor{true};
static constexpr int fps = 30;
// The disparity is computed at this resolution, then upscaled to RGB resolution
//static constexpr auto monoRes = dai::MonoCameraProperties::SensorResolution::THE_720_P;

static float rgbWeight = 0.4f;
static float depthWeight = 0.6f;

static void updateBlendWeights(int percentRgb, void *ctx) {
    rgbWeight = float(percentRgb) / 100.f;
    depthWeight = 1.f - rgbWeight;
}
using namespace std;

// Funktion zur Berechnung des Schwellwerts und Zeichnen des Histogramms
void calculateThreshold(cv::Mat depthImage) {
    int histWidth = 256, histHeight = depthImage.rows;
    int histSize = 256;
    float hist[256] = {0};

    // calc histogram
    for (int y = 0; y < depthImage.rows; y++) {
        for (int x = 0; x < depthImage.cols; x++) {
            int intensity = (int) depthImage.at<uchar>(y, x);
            hist[intensity]++;
        }
    }
    cv::Mat histMat(1, histSize, CV_32F, hist);

    // gaussian blur histogram
    cv::GaussianBlur(histMat, histMat, cv::Size(19, 19), 2.0);

    // convert depthImage to 8bit grayscale
    cv::cvtColor(depthImage, depthImage, cv::COLOR_BGR2GRAY);


    // Finden des höchsten Werts im Tiefenbild
    double maxValue;
    cv::Point maxLoc;
    cv::minMaxLoc(depthImage, nullptr, &maxValue, nullptr, &maxLoc);
    //std::cout << maxLoc << std::endl;

    double histMaxValue;
    cv::minMaxLoc(histMat, nullptr, &histMaxValue, nullptr, nullptr);

    // normalize histogram
    histMat = (histMat / histMaxValue) * histHeight;

    // create histogram image
    cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0, 0, 0));
    int binWidth = cvRound((double) histWidth / histSize);

    // draw lines
    for (int i = 1; i < histSize; i++) {
        cv::line(histImage,
             cv::Point(binWidth * (i - 1), histHeight - cvRound(histMat.at<float>(i - 1))),
             cv::Point(binWidth * (i), histHeight - cvRound(histMat.at<float>(i))),
             cv::Scalar(255, 255, 255));
    }

    int locPos;
    std::cout << maxValue << std::endl;
    for (int x = maxValue - 1; x > 0; x--) {
        // first value above 200
        std::cout << "test" << std::endl;
        if (histMat.at<float>(x) > 200) {
            locPos = x;
            break;
        }
    }

    //std::cout << locPos << std::endl;

    int localMaxPos, localMinPos;

    // search local max
    for (localMaxPos = locPos; localMaxPos > 0; localMaxPos--) {
        if (histMat.at<float>(localMaxPos) > histMat.at<float>(localMaxPos - 1)) {
            break;
        }
    }
    // search local min
    for (localMinPos = localMaxPos; localMinPos > 0; localMinPos--) {
        if (histMat.at<float>(localMinPos) < histMat.at<float>(localMinPos-1) ) {
            break;
        }
    }

    //if localMinPos is 0 and localMaxPos is 0 -> set to -1
    if (localMinPos == 0 && localMaxPos == 0) {
        std::cerr << "No local min and max found" << std::endl;
        localMinPos = -1;
        localMaxPos = -1;
    }

    std::cout << localMinPos << " " << localMaxPos << " " << locPos << std::endl;

    cv::Scalar lineColor = cv::Scalar(255, 0, 0);

    // draw max line
    line(histImage,
         cv::Point(binWidth * localMaxPos, 0),
         cv::Point(binWidth * localMaxPos, histHeight),
         lineColor);

    // draw min line
    line(histImage,
         cv::Point(binWidth * localMinPos, 0),
         cv::Point(binWidth * localMinPos, histHeight),
         lineColor);

    cv::Mat binaryImage;

    // convert img to binary
    cv::threshold(depthImage, binaryImage, localMinPos, 255, cv::THRESH_BINARY);

    cv::imshow("Histogram", histImage);
    cv::imshow("Binary", binaryImage);
}

double schwellwert(cv::Mat depth) {
    float histo_vals[256] = {0};
    cv::Mat histogram(256, 256, CV_8UC1, cv::Scalar(0));

    double max, min;
    int Val200_x = -1;
    int rearMax = - 1;
    int prevLocalMin = -1;
    int prevValue;
    cv::Point maxLoc;
    cv::Point minLoc;

    // geting the histogram values
    for (int i = 0; i < depth.rows; i++) {
        for (int j = 0; j < depth.cols; j++) {
            histo_vals[depth.at<uchar>(i,j)]++;
        }
    }

    histo_vals[0] = 0;
    cv::Mat blurred(256,1,CV_32F,histo_vals);

    // creating the smoothed histogram with
    // Gaussian Blur of 19x19
    // Standard Deviation of 2.0
    cv::GaussianBlur(blurred,blurred,cv::Size(21,21),2.0);

    minMaxLoc(blurred,&min,&max,nullptr,nullptr);

    for (int i = 0; i < blurred.rows; i++) {
        blurred.at<float>(i,1) = 256 * (blurred.at<float>(i,1) / max);
    }

    // building the smoothed Histogram
    for (int i = 0; i < 255; i++) {
        cv::line(histogram,cv::Point(i,255 - blurred.at<float>(i)),cv::Point(i+1,255 - blurred.at<float>(i+1)),cv::Scalar(255));
    }

    cv::minMaxLoc(blurred,&min,&max,&minLoc,&maxLoc);

    // getting the x coordinate of the first value above 200
    for (int i = maxLoc.y; i >= 0; i--) {
        if (blurred.at<float>(i, 1) >= 200) {
            Val200_x = i;
            break;
        }
    }

    cout << "Val200_x: " << Val200_x << endl;

    // find the rear maximum value -> rearMax
    if (Val200_x != -1) {
        // Value of the first value above 200 at the x coordinate
        prevValue = blurred.at<float>(Val200_x, 0);

        for (int i = Val200_x - 1; i >= 0; i--) {
            int currentValue = blurred.at<float>(i, 0);

            // checks if the current value is larger than the value following that
            if (currentValue > rearMax) {
                rearMax = i;
            } else if (currentValue < prevValue) {
                // current maximum found -> values decrease => possible minimum
                for (; i >= 0; i--) {
                    int localMinValue = blurred.at<float>(i, 0);
                    // stops if larger value is found => values increase again
                    if (localMinValue > prevValue) {
                        prevLocalMin = prevValue;
                        break;
                    }
                    prevValue = localMinValue;
                }
                break;
            }

            prevValue = currentValue;
        }
    }

    // draw vertical lines
    cv::line(histogram, cv::Point(rearMax, 0), cv::Point(rearMax, 256), cv::Scalar(255,0,0));
    cv::line(histogram, cv::Point(prevLocalMin, 0), cv::Point(prevLocalMin, 256), cv::Scalar(255,0,0));

    imshow("Histogram",histogram);
    cv::waitKey(0);

    return prevLocalMin;
}

int main(int argc, char *argv[]) {
    using namespace std;
    int mode = std::atoi(argv[1]);


    //-------------------------------------------Videos einlesen---------------------------------------------
    if (mode == 3) {
        cv::VideoCapture capRgb("rgb.avi");
        cv::VideoCapture capDepth("depth.avi");
        cv::VideoCapture capChildren("children.mp4");
        // check if we succeeded
        if (!capRgb.isOpened() || !capDepth.isOpened() || !capChildren.isOpened()) {
            cerr << "ERROR! Unable to open video\n";
            return -1;
        }

        cv::Mat rgbFrame;
        cv::Mat depthFrame;
        cv::Mat childrenFrame;
        double thresholdVal;
        while (true) {
            /*capRgb.read(rgbFrame);
            capDepth.read(depthFrame);*/
            capChildren.read(childrenFrame);

            //calculateThreshold(depthFrame);

            /*if (rgbFrame.empty() || depthFrame.empty() || childrenFrame.empty()) {
                cerr << "ERROR! blank frame grabbed\n";
                break;
            }*/

            cv::Rect myROI(0, 50, 430, 310);
            childrenFrame = childrenFrame(myROI);

            //convert rgb video to 8bit grayscale
            //cv::cvtColor(childrenFrame, childrenFrame, cv::COLOR_BGR2GRAY);

            calculateThreshold(childrenFrame);

            /*thresholdVal = schwellwert(childrenFrame);

            if (thresholdVal < 0) {
                thresholdVal = 0;
            }
            threshold(childrenFrame,childrenFrame,thresholdVal,255,cv::THRESH_OTSU);*/

            //cv::imshow("Video RGB", rgbFrame);
            //cv::imshow("Video Depth", depthFrame);
            cv::imshow("Video Children", childrenFrame);

            int key = cv::waitKey(2);
            if (key == 'q' || key == 'Q') {
                return 0;
            }
        }

        /*}else{
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

            // Linking
            camRgb->isp.link(rgbOut->input);
            left->out.link(stereo->left);
            right->out.link(stereo->right);
            stereo->disparity.link(depthOut->input);
            stereo->setExtendedDisparity(true);

            // Connect to device and start pipeline
            device.startPipeline(pipeline);

            // Sets queues size and behavior
            for (const auto &name: queueNames) {
                device.getOutputQueue(name, 4, false);
            }

            std::unordered_map<std::string, cv::Mat> frame;

            auto rgbWindowName = "rgb";
            auto depthWindowName = "depth";
            cv::namedWindow(rgbWindowName);
            cv::namedWindow(depthWindowName);

            //-------------------------------------- Ende des Setups/ config der kamera ----------------------------
            cv::VideoWriter videoRgb("rgb.avi", 0, 24, cv::Size(1280, 720));
            cv::VideoWriter videoDepth("depth.avi", 0, 24, cv::Size(1280, 720));

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
                            if (1) frame[name].convertTo(frame[name], CV_8UC1, 255. / maxDisparity);
                            calculateThreshold(frame[name]);
                            // Optional, apply false colorization
                            //if (1) cv::applyColorMap(frame[name], frame[name], cv::COLORMAP_JET);

                            //apply medianFilter 7by7
                            cv::medianBlur(frame[name], frame[name], 7);

                            // Extract confidence from depth values
                            cv::Mat confidence = frame[name].clone();

                            // Set confidence values below threshold to 0 and above threshold to 200
                            confidence.setTo(0, frame[name] <= 200);
                            confidence.setTo(200, frame[name] > 200);

                            // Only keep reliable depth values
                            cv::Mat trustedDepth;
                            cv::bitwise_and(frame[name], confidence, trustedDepth);

                            videoDepth.write(frame[name]);
                        } else {
                            frame[name] = latestPacket[name]->getCvFrame();
                            videoRgb.write(frame[name]);
                        }
                        cv::imshow(name, frame[name]);
                    }
                }

                // Blend when both received
                if (frame.find(rgbWindowName) != frame.end() && frame.find(depthWindowName) != frame.end()) {
                    // Need to have both frames in BGR format before blending
                    if (frame[depthWindowName].channels() < 3) {
                        cv::cvtColor(frame[depthWindowName], frame[depthWindowName], cv::COLOR_GRAY2BGR);
                    }
                    cv::Mat blended;
                    cv::addWeighted(frame[rgbWindowName], rgbWeight, frame[depthWindowName], depthWeight, 0, blended);
                    frame.clear();
                }

                int key = cv::waitKey(1);
                if (key == 'q' || key == 'Q') {
                    videoDepth.release();
                    videoRgb.release();
                    return 0;
                }
            }
        }*/
    }


    return 0;
}
