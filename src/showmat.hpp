#include "common.h"

using namespace std;
using namespace cv;

void showMat(Mat mat, string windName, bool display, bool continuosly)
{
    if(display){
        cv::namedWindow(windName, cv::WINDOW_NORMAL);
        imshow(windName, mat);    

        if (continuosly){
            cv::waitKey(50);            
        }
        else {
            if (cv::waitKey() == 27)
                cv::waitKey(100);
        }
    }
}