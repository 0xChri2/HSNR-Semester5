
#include "opencv2/opencv.hpp"

using namespace cv;

std::vector<Point> findMaxLines(Mat bild, int k){
    Mat bild2 = bild.clone();
    std::vector<Point> linePoints;
    for (int i = 0; i < k; ++i) {
        Point maxPoint;
        minMaxLoc(bild2, nullptr, nullptr, nullptr, &maxPoint);
        linePoints.push_back(maxPoint);
        bild2.at<u_char>(maxPoint.x, maxPoint.y) = 0;
    }
    return linePoints;
}


void HoughLines(Mat bild){
    Mat acc((int)sqrt(bild.rows * bild.rows + bild.cols * bild.cols ), 180,CV_32F,Scalar(0));
    int middleX = bild.cols / 2;
    int middleY = bild.rows / 2;
    for (int i = 0; i < bild.rows; ++i) {
        for (int j = 0; j < bild.cols; ++j) {
            if (bild.at<u_char>(i,j) == 255){
                int x = middleX - j;
                int y = middleY - i;
                for (int k = 0; k < 180; ++k) {
                    float r = (float) x * (float) cos( (float) k /180 * M_PI) + (float) y * (float) sin( (float) k /180 * M_PI) ;
                    acc.at<float>((sqrt(bild.rows * bild.rows + bild.cols * bild.cols ) / 2.0) + (int)r,k) += 1;
                }
            }
        }
    }
    Mat acc2 = acc.clone();
    double min, max;
    minMaxLoc(acc, &min, &max);
    double scale = 255 / (max - min);
    convertScaleAbs(acc, acc2, scale, min * scale);
    imshow("Hough Bild", acc2);
    waitKey(0);

    //std::vector<Point> linePoints2 =  findMaxLines(acc, 20);
}

int main() {

    Mat bild = imread("/opt/clion/homeverzeichnis/uebung05/fassade.png", 0);
    Mat canny;
    Canny(bild, canny , 100, 250);
    std::cout << canny.type() << std::endl;
    HoughLines(canny);

    return 0;

}