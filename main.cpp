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
    bool flag = (string(argv[3]) == "true")? true: false;



    int nBinsMagnitude = 4;
    int nBinsAngle = 6;
    int distanceMagnitude = 2;
    int distanceAngle = 3;
    int cuboidLength = 5;
    float maxMagnitude = 20;
    int logQuantization = 0;

    for (int i= 1; i < 10 ; i ++)
    {
        for (int j = 1; j <= 4; j++)
        {
            ///home/josue/Documents/dataset
            string filename = "/home/josue/Documents/dataset/handclapping/person0" + to_string(i) + "_handclapping_d" + to_string(j) + "_uncomp.avi";
            std::cout << filename;
            VideoReading *vr = new VideoReading(filename);

            vector<int> tempScales;
            tempScales.push_back(1);
            tempScales.push_back(2);

            opticalflow oflow;
            oflow.getOpticalFlowCuboid(vr->video, vr->matMagnitude, vr->matOrientation);

            OFCM *ofcm = new OFCM(
                nBinsMagnitude, nBinsAngle, distanceMagnitude, distanceAngle, cuboidLength, maxMagnitude, logQuantization);

            Mat output;
            ofcm->setData(vr->video);
            ofcm->extract(vr->cuboids, output, vr->matMagnitude, vr->matOrientation, filename, flag);
            cout << endl;
        }
    }

    return 0;
}
