#include <iostream>
#include <opencv2/opencv.hpp>



using namespace cv;

int main ( int argc, char** argv)
{

    uchar lookup[256] = {};
    uchar lookup2[256] = {};
    double histo[256] = {};

    double minVal, maxVal;
    Mat bild;
    bild = imread("/home/forall/test/London.bmp", 0 );
    Mat bild_zwei = bild.clone();
    Mat bild_drei  = bild.clone();
    minMaxLoc(bild, &minVal, &maxVal);
    for (int i = 0; i < 256; ++i) {
        lookup[i] = ((uchar)i - (uchar) minVal) * ((255.0) / ((uchar) maxVal- (uchar)minVal));
    }

    //histoohnocringo

    for(int i=0; i < bild.rows; i++)
        for(int j=0; j < bild.cols; j++)
            histo[bild.at<uchar>(i,j)] += 1;

    for(int i=0; i < 256; i++)
            histo[i] /= bild.size[0] *  bild.size[1] ;


    uchar g_min_rob;
    uchar g_max_rob;

    double threshold = 0;
    for(int i=0; i < 256; i++){
        threshold += histo[i];
        if (threshold >= 0.1){
            g_min_rob = i-1;
            break;
        }
    }
    threshold = 0;
    for(int i=0; i < 256; i++){
        threshold += histo[i];
        if (threshold >= 0.9){
            g_max_rob = i-1;
            break;
        }
    }


    for (int i = 0; i < 256; ++i) {
        lookup2[i] = ((uchar)i - (uchar) g_min_rob) * ((255.0) / ((uchar) g_max_rob- (uchar)g_min_rob));
    }











    for(int i=0; i < bild.rows; i++)
        for(int j=0; j < bild.cols; j++)
            bild_zwei.at<uchar>(i,j) = lookup[bild.at<uchar>(i,j)];


    for(int i=0; i < bild.rows; i++)
        for(int j=0; j < bild.cols; j++)
            bild_drei.at<uchar>(i,j) = lookup2[bild.at<uchar>(i,j)];




    imshow("Test", bild);
    imshow("Test2",bild_zwei);
    imshow("Test3",bild_drei);
    waitKey(0);
    return 0;
}