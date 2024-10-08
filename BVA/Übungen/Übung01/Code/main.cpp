#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat bild;
    bild = imread("/Users/christophriedel/Programming/HSNR-Semester5/BVA/Übungen/Übung01/zelle_grau.bmp", 0);
    cout << "Bildgröße:" << bild.size << endl << "Bild Channel:" << bild.channels() << endl;
    Scalar mean, stddev;
    meanStdDev(bild, mean, stddev);
    cout << "Mittlewert" << mean[0] << endl << "Standard Abweichung:" << stddev[0] << endl;
    double minVal;
    double maxVal;
    minMaxLoc(bild, &minVal, &maxVal);
    cout << "Min Grau:" << minVal << endl << "Max Grau:" << maxVal << endl;
    Mat graubig;
    compare(bild, mean, graubig , CMP_GT );
    cout << "Weiß Pixel:" << countNonZero(graubig) << endl;
    imshow("tes2", graubig);
    waitKey(0);
    imshow("Test", bild);
    waitKey(0);
    Mat color1;
    cvtColor(bild, color1, COLOR_GRAY2BGR);
    imshow("Test", color1);
    for(int i = 0;i < bild.rows; i++){
        for(int j = 0; j < bild.cols; j++){
            if(graubig.at<uchar>(i,j) == 255){
                color1.at<Vec3b>(i,j) = Vec3b(0,0,255);
            }
        }
    }
    imshow("Test", color1);
    waitKey(0);
    return 0;
}

    Mat bild;
    bild = imread("/home/forall/Downloads/zelle_grau(2).bmp", 0);
    cout << "Bildgröße:" << bild.size << endl << "Bild Channel:" << bild.channels() << endl;
    Scalar mean, stddev;
    meanStdDev(bild, mean, stddev);
    cout << "Mittlewert" << mean[0] << endl << "Standard Abweichung:" << stddev[0] << endl;
    double minVal;
    double maxVal;
    minMaxLoc(bild, &minVal, &maxVal);
    cout << "Min Grau:" << minVal << endl << "Max Grau:" << maxVal << endl;
    Mat graubig;
    compare(bild, mean, graubig , CMP_GT );
    cout << "Weiß Pixel:" << countNonZero(graubig) << endl;
    imshow("tes2", graubig);
    waitKey(0);
    imshow("Test", bild);
    waitKey(0);
    Mat color1;
    cvtColor(bild, color1, COLOR_GRAY2BGR);
    imshow("Test", color1);
    for(int i = 0;i < bild.rows; i++){
        for(int j = 0; j < bild.cols; j++){
            if(graubig.at<uchar>(i,j) == 255){
                color1.at<Vec3b>(i,j) = Vec3b(0,0,255);
            }
        }
    }
    imshow("Test", color1);
    waitKey(0);
    return 0;
}