#ifndef OFCM_H
#define OFCM_H

#include "common.h"
#include "cooccurrenceMatrix.hpp"
#include "cuboid.hpp"
#include "haralick.hpp"

using namespace std;
using namespace cv;

class OFCM
{
    vector<cv::Mat> mImages;
    bool mIsPrepared;

    int nBinsMagnitude;
    int nBinsAngle;
    int distanceMagnitude;
    int distanceAngle;
    int cuboidLength;
    float maxMagnitude;
    int maxAngle;
    int logQuantization;

    CoOccurrenceMatrix *comMagnitude;
    CoOccurrenceMatrix *comAngles;
    vector<int> temporalScales;

    int numOpticalFlow;
    int descriptorLength;

    bool movementFilter = true;
    int thr1 = 60;
    int **mapToOpticalFlows;
    int numImgs;
    typedef std::pair<cv::Mat_<int>, cv::Mat_<int>> ParMat;
    vector<ParMat> data;

    void defaults()
    {
        this->mIsPrepared = false;
        this->maxAngle = 361;
        comMagnitude = NULL;
        comAngles = NULL;
    }

  public:
    OFCM(
        int nBinsMagnitude, int nBinsAngle, int distanceMagnitude, int distanceAngle, int cuboidLength, float maxMagnitude, int logQuantization, vector<int> temporalScales)
    {
        defaults();
        this->nBinsMagnitude = nBinsMagnitude;
        this->nBinsAngle = nBinsAngle;
        this->distanceMagnitude = distanceMagnitude;
        this->distanceAngle = distanceAngle;
        this->cuboidLength = cuboidLength;

        if (logQuantization == 1)
        {
            this->logQuantization = logQuantization;
            this->maxMagnitude = 15;
        }
        else
        {
            this->maxMagnitude = maxMagnitude;
        }
        for (auto v : temporalScales)
        {
            this->temporalScales.push_back(v);
        }
    }

    int computeOpticalFlowPerCuboid()
    {
        int res = 0;
        for (int i = 0; i < this->cuboidLength; i++)
        {
            for (int j : temporalScales)
            {
                int k = i + j;
                if (k < this->cuboidLength)
                {
                    res++;
                }
                else
                {
                    break;
                }
            }
        }
        return res;
    }

    void setParameters()
    {
        int tamVecMagniMatrices = 4 * (this->nBinsMagnitude * this->nBinsMagnitude);
        int tamVecAngleMatrices = 4 * (this->nBinsAngle * this->nBinsAngle);

        comMagnitude = new CoOccurrenceMatrix(
            this->nBinsMagnitude, this->distanceMagnitude);
        comAngles = new CoOccurrenceMatrix(
            this->nBinsAngle, this->distanceAngle);

        this->numOpticalFlow = computeOpticalFlowPerCuboid();
        this->descriptorLength = ((4 * 12) + (4 * 12)) * this->numOpticalFlow;
    }

    void setOpticalFlowData()
    {
        cv::Mat mag = cv::Mat::zeros(mImages[0].rows, mImages[0].cols, CV_8UC3);
        cv::Mat ang = cv::Mat::zeros(mImages[0].rows, mImages[0].cols, CV_8UC3);

        int maxMag = 0, maxAng = 0;
        data.clear();
        vector<cv::Point2f> points[2], basePoints;
        vector<uchar> status;
        vector<float> err;
        cv::Size winSize(31, 31);
        cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.3);
        int rows, cols;

        rows = mImages[0].rows;
        cols = mImages[0].cols;

        this->numImgs = static_cast<int>(mImages.size());
        allocateMapToOpticalFlowsMatrix();

        for (int t : this->temporalScales)
        {
            for (int i = 0; i < static_cast<int>(mImages.size()); i++)
            {
                int j = i + t;
                if (j < static_cast<int>(mImages.size()))
                {
                    FillPoints(points[0], mImages[j], mImages[i], this->thr1);

                    ParMat angles_magni;
                    angles_magni.first = cv::Mat(rows, cols, CV_16SC1, -1);  // angles
                    angles_magni.second = cv::Mat(rows, cols, CV_16SC1, -1); // magnitude

                    if (points[0].size() > 0)
                    {
                        calcOpticalFlowPyrLK(mImages[i], mImages[j], points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
                        VecDesp2Mat(points[1], points[0], angles_magni);
                    }

                    this->data.push_back(angles_magni);
                    this->mapToOpticalFlows[i][j] = static_cast<int>(this->data.size()) - 1;
                    this->mapToOpticalFlows[j][i] = static_cast<int>(this->data.size()) - 1;

                    for (int a = 0; a < rows; a++){
                        for (int b = 0; b < cols; b++){
                            if(angles_magni.first[a][b] > maxAng)
                                maxAng = angles_magni.first[a][b];
                            if(angles_magni.first[a][b] > 0){
                                //std::cout<<" "<<angles_magni.first[a][b] ;
                                circle(ang, cv::Point2f(a,b), 1, cv::Scalar(0, 0, 50), 3, 8, 0);
                            }
                            if(angles_magni.first[a][b] > 2){
                                //std::cout<<" "<<angles_magni.first[a][b] ;
                                circle(ang, cv::Point2f(a,b), 1, cv::Scalar(0, 0, 50), 3, 8, 0);
                            }
                            if(angles_magni.first[a][b] > 4){
                                //std::cout<<" "<<angles_magni.first[a][b] ;
                                circle(ang, cv::Point2f(a,b), 1, cv::Scalar(0, 0, 150), 3, 8, 0);
                            }
                            if(angles_magni.first[a][b] > 6){
                                //std::cout<<" "<<angles_magni.first[a][b] ;
                                circle(ang, cv::Point2f(a,b), 1, cv::Scalar(0, 0, 200), 3, 8, 0);
                            }
                            if(angles_magni.first[a][b] > 8){
                                //std::cout<<" "<<angles_magni.first[a][b] ;
                                circle(ang, cv::Point2f(a,b), 1, cv::Scalar(0, 0, 250), 3, 8, 0);
                            }   
                        }                        
                    }
                    std::cout<<"MaxAngle: "<<maxAng <<std::endl;

                    for (int a = 0; a < rows; a++){
                        for (int b = 0; b < cols; b++){
                            if(angles_magni.second[a][b] > maxMag)
                                maxMag = angles_magni.second[a][b];
                            if(angles_magni.second[a][b] > 0){
                                //std::cout<<" "<<angles_magni.second[a][b] ;
                                circle(mag, cv::Point2f(a,b), 1, cv::Scalar(0, 50, 0), 3, 8, 0);
                            }
                            if(angles_magni.second[a][b] > 2){
                                //std::cout<<" "<<angles_magni.second[a][b] ;
                                circle(mag, cv::Point2f(a,b), 1, cv::Scalar(0, 100, 0), 3, 8, 0);
                            }
                            if(angles_magni.second[a][b] > 4){
                                //std::cout<<" "<<angles_magni.second[a][b] ;
                                circle(mag, cv::Point2f(a,b), 1, cv::Scalar(0, 150, 0), 3, 8, 0);
                            }
                            if(angles_magni.second[a][b] > 6){
                                //std::cout<<" "<<angles_magni.second[a][b] ;
                                circle(mag, cv::Point2f(a,b), 1, cv::Scalar(0, 200, 0), 3, 8, 0);
                            }
                            if(angles_magni.second[a][b] > 8){
                                //std::cout<<" "<<angles_magni.second[a][b] ;
                                circle(mag, cv::Point2f(a,b), 1, cv::Scalar(0, 250, 0), 3, 8, 0);
                            }                               
                        }
                        //std::cout<<std::endl;
                    }  
                    std::cout<<"MaxMag: " <<maxMag <<std::endl;                  
                    
                    cv::namedWindow("first", cv::WINDOW_NORMAL);
                    imshow("first", mag);

                    cv::namedWindow("second", cv::WINDOW_NORMAL);
                    imshow("second", ang);

                    if (cv::waitKey() == 27)
                        cv::waitKey(20);
                }
                else
                {
                    break;
                }
            }
        }
    }

    void allocateMapToOpticalFlowsMatrix()
    {
        this->mapToOpticalFlows = new int *[this->numImgs];
        for (int i = 0; i < this->numImgs; i++)
        {
            this->mapToOpticalFlows[i] = new int[this->numImgs];
            memset(this->mapToOpticalFlows[i], 0, sizeof(int) * this->numImgs);
        }
    }

    void FillPoints(std::vector<cv::Point2f> &vecPoints, cv::Mat frameB, cv::Mat frameA, int thr)
    {
        cv::Mat test = cv::Mat::zeros(frameA.rows, frameB.cols, CV_8UC3);
        cv::Point2f a;
        vecPoints.clear();
        cvtColor(frameB, frameB, cv::COLOR_BGR2GRAY);
        cvtColor(frameA, frameA, cv::COLOR_BGR2GRAY);

        cv::Mat frameDif = cv::abs(frameB - frameA);
        for (int i = 0; i < frameDif.rows; ++i)
        {
            for (int j = 0; j < frameDif.cols; ++j)
            {
                if (frameDif.at<uchar>(i, j) > thr)
                {
                    a = cv::Point2f(static_cast<float>(j), static_cast<float>(i));
                    vecPoints.push_back(a);
                    circle(test, a, 1, cv::Scalar(255, 0, 0), 6, 8, 0);
                }
            }
        }

        cv::namedWindow("FillPoints", cv::WINDOW_NORMAL);
        imshow("FillPoints", test);

        /* if (cv::waitKey() == 27)
            cv::waitKey(100); */
    }

    void VecDesp2Mat(vector<cv::Point2f> &vecPoints, vector<cv::Point2f> &positions, ParMat &AMmat)
    {
        int x, y, valAngle, valMagnitude;
        float magnitude, angle, catetoOposto, catetoAdjacente;

        for (int i = 0; i < static_cast<int>(positions.size()); ++i)
        {
            catetoOposto = vecPoints[i].y - positions[i].y;
            catetoAdjacente = vecPoints[i].x - positions[i].x;

            magnitude = sqrt((catetoAdjacente * catetoAdjacente) + (catetoOposto * catetoOposto));

            angle = static_cast<float>(atan2f(catetoOposto, catetoAdjacente) * 180 / CV_PI);
            if (angle < 0)
                angle += 360;

            valAngle = static_cast<float>(floor(angle / (this->maxAngle / this->nBinsAngle)));

            if (logQuantization == 1)
                valMagnitude = static_cast<int>(floor(log2(magnitude)));
            else
                valMagnitude = static_cast<int>(floor(magnitude / (this->maxMagnitude / this->nBinsMagnitude)));

            if (valMagnitude < 0)
                valMagnitude = 0;

            if (valMagnitude >= this->nBinsMagnitude)
                valMagnitude = this->nBinsMagnitude - 1;

            y = static_cast<int>(positions[i].y);
            x = static_cast<int>(positions[i].x);

            AMmat.first(y, x) = valAngle;
            AMmat.second(y, x) = valMagnitude;
        }
    }

    void setData(const vector<cv::Mat> &imgs)
    {
        for (auto &img : imgs)
        {
            /* cv::namedWindow("Img", cv::WINDOW_NORMAL);
            imshow("Img", img);
            //if (cv::waitKey() == 27)
            cv::waitKey(20); */

            mImages.push_back(img.clone());
        }
        preProcess();
        mIsPrepared = true;
    }

    void preProcess()
    {
        if (mImages.empty())
        {
            throw invalid_argument("Images must be set before");
        }
        setParameters();
        setOpticalFlowData();
        //extractFeatures();
    }

    void extractFeatures(const Cube &cuboid, cv::Mat &output)
    {
        bool hasMovement = !movementFilter; // f. to eliminate pacthes w/out movement
        std::deque<ParMat> patches;
        patches = CreatePatch(cuboid, hasMovement);
        output.release();

        // verify sif there is any movement
        if (hasMovement)
        {
            for (int i = 0, k = 0; i < patches.size(); i++)
            {
                std::vector<cv::Mat> mMagnitude, mAngles;

                comMagnitude->GetAllMatrices(cv::Rect(0, 0, cuboid.w, cuboid.h), patches[i].second, mMagnitude);
                comAngles->GetAllMatrices(cv::Rect(0, 0, cuboid.w, cuboid.h), patches[i].first, mAngles);

                /* switch (this->extractionType)
                {
                case ExtractionType::HaralickFeatures:
                    extractHaralickFeatures(mMagnitude, mAngles, output);
                    break;
                case ExtractionType::VectorizedMatrices:
                    extractVectorizedMatrices(mMagnitude, mAngles, output);
                    break;
                case ExtractionType::ConcatVectorizedHaralick:
                    extractConcatVectorizedHaralick(mMagnitude, mAngles, output);
                    break;
                default: */
                extractHaralickFeatures(mMagnitude, mAngles, output);
                //}
            }
            output = output.reshape(0, 1);
        }
        patches.clear();
    }

    deque<ParMat> CreatePatch(const Cube &cuboid, bool &hasMovement)
    {
        std::deque<ParMat> patches;
        cv::Mat patchAngles, patchMagni;
        int thr = 1; // it's already quantized so zero is a movement from "bin 0"

        int t1 = cuboid.l + cuboid.t - 1;

        for (size_t ts : this->temporalScales)
        {
            for (size_t i = cuboid.t; i < t1; i++)
            {
                size_t f = i + ts; // image to process with i
                // if (f >= t0)
                if (f <= t1)
                {
                    int optFlowPos = this->mapToOpticalFlows[i][f];
                    ParMat angles_magni;

                    // on "first" we have the angles
                    patchAngles = cv::Mat(this->data[optFlowPos].first, cv::Rect(cuboid.x, cuboid.y, cuboid.w, cuboid.h));
                    // on "second" we have the magnitudes
                    patchMagni = cv::Mat(this->data[optFlowPos].second, cv::Rect(cuboid.x, cuboid.y, cuboid.w, cuboid.h));

                    angles_magni.first = patchAngles.clone();
                    angles_magni.second = patchMagni.clone();

                    patches.push_back(angles_magni);

                    // if movement was not detected yet
                    if (!hasMovement)
                    {
                        for (int j = 0; j < patchMagni.rows; ++j)
                            for (int k = 0; k < patchMagni.cols; ++k)
                                if (patchMagni.at<int>(j, k) >= thr)
                                {
                                    hasMovement = true;
                                    k = patchMagni.cols;
                                    j = patchMagni.rows;
                                }
                    }
                }
                else
                {
                    break;
                }
            }
        }

        return patches;
    }

    void extractHaralickFeatures(std::vector<cv::Mat> &mMagnitude, std::vector<cv::Mat> &mAngles, cv::Mat &output)
    {
        for (cv::Mat &degreeMat : mMagnitude)
            output.push_back(Haralick::compute(degreeMat));

        for (cv::Mat &degreeMat : mAngles)
            output.push_back(Haralick::compute(degreeMat));
    }
};

#endif
