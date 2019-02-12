#include "common.h"
#include <opencv2/opencv.hpp>

using namespace std;

class VideoReading {
    int videoLength;
    int videoWidth;
    int videoHeight;

    vector<cv::Mat> video;

public:
    VideoReading(string & filename) {
        cv::VideoCapture capture;
        capture.open(filename);
        if (!capture.isOpened()) {
            cerr << "Error processing file. Can't read video " << filename;
        }
        videoLength = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_COUNT));
        videoWidth = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
        videoHeight = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

        int step = 5;
        cv::Mat image;

        for (int i = 0; i < videoLength; i += step) {
            capture.set(CV_CAP_PROP_POS_FRAMES, i);
            capture.read((image));
            if (image.empty()) {
                std::cerr << "Error processing file. Can't read frame " << i << "from video %s" << filename;
            }
            video.push_back(image.clone());
        }

        cout << "videoLength: " << videoLength << endl;
        cout << "videoWidth: " << videoWidth << endl;
        cout << "videoHeight: " << videoHeight << endl;
    }

};
