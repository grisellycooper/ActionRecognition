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
    int logQuantization = 1; // ?
    int nBinsMagnitude = 8;  //?
    int maxMagnitude = 15;   // ?
    float maxAngle = 361.0;
    int nBinsAngle = 8;
  public:
    opticalflow(/* args */);
    void getOpticalFlowCuboid(vector<Mat> &images,
                              vector<Mat *> &matMagnitude,
                              vector<Mat *> &matOrientation,
                              int step);

    Mat *getOrientation(vector<Point2f> cornersImagePrev,
                        vector<Point2f> cornersImageNext, Size imageSize);

    Mat *getMagnitude(vector<Point2f> cornersImagePrev,
                      vector<Point2f> cornersImageNext, Size imageSize);

    // Desctructor
    ~opticalflow();
};

/**
 * Implement
*/

void opticalflow::getOpticalFlowCuboid(vector<Mat> &images,
                                       vector<Mat *> &matMagnitude,
                                       vector<Mat *> &matOrientation,
                                       int step)
{
    // Get statusFeature : each element of the vector is:
    // = 1,  the flow for the corresponding features has been found
    // otherwise = 0.
    cout << "get optical " << endl;
    Size imageSize = images.at(0).size();
    Size winSize = Size(21, 21);
    Size winSizeMin = Size(10, 10);
    int threshold = 30;
    cout << "total imgs " << images.size() << endl;
    for (int idx = 0; idx < images.size() - step; idx = idx + step)
    {
        cout << "idx: " << idx << endl;
        //Define points
        Mat imagePrev = images.at(idx);        //  GrayScale
        Mat imageNext = images.at(idx + step); // GrayScale

        vector<Point2f> cornersImagePrev, cornersImageNext;

        cvtColor(imagePrev, imagePrev, CV_BGR2GRAY);
        cvtColor(imageNext, imageNext, CV_BGR2GRAY);

        Mat frameDif = cv::abs(imageNext - imagePrev);
        for (int i = 0; i < frameDif.rows; ++i)
        {
            for (int j = 0; j < frameDif.cols; ++j)
            {
                if (frameDif.at<uchar>(i, j) > threshold)
                    cornersImagePrev.push_back(Point2f(static_cast<float>(j),
                                                       static_cast<float>(i)));
            }
        }

        cout << "corners " << cornersImagePrev.size() << endl;

        vector<uchar> statusFeat;

        //Verify points
        if (cornersImagePrev.size() > 0)
        {
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
                    cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
                    20, // Maximum number of iterations
                    0.3 // Minimum change per iteration
                    ),
                0,      // Flags
                0.001); // min

            //Create orientation matriz
            Mat *orientation = getOrientation(cornersImagePrev, cornersImageNext, imageSize);
            matOrientation.push_back(orientation);

            //Create magnitud matriz
            Mat *magnitude = getMagnitude(cornersImagePrev, cornersImageNext, imageSize);
            matMagnitude.push_back(magnitude);
        }
        else
        {
            matMagnitude.push_back(NULL);
            matOrientation.push_back(NULL);
            cout << "no matrices"<< endl;

        }
    }
}

Mat *opticalflow::getMagnitude(vector<Point2f> cornersImagePrev,
                               vector<Point2f> cornersImageNext,
                               Size imageSize)
{
    // Mij = sqrt(u2_i,j + v2_i,j)
    Mat *magnitud = new Mat(imageSize, CV_16SC1, -1);

    for (int i = 0; i < cornersImagePrev.size(); ++i)
    {

        float magnitude = sqrt(pow((cornersImagePrev[i].x - cornersImageNext[i].x), 2) +
                               (pow((cornersImagePrev[i].y - cornersImageNext[i].y), 2)));

        int valMagnitude;
        if (this->logQuantization == 1)
        {
            valMagnitude = static_cast<int>(floor(log2(magnitude)));
        }
        else
        {
            valMagnitude = static_cast<int>(floor(magnitude / (maxMagnitude / nBinsMagnitude)));
        }

        if (valMagnitude < 0)
        {
            valMagnitude = 0; 
        }

        if (valMagnitude >= nBinsMagnitude)
        {
            valMagnitude = nBinsMagnitude - 1;
        }

        int idx = static_cast<int>(cornersImagePrev[i].x);
        int idy = static_cast<int>(cornersImagePrev[i].y);
        //cout << "eval: " << valMagnitude << endl;
        magnitud->at<int>(idy, idx) = valMagnitude;

    }
    return magnitud;
}

Mat *opticalflow::getOrientation(vector<Point2f> cornersImagePrev,
                                 vector<Point2f> cornersImageNext,
                                 Size imageSize)
{
    //angulo
    Mat *orientation = new Mat(imageSize, CV_16SC1, -1);

    for (int i = 0; i < cornersImagePrev.size(); ++i)
    {

        float catetoAdyacente = cornersImagePrev[i].x - cornersImageNext[i].x;
        float catetoOpuesto = cornersImagePrev[i].y - cornersImageNext[i].y;
        float angle = static_cast<float>(atan2f(catetoOpuesto, catetoAdyacente) * 180 / CV_PI);
        if (angle < 0)
        {
            angle += 360;
        }

        float valAngle = static_cast<float>(floor(angle / (this->maxAngle /
                                                     this->nBinsAngle)));

        int idx = static_cast<int>(cornersImagePrev[i].x);
        int idy = static_cast<int>(cornersImagePrev[i].y);
        //cout << "ang: " << valAngle << endl;
        orientation->at<int>(idy,idx) = valAngle;

    }

        return orientation;
}

opticalflow::opticalflow(/* args */)
{
}

opticalflow::~opticalflow()
{
}
