#ifndef OFCM_H
#define OFCM_H

#include "common.h"
#include "cooccurrenceMatrix.hpp"

using namespace std;

class OFCM {
    vector <cv::Mat> mImages;
    bool mIsPrepared;

    int nBinsMagnitude;
    int nBinsAngle;
    int distanceMagnitude;
    int distanceAngle;
    int cuboidLength;
    float maxMagnitude;
    int maxAngle;
    int logQuantization;

    CoOccurrenceMatrix * comMagnitude;
    CoOccurrenceMatrix * comAngles;
    vector<int> temporalScales;

    int numOpticalFlow;
    int descriptorLength;

    void defaults() {
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
        , vector<int> temporalScales
    ) {
        defaults();
        this->nBinsMagnitude = nBinsMagnitude;
        this->nBinsAngle = nBinsAngle;
        this->distanceMagnitude = distanceMagnitude;
        this->distanceAngle = distanceAngle;
        this->cuboidLength = cuboidLength;

        if (logQuantization == 1) {
            this->logQuantization = logQuantization;
            this->maxMagnitude = 15;
        } else {
            this->maxMagnitude = maxMagnitude;
        }
        for (auto v : temporalScales) {
            this->temporalScales.push_back(v);
        }

    }

    int computeOpticalFlowPerCuboid() {
        int res = 0;
        for (int i = 0; i < this->cuboidLength; i++) {
            for (int j : temporalScales) {
                int k = i + j;
                if (k < this->cuboidLength) {
                    res++;
                } else {
                    break;
                }
            }
        }
        return res;
    }

    void setParameters() {
        int tamVecMagniMatrices = 4 * (this->nBinsMagnitude * this->nBinsMagnitude);
        int tamVecAngleMatrices = 4 * (this->nBinsAngle * this->nBinsAngle);

        comMagnitude = new CoOccurrenceMatrix(
            this->nBinsMagnitude
            , this->distanceMagnitude);
        comAngles = new CoOccurrenceMatrix(
            this->nBinsAngle
            , this->distanceAngle);

        this->numOpticalFlow = computeOpticalFlowPerCuboid();
        this->descriptorLength = ((4 * 12) + (4 * 12)) * this->numOpticalFlow;
    }

    void setOpticalFlowData() {

    }

    void setData(const vector<cv::Mat> & imgs) {
        for(auto &img: imgs) {
            mImages.push_back(img.clone());
        }
        preProcess();
        mIsPrepared = true;
    }

    void preProcess() {
        if (mImages.empty()) {
            throw invalid_argument("Images must be set before");
        }
        setParameters();
        setOpticalFlowData();
    }
};

#endif
