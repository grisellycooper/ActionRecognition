#include <vector>
#include <iostream>
#include <string>
#include "cuboid.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class opticalflow
{
private:
    /* data */
public:
    opticalflow(/* args */);
    void getOpticalFlowCuboid(vector<Mat> &images, 
        vector<vector<Point2f>> &prevPoints,
        vector<vector<Point2f>> & nextPoints,
        vector<uchar> &statusFeature,
        int step);

    // Desctructor
    ~opticalflow();
};

/**
 * Implement
*/

void opticalflow::getOpticalFlowCuboid(vector<Mat> &images, 
        vector<vector<Point2f>> &prevPoints,
        vector<vector<Point2f>> & nextPoints,
        vector<uchar> &statusFeature,
        int step)
{
    // Get statusFeature : each element of the vector is:
    // = 1,  the flow for the corresponding features has been found
    // otherwise = 0.

    Size imageSize = images.at(0).size();
    static const int MAX_CORNERS = 1000;
    Size winSize = Size(21, 21);
    Size winSizeMin = Size(10, 10);

    for(int idx = 0; idx < images.size - step; idx+step)
    {
        Mat imagePrev = images.at(idx); //  GrayScale
        Mat imageNext = images.at(idx + step); // GrayScale
        Mat imgResult = images.at(idx + step);

        vector<Point2f> cornersImagePrev, cornersImageNext;

        goodFeaturesToTrack(
            imagePrev,         // Image to track
            cornersImagePrev,      // Vector of detected corners (output)
            MAX_CORNERS,   // Keep up to this many corners
            0.01,          // Quality level (percent of maximum)
            5,             // Min distance between corners
            cv::noArray(), // Mask
            3,             // Block size
            false,         // true: Harris, false: Shi-Tomasi
            0.04           // method specific parameter
        );

        cornerSubPix(
            imagePrev,                        // Input image
            cornersImagePrev,                     // Vector of corners (input and output)
            winSizeMin, // Half side length of search window
            Size(-1, -1),             // Half side length of dead zone (-1=none)
            TermCriteria(
                cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
                20,  // Maximum number of iterations
                0.03 // Minimum change per iteration
                ));

        vector<uchar> statusFeat;

        calcOpticalFlowPyrLK(
            imagePrev,                                        // Previous image
            imageNext,                                       // Next image
            cornersImagePrev,                                     // Previous set of corners (from imagePrev)
            cornersImageNext,                                     // Next set of corners (from imageNext)
            statusFeat,                               // Output vector, each is 1 for tracked
            noArray(),                                // Output vector, lists errors (optional)
            winSize, // Search window size
            5,                                            // Maximum pyramid level to construct
            TermCriteria(
                cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
                20, // Maximum number of iterations
                0.3 // Minimum change per iteration
                ));
    }
    
    //vector cornersImagePrev  contains start points
    //vector cornersImagePrev  contains end points

   
}

opticalflow::opticalflow(/* args */)
{
}

opticalflow::~opticalflow()
{
}
