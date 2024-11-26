#include "opencv2/opencv.hpp"

using namespace cv;

int main() {

    Mat bild = imread("/opt/clion/homeverzeichnis/ue4/muenzen.bmp", 0);
    Mat blur = bild.clone();

    medianBlur(bild, blur, 3);
    Mat sobelX = blur.clone();
    Mat sobelY = blur.clone();
    Mat sobelAbs;
    Sobel(blur, sobelX, CV_32F, 1, 0);
    Sobel(blur, sobelY, CV_32F, 0, 1);


    double min, max;
    minMaxLoc(sobelX, &min, &max);
    double scale = 255 / (max - min);
    convertScaleAbs(sobelX, sobelAbs, scale, min * scale);
    imshow("sobelXAbs", sobelAbs);

    minMaxLoc(sobelY, &min, &max);
    scale = 255 / (max - min);
    convertScaleAbs(sobelY, sobelAbs, scale, min * scale);
    imshow("sobelYAbs", sobelAbs);

    Mat mangi;
    magnitude(sobelX, sobelY, mangi);

    minMaxLoc(mangi, &min, &max);
    scale = 255 / (max - min);
    convertScaleAbs(mangi, sobelAbs, scale, min * scale);
    imshow("sobelMagnituteAbs", sobelAbs);

    Mat bildColor;
    cvtColor(bild, bildColor, COLOR_GRAY2RGB, 3);


    for (int i = 4; i < bildColor.rows; i += 5) {
        for (int j = 4; j < bildColor.cols; j += 5) {
            if (sobelAbs.at<uchar>(i, j) > 50) {
                arrowedLine(bildColor, Point(j, i),
                            Point(j + (int) (sobelX.at<float>(i, j) / 10), i + (int) (sobelY.at<float>(i, j) / 10)),
                            Scalar(255, 0, 0), 1, 8, 0, 0.05);
            }
        }
    }


    imshow("bildColor", bildColor);

    waitKey(0);
    return 0;

}