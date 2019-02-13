#include "common.h"
#include "cuboid.hpp"
#include <opencv2/opencv.hpp>

using namespace std;

class VideoReading {
    int videoLength;
    int videoWidth;
    int videoHeight;

    vector<cv::Mat> video;
    vector<Cube> cuboids;

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
        int M = 48;
        int N = 48;
        int T = 10;
        int m = 24;
        int n = 24;
        int t = 5;

        for (int i = 0; i < videoLength; i += step) {
            capture.set(CV_CAP_PROP_POS_FRAMES, i);
            capture.read((image));
            if (image.empty()) {
                std::cerr << "Error processing file. Can't read frame " << i << "from video %s" << filename;
            }
            video.push_back(image.clone());
        }

        for (int ti = 0; ti <= 0 + video.size() - T; ti += t) {
            for (int y = 0; y <= 0 + videoHeight - N; y += n) {
                for (int x = 0; x <= 0 + videoWidth - M; x += m) {
                    cuboids.push_back(Cube(x, y, t, M, N, T));
                }
            }
        }

        for (int i = 0; i < 100; i++) {
            cout << "cuboide: " << i << endl;
            cout << "x: " << cuboids[i].x << endl;
            cout << "y: " << cuboids[i].y << endl;
            cout << "t: " << cuboids[i].t << endl;
        }

        // cout << "cuboids size: " << cuboids.size() << endl;
    }

};
