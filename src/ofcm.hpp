#ifndef OFCM_H
#define OFCM_H

#include "common.h"
#include "cooccurrenceMatrix.hpp"
#include "cuboid.hpp"
#include "haralick.hpp"
#include "showmat.hpp"
#include <iostream>
#include <fstream>

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

    int count = 0;
    void defaults()
    {
        this->mIsPrepared = false;
        this->maxAngle = 361;
        comMagnitude = NULL;
        comAngles = NULL;
    }

  public:
    OFCM(
        int nBinsMagnitude, int nBinsAngle, int distanceMagnitude, int distanceAngle, int cuboidLength, float maxMagnitude, int logQuantization)
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

            // ceil(log(2; image[0].width ** 2 * image[0].height ** 2))
            this->maxMagnitude = 15;
        }
        else
        {
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
        this->descriptorLength = ((4 * 15) * 2) * this->numOpticalFlow;
        //this->descriptorLength = 12;
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

    void extract(const vector<Cube> &cuboids,
                 Mat &output,
                 vector<Mat> &matMagnitude,
                 vector<Mat> &matAngle,
                 string videoname,
                 bool flagEnd)
    {

        std::cout << "descriptorLength: " << this->descriptorLength << std::endl;
        output.create(cuboids.size(), this->descriptorLength, CV_32F);
        int idCuboid = 0;
        int imagesSize = mImages.size();
        /* For each cuboid, we compute 2 * (4 * 12 * n-1) matrices
         * 2 -> because magnitude and orientation
         * 4 -> because 0°, 45°, 90°, 135°
         * 12 -> because we choose 12 haralik features
         * n-1 -> because the cuboid length
         * This matrix is stored in features Matrix
         */

        vector<Mat> tempFeaturesCuboid;

        for (auto &cuboid : cuboids) {
            Mat features;
            // Cube masterCube (0, 0, 0, mImages[0].cols, mImages[0].rows, imagesSize);
            // Cube cutCube = masterCube & cuboid;

        //     // Technically the `cuboid` should be a valid cuboid which is inside the masterCube.
        //     //std::cout << "---------------------- CUBOID: " << idCuboid++ << std::endl;
            extractFeatures(cuboid, features, matMagnitude, matAngle);

        //     //std::cout << "Features size: " << features.size().width << " x " << features.size().height << std::endl;
            features.release();
        //     tempFeaturesCuboid.push_back(features.clone());
        }
        for (int i = 0; i < matMagnitude.size(); i++) {
            matMagnitude[i].release();
            matAngle[i].release();
        }

        // writeFeatures(tempFeaturesCuboid, videoname, flagEnd);

    }

    void extractFeatures(const Cube &cuboid, Mat &output, vector<Mat> &matMagnitude, vector<Mat> &matAngle)
    {
        //std::vector<ParMat> patches;
        vector<Mat> miniMagnitude, miniAngle;
        CreatePatch(cuboid, matMagnitude, matAngle, miniMagnitude, miniAngle);
        output.release();
        //Mat mee = Mat::zeros(50, 50, CV_16SC1);

        //std::cout<<"PatchSize: " << miniMagnitude.size() << " - " << miniAngle.size() << std::endl;

        for (int i = 0; i < miniMagnitude.size(); i++)
        {
            /* std::cout << "--- Patch : " << i  <<std::endl;
            showMat(mee, "mee", true, false); */
            std::vector<cv::Mat> mMagnitude, mAngles;

            comAngles->GetAllMatrices(Rect(0, 0, cuboid.w, cuboid.h), miniAngle[i], mAngles);
            comMagnitude->GetAllMatrices(Rect(0, 0, cuboid.w, cuboid.h), miniMagnitude[i], mMagnitude);

            /* std::cout << "*** Angles Matrices : " <<std::endl;
            cout<<mAngles[0] <<endl;

            std::cout << "*** Magnitud Matrices : " <<std::endl;
            cout<<mMagnitude[0] <<endl; */
            extractHaralickFeatures(mMagnitude, mAngles, output);
            //cout<<std::endl;
        }
        //output = output.reshape(0, 1);

        //patches.clear();
    }

    void CreatePatch(const Cube &cuboid, vector<Mat> &matMagnitude, vector<Mat> &matAngle, vector<Mat> &miniMag, vector<Mat> &miniAng)
    {
        // std::cout << "*********** magSize: " << matMagnitude.size() << " - angSize: " << matAngle.size() << std::endl;
        // cout << "Size: " << matMagnitude[0].rows << " x " << matMagnitude[0].cols << endl;
        // cout << "Size: " << matAngle[0].rows << " x " << matAngle[0].cols << endl;
        //std::vector<ParMat> patches;
        //cv::Mat_<int> patchAngles, patchMagni;
        int thr = 1; // it's already quantized so zero is a movement from "bin 0"

        int t1 = cuboid.l + cuboid.t - 1;

        for (int i = cuboid.t; i < t1; i++)
        {
            //std::cout << "CuboidId: "<< i << std::endl;
            //int next = i + 1; // Not needed for now
            //if (next <= t1) {
            //int optFlowPos = this->mapToOpticalFlows[i][next];

            ParMat angles_magni;
            Mat src1 = matMagnitude[i];
            Mat src2 = matAngle[i];
            Rect R(cv::Rect(cv::Point2i(cuboid.x, cuboid.y), cv::Point2i(cuboid.x + cuboid.w, cuboid.y + cuboid.h))); //Create a rect
            Mat patchMagni = src1(R);
            Mat patchAngles = src2(R);

            //cout<<"Size: " <<src1.rows << " x " <<src1.cols <<endl;

            miniMag.push_back(patchMagni);
            miniAng.push_back(patchAngles);
        }
    }

    void extractHaralickFeatures(std::vector<cv::Mat> &mMagnitude,
                                std::vector<cv::Mat> &mAngles,
                                cv::Mat &output)
    {
        for (cv::Mat &degreeMat : mMagnitude)
            output.push_back(Haralick::compute(degreeMat));

        for (cv::Mat &degreeMat : mAngles)
            output.push_back(Haralick::compute(degreeMat));
    }

    void writeFeatures(vector<Mat> features,
                         string nameVideo,
                         bool flagEnd)
    {

        // ofstream outputFile;
        ofstream fs;
        std::string fileName = "video_json.json";
        //.open(fileName
        fs.open(fileName, std::ios::app); // std::ios::out/aṕp

        fs.seekp (0, fs.end);
        if (fs.tellp() > 0)
        {
            fs << ",";
        }
        else
        {
            fs << "{ \"videos\":[";
        }


        //head json
        string head = "{ \"name\": \"" + nameVideo + "\", \"data\": [";
        fs << head;
        for (int idCub = 0; idCub < features.size(); idCub++)
        {
            string line = "";
            for (int idw = 0; idw < features[idCub].size().width; idw++)
            {
                for (int idh = 0; idh < features[idCub].size().height; idh++)
                {
                    string data = "";
                    float value = features[idCub].at<float>(idh, idw);
                    if (value == 0)
                    {
                        data = "0.0";
                    }
                    else
                    {
                        data = to_string(value);
                    }

                    line += data + ",";
                }
            }
            if (idCub + 1 == features.size())
            {
                line = "[" + line.substr(0, line.size() - 1) + "]";
                fs << line;
            }
            else
            {
                line = "[" + line.substr(0, line.size() - 1) + "],";
                fs << line;
            }
        }
        string foot = "]}";
        fs << foot;

        if(flagEnd)
        {
            fs << "]}";
        }

        fs.close();

    }
};

#endif
