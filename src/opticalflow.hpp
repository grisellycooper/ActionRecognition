#include <vector>
#include <iostream>
#include <string>
#include "cuboid.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "showmat.hpp"

#define debug 1

using namespace cv;
using namespace std;

class opticalflow
{
  private:
    int logQuantization = 1; // ?
    int nBinsMagnitude = 8;  //?
    int maxMagnitude = 15;   // ?
    float maxAngle = 361.0;
    int nBinsAngle = 8;
    bool isDebug = true;
  public:
    opticalflow(/* args */);
    void getOpticalFlowCuboid(vector<Mat> &images,
                              vector<Mat *> &matMagnitude,
                              vector<Mat *> &matOrientation,
                              int step);

    Mat *getOrientation(vector<Point2f> cornersImagePrev,
                        vector<Point2f> cornersImageNext, Size  );

    Mat *getMagnitude(vector<Point2f> cornersImagePrev,
                      vector<Point2f> cornersImageNext, Size imageSize);

    // Desctructor
    ~opticalflow();
};

/**
 * Implement
*/

void opticalflow::getOpticalFlowCuboid(
    vector<Mat> &images
    , vector<Mat *> &matMagnitude
    , vector<Mat *> &matOrientation
    , int step = 1
    )
{
    // Get statusFeature : each element of the vector is:
    // = 1,  the flow for the corresponding features has been found
    // otherwise = 0.
    //cout << "get optical " << endl;
    Mat movementTest; 
    Size imageSize = images.at(0).size();
    Size winSize = Size(21, 21);
    Size winSizeMin = Size(10, 10);
    int threshold = 60;
    //cout << "total imgs " << images.size() << endl;
    for (int idx = 0; idx < images.size() - step; idx = idx + step) {
        
        if(debug == 1)
            movementTest = Mat::zeros(images[0].rows, images[1].cols, CV_8UC3); 
        
        // cout << "idx: " << idx << endl;
        //Define points
        Mat imagePrev = images.at(idx).clone();        //  GrayScale
        Mat imageNext = images.at(idx + step).clone(); // GrayScale

        vector<Point2f> cornersImagePrev, cornersImageNext;

        cvtColor(imagePrev, imagePrev, COLOR_BGR2GRAY);
        cvtColor(imageNext, imageNext, COLOR_BGR2GRAY);

        Mat frameDif = cv::abs(imageNext - imagePrev);
        for (int i = 0; i < frameDif.rows; ++i) {
            for (int j = 0; j < frameDif.cols; ++j) {
                if (frameDif.at<uchar>(i, j) > threshold) {
                    cornersImagePrev.push_back(Point2f((float)(j), (float)(i)));
                    if(debug == 1)
                        circle(movementTest, Point2f((float)(j), (float)(i)), 1, cv::Scalar(0, 0, 255), 1, 4);
                }
            }
        }
        if(debug == 1)
            showMat(movementTest, "FrameDiff", true, false);

        vector<uchar> statusFeat;

        // Verify points
        if (cornersImagePrev.size() > 0) {
            calcOpticalFlowPyrLK(
                imagePrev,        // Previous image
                imageNext,        // Next image
                cornersImagePrev, // Previous set of corners (from imagePrev)
                cornersImageNext, // Next set of corners (from imageNext)
                statusFeat,       // Output vector, each is 1 for tracked
                noArray(),        // Output vector, lists errors (optional)
                winSize,          // Search window size
                5,                // Maximum pyramid level to construct
                TermCriteria(
                    TermCriteria::MAX_ITER | TermCriteria::EPS,
                    20, // Maximum number of iterations
                    0.3 // Minimum change per iteration
                    ),
                0,      // Flags
                0.001); // min

            Mat *orientation = getOrientation(cornersImagePrev, cornersImageNext, imageSize);
            matOrientation.push_back(orientation);

            Mat *magnitude = getMagnitude(cornersImagePrev, cornersImageNext, imageSize);
            matMagnitude.push_back(magnitude);

        } else {
            matMagnitude.push_back(NULL);
            matOrientation.push_back(NULL);
            std::cout<<"Frame " << idx << " has no movement"<< std::endl;
        }
    }
}

Mat *opticalflow::getMagnitude(vector<Point2f> cornersImagePrev,
                               vector<Point2f> cornersImageNext,
                               Size imageSize)
{
    // Mij = sqrt(u2_i,j + v2_i,j)
    Mat *magnitud = new Mat(imageSize, CV_16SC1, -1);

    Mat magnitudeTest = Mat::zeros(imageSize, CV_8UC3); // Debug    

    for (int i = 0; i < cornersImagePrev.size(); ++i)
    {
        float magnitude = sqrt(pow((cornersImagePrev[i].x - cornersImageNext[i].x), 2) +
                               (pow((cornersImagePrev[i].y - cornersImageNext[i].y), 2)));

        int valMagnitude;
        if (this->logQuantization == 1) {
            valMagnitude = (int)(floor(log2(magnitude)));
        } else {
            valMagnitude = (int)(floor(magnitude / (maxMagnitude / nBinsMagnitude)));
        }

        valMagnitude = valMagnitude < 0 ? 0 : valMagnitude;

        if (valMagnitude >= nBinsMagnitude) {
            valMagnitude = nBinsMagnitude - 1;
        }

        int y = (int)(cornersImagePrev[i].y);
        int x = (int)(cornersImagePrev[i].x);
        
        *(magnitud->ptr<short>(y, x)) = valMagnitude;
       // cout << "real: " << valMagnitude << "h: " <<*(magnitud->ptr<int>(y, x)) << endl ;

        if(debug == 1){
            if(valMagnitude > 1)
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 50, 0), 1, 4);
            if(valMagnitude > 2)
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 100, 0), 1, 4);
            if(valMagnitude > 3)    
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 150, 0), 1, 4);
            if(valMagnitude > 4)
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 200, 0), 1, 4);
            if(valMagnitude > 5)
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 250, 0), 1, 4);
            if(valMagnitude > 6)
                circle(magnitudeTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(255, 255, 255), 1, 4);
        } 
    }

    if(debug == 1)
        showMat(magnitudeTest, "Magnitude", true, true);

    return magnitud;
}

Mat *opticalflow::getOrientation(vector<Point2f> cornersImagePrev,
                                 vector<Point2f> cornersImageNext,
                                 Size imageSize)
{
    //angulo
    Mat * orientation = new Mat(imageSize, CV_16SC1, -1);
    int size = cornersImagePrev.size();

    Mat orientationTest = Mat::zeros(imageSize, CV_8UC3); // Debug    

    for (int i = 0; i < size; ++i) {

        float dx = cornersImagePrev[i].x - cornersImageNext[i].x;
        float dy = cornersImagePrev[i].y - cornersImageNext[i].y;

        float angle = (float)(atan2f(dy, dx) * 180 / CV_PI);
        if (angle < 0) {
            angle += 360;
        }

        float valAngle = (float)(floor(angle / (this->maxAngle / this->nBinsAngle)));

        int x = (int)(cornersImagePrev[i].x);
        int y = (int)(cornersImagePrev[i].y);
        
        *(orientation->ptr<short>(y, x)) = valAngle;

        if(debug == 1){
            if(valAngle > 3)
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 0, 50), 1, 4);
            if(valAngle > 4)
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 0, 100), 1, 4);
            if(valAngle > 5)    
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 0, 150), 1, 4);
            if(valAngle > 6)
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 0, 200), 1, 4);
            if(valAngle > 7)
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(0, 0, 250), 1, 4);
            if(valAngle > 8)
                circle(orientationTest, Point2f((float)(x), (float)(y)), 1, cv::Scalar(255, 255, 255), 1, 4);
        }            
    }
    if(debug == 1)
        showMat(orientationTest, "Orientation", true, true);
    return orientation;
}

opticalflow::opticalflow(/* args */)
{
}

opticalflow::~opticalflow()
{
}
