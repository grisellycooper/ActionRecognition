#include "src/cube.hpp"
#include "src/videoreading.hpp"

using namespace std;

int main() {
    string filename = "/home/josue/Documents/videos/cam1/anillos.mp4";
    VideoReading * vr = new VideoReading(filename);

    return 0;
}
