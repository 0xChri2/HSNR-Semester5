#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

void image_filter(Mat org, Mat res, Mat mask) {
    std::cout << "C = " << std::endl << " " << mask << std::endl << std::endl;
    for (int i = mask.rows / 2; i < org.rows - mask.rows / 2; ++i) {
        for (int j = mask.cols / 2; j < org.cols - mask.cols / 2; ++j) {
            int sum = 0;
            for (int k = -mask.rows / 2; k <= mask.rows / 2; ++k) {
                for (int l = -mask.cols / 2; l <= mask.cols / 2; ++l) {
                    sum += (int) ((float) org.at<u_char>(i + k, j + l) *
                                  mask.at<float>(k + mask.rows / 2, l + mask.cols / 2));
                }
            }
            res.at<u_char>(i, j) = (u_char) round((double) sum / cv::sum(mask).val[0]);
        }
    }
}

int main() {
    Mat bild = imread("/opt/clion/homeverzeichnis/untitled/plant.png", 0);
    Mat temp = bild.clone();
    Mat res3x3 = bild.clone();
    Mat res7x7 = bild.clone();
    Mat res7x1x7 = bild.clone();

    Mat mask3x3 = (Mat_<float>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
    Mat mask1x7 = (Mat_<float>(1, 7) << 1, 6, 15, 20, 15, 6, 1);
    Mat mask7x1 = (Mat_<float>(7, 1) << 1, 6, 15, 20, 15, 6, 1);
    Mat mask7x7;
    gemm(mask7x1, mask1x7, 1, noArray(), 0, mask7x7);

    image_filter(bild, res3x3, mask3x3);
    image_filter(res3x3, temp, mask3x3);
    image_filter(temp, res3x3, mask3x3);


    image_filter(bild, res7x7, mask7x7);

    image_filter(bild, temp, mask7x1);
    image_filter(temp, res7x1x7, mask1x7);

    imshow("res3x3", res3x3);
    imshow("res7x7", res7x7);
    imshow("res7x1x7", res7x1x7);
    imshow("org", bild);
    waitKey(0);
}