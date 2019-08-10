#ifndef VIDEOREADING_H
#define VIDEOREADING_H


#include "common.h"
#include "cuboid.hpp"

using namespace std;
using namespace cv;

class VideoReading {

public:
    int videoLength;
    int videoWidth;
    int videoHeight;

    vector<Mat> matMagnitude;
    vector<Mat> matOrientation;

    vector<Mat> video;
    vector<Cube> cuboids;
    VideoReading(string & filename) {
        cv::VideoCapture capture;
        capture.open(filename);
        if (!capture.isOpened()) {
            cerr << "Error processing file. Can't read video " << filename;
        }
        videoLength = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_COUNT));
        videoWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
        videoHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

        std::cout<<"Video Info -----------" <<std::endl;
        std::cout<<"videoLength: " << videoLength <<std::endl;
        std::cout<<"videoWidth: " << videoWidth <<std::endl;
        std::cout<<"videoHeight: " << videoHeight <<std::endl;
        std::cout<<"-----------------------" <<std::endl;

        
        int step = 5;
        cv::Mat image;
        int M = 36;
        int N = 36;
        int T = 10;
        int m = 18; //X stride
        int n = 18; //Y stride
        int t = 5;  //frame strideCoOccurrenceMatrix

        for (int i = 0; i < videoLength; i += step) {
            capture.set(cv::CAP_PROP_POS_FRAMES, i);
            capture.read((image));
            if (image.empty()) {
                std::cerr << "Error processing file. Can't read frame " << i << "from video %s" << filename;
            }

            video.push_back(image.clone());
        }
        
        cout << "Video size: " << video.size() << endl;
        matMagnitude.resize(video.size());
        matOrientation.resize(video.size());


        for (int ti = 0; ti <= video.size() - T; ti += t) {
            for (int y = 0; y <= videoHeight - N; y += n) {
                for (int x = 0; x <= videoWidth - M; x += m) {
                    cuboids.push_back(Cube(x, y, ti, M, N, T));
                }
            }
        }
        std::cout <<"cuboids size: " << cuboids.size() <<std::endl;
    }



};

#endif // VIDEOREADING_H
