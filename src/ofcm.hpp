#ifndef OFCM_H
#define OFCM_H

#include "common.h"
#include "cooccurrenceMatrix.hpp"
#include "cuboid.hpp"
#include "haralick.hpp"
#include "showmat.hpp"

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
        int nBinsMagnitude
        , int nBinsAngle
        , int distanceMagnitude
        , int distanceAngle
        , int cuboidLength
        , float maxMagnitude
        , int logQuantization
    )
    {
        defaults();
        this->nBinsMagnitude = nBinsMagnitude;
        this->nBinsAngle = nBinsAngle;
        this->distanceMagnitude = distanceMagnitude;
        this->distanceAngle = distanceAngle;
        this->cuboidLength = cuboidLength;

        if (logQuantization == 1) {
            this->logQuantization = logQuantization;

            // ceil(log(2; image[0].width ** 2 * image[0].height ** 2))
            this->maxMagnitude = 15;
        } else {
            this->maxMagnitude = maxMagnitude;
        }
        this->temporalScales.push_back(1);
        // }
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

    void setData(const vector<cv::Mat> &imgs)
    {
        for (auto &img : imgs)
        {
            showMat(img, "InputVideo", false, false);
            mImages.push_back(img.clone());            
        }
        // Todo setup maxMagnitude:
        // ceil(log(2; image[0].width ** 2 * image[0].height ** 2))
        // this->maxMagnitude = 15;
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
        // setOpticalFlowData();
        //extractFeatures();
    }

    void extract(const vector<Cube>& cuboids, Mat& output, vector<Mat *> matMagnitude, vector<Mat *> matAngle) {
        output.create(cuboids.size(), this->descriptorLength, CV_32F);

        int imagesSize = mImages.size();
        

        for (auto & cuboid: cuboids) {
            Mat features;
            // Cube masterCube (0, 0, 0, mImages[0].cols, mImages[0].rows, imagesSize);
            // Cube cutCube = masterCube & cuboid;

            // Technically the `cuboid` should be a valid cuboid which is inside the masterCube.

            extractFeatures(cuboid, features, matMagnitude, matAngle);
        } 
    }

    void extractFeatures(const Cube &cuboid, Mat &output, vector<Mat *> matMagnitude, vector<Mat *> matAngle)
    {
        std::deque<ParMat> patches;
        patches = CreatePatch(cuboid, matMagnitude, matAngle);
        output.release();

        for (int i = 0, k = 0; i < patches.size(); i++) {
            std::vector<cv::Mat> mMagnitude, mAngles;

            comAngles->GetAllMatrices(Rect(0, 0, cuboid.w, cuboid.h), patches[i].first, mAngles);
            comMagnitude->GetAllMatrices(Rect(0, 0, cuboid.w, cuboid.h), patches[i].second, mMagnitude);

            //extractHaralickFeatures(mMagnitude, mAngles, output);
        }
        //output = output.reshape(0, 1);


        patches.clear();
    }

    deque<ParMat> CreatePatch(const Cube &cuboid, vector<Mat *> matMagnitude, vector<Mat *> matAngle)
    {
        //std::cout<<"magSize: " << matMagnitude.size()<<" - angSize: "<<matAngle.size()<<std::endl;
        std::deque<ParMat> patches;
        cv::Mat patchAngles, patchMagni;
        int thr = 1; // it's already quantized so zero is a movement from "bin 0"

        int t1 = cuboid.l + cuboid.t - 1;

        for (int i = cuboid.t; i < t1; i++) {
            int next = i + 1; // image to process with i
            if (next <= t1) {
                //int optFlowPos = this->mapToOpticalFlows[i][next];
                // AQUI ESTA EL ERROR!!! 
                if(matAngle[cuboid.t/5] != NULL){                    
                    ParMat angles_magni;                
                    patchAngles = Mat(*matAngle[cuboid.t/5], cv::Rect(cuboid.x, cuboid.y, cuboid.w, cuboid.h));
                    patchMagni = Mat(*matMagnitude[cuboid.t/5], cv::Rect(cuboid.x, cuboid.y, cuboid.w, cuboid.h));

                    angles_magni.first = patchAngles.clone();
                    angles_magni.second = patchMagni.clone();

                    patches.push_back(angles_magni);    
                }
                else{
                    //patches.push_back(NULL);    
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
