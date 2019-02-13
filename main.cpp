#include "src/videoreading.hpp"


using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Invalid video file path" << endl;
        return 1;
    }
    string filename(argv[1]);
    VideoReading * vr = new VideoReading(filename);

    return 0;
}
