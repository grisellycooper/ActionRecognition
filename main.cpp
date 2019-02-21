#include "src/videoreading.hpp"
#include "src/opticalflow.hpp"
#include "src/ofcm.hpp"
#include "src/haralick.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Invalid video file path" << endl;
        return 1;
    }

    string person(argv[1]);
    string video(argv[2]);
    string filename = "Video/boxing/person" + person + "_boxing_d" + video + "_uncomp.avi";
    std::cout << filename ;
    VideoReading *vr = new VideoReading(filename);


    int nBinsMagnitude = 4;
    int nBinsAngle = 6;
    int distanceMagnitude = 2;
    int distanceAngle = 3;
    int cuboidLength = 5;
    float maxMagnitude = 20;
    int logQuantization = 0;

    vector<int> tempScales;
    tempScales.push_back(1);
    tempScales.push_back(2);

    opticalflow oflow;
    oflow.getOpticalFlowCuboid(vr->video, vr->matMagnitude, vr->matOrientation);


    OFCM *ofcm = new OFCM(
        nBinsMagnitude, nBinsAngle, distanceMagnitude, distanceAngle, cuboidLength, maxMagnitude, logQuantization);

    Mat output;
    ofcm->setData(vr->video);
    ofcm->extract(vr->cuboids, output, vr->matMagnitude, vr->matOrientation, filename );
    cout << endl;
    return 0;
}
