#include <opencv2/opencv.hpp>

#include "ofcm_features.hpp"

using namespace ssig;
using namespace std;


void printMatrix(const cv::Mat & mat, int prec=10)
{
    for(int i=0; i<mat.size().height; i++)
    {
        for(int j=0; j<mat.size().width; j++)
        {
            cout << setprecision(prec) << mat.at<float>(i,j);
            if(j != mat.size().width-1)
                cout << ",";
            else
                cout << ",";
        }
    }
}


int main(int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "Invalid params" << endl;
        return 0;
    }
    string path(argv[1]);
    string category(argv[2]);
    string name = path.substr(37);

    //////// --> Begin descriptor parameters
    int nBinsMagnitude = 4;
    int nBinsAngle = 8;
    int distanceMagnitude = 1;
    int distanceAngle = 3;
    int cuboidLength = 6;
    float maxMagnitude = 20;
    int logQuantization = 1;
    bool movementFilter = true;
    vector<int> tempScales;
    tempScales.push_back(1);

    OFCM * desc = new OFCM();
    desc->setnBinsMagnitude(nBinsMagnitude);
    desc->setnBinsAngle(nBinsAngle);
    desc->setDistanceMagnitude(distanceMagnitude);
    desc->setDistanceAngle(distanceAngle);
    desc->setCuboidLength(cuboidLength);
    desc->setMaxMagnitude(maxMagnitude);
    desc->setLogQuantization(logQuantization);
    desc->setMovementFilter(movementFilter);
    desc->setTemporalScales(tempScales);
    //////// <-- End descriptor parameters

    //////// --> Begin Video Parameters
    int64 videoLength, videoWidth, videoHeight;
    cv::VideoCapture capture;
    cv::Mat image, output, loadedFeatures;
    cv::FileStorage storageMatrix;
    vector<Cube> cuboids;
    vector<cv::Mat> video;
    int step = 10;
    int sampleX, sampleY, sampleL, strideX, strideY, strideL;
    //////// <-- End Video Parameters

    capture.open(path);

    if (!capture.isOpened()) {
        cerr << "Error processing file. Can't read video " << path;
    }

    videoLength = static_cast<int64>(capture.get(CV_CAP_PROP_FRAME_COUNT));
    videoWidth = static_cast<int64>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
    videoHeight = static_cast<int64>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

    for (int64 frameStep = 0; frameStep < videoLength; frameStep += step) {
        capture.set(CV_CAP_PROP_POS_FRAMES, frameStep);
        capture.read((image));
        if (image.empty()) {
            std::cerr << "Error processing file. Can't read frame " << frameStep << "from video %s" << path;
        }

        video.push_back(image.clone());
    }

    sampleX = 24; sampleY = 24; sampleL = 6;
    strideX = 12; strideY = 12; strideL = 3;

    for (int t = 0; t <= static_cast<int>(0 + video.size() - sampleL); t += strideL) {
        for (int y = 0; y <= static_cast<int>(0 + videoHeight - sampleY); y += strideY) {
            for (int x = 0; x <= static_cast<int>(0 + videoWidth - sampleX); x += strideX) {
                cuboids.push_back(Cube(x, y, t, sampleX, sampleY, sampleL));
            }
        }
    }

    desc->setData(video);
    desc->extract(cuboids, output);
    cout << category << "|" << name << "|" << output.size().height << "," << output.size().width << "|";
    printMatrix(output, 8);
    cout << endl;
    output.release();

    return 0;
}
