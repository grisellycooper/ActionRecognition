#include "src/videoreading.hpp"
#include "src/opticalflow.hpp"
#include "src/ofcm.hpp"
#include "src/haralick.hpp"


using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Invalid video file path" << endl;
        return 1;
    }
    string filename(argv[1]);
    VideoReading * vr = new VideoReading(filename);

    int nBinsMagnitude = 4;
    int nBinsAngle = 6;
    int distanceMagnitude = 2;
    int distanceAngle = 3;
    int cuboidLength = 5;
    float maxMagnitude = 20;
    int logQuantization = 0;

    vector<int> tempScales;
    tempScales.push_back(1); tempScales.push_back(2);

    opticalflow oflow;
    oflow.getOpticalFlowCuboid(vr->video, vr->matMagnitude, vr->matOrientation); 
    
    std::cout<<"*********** magSize: " << vr->matMagnitude.size()<<" - angSize: "<< vr->matOrientation.size()<<std::endl;
    cout<<"Size: " <<(*vr->matMagnitude[0]).rows << " x " <<(*vr->matMagnitude[0]).cols <<endl;
    cout<<"Size: " <<(*vr->matOrientation[0]).rows << " x " <<(*vr->matOrientation[0]).cols <<endl;
        

    OFCM * ofcm = new OFCM(
         nBinsMagnitude
         , nBinsAngle
         , distanceMagnitude
         , distanceAngle
         , cuboidLength
         , maxMagnitude
         , logQuantization
    );
    
    Mat output;
    ofcm->setData(vr->video);
    //ofcm->extract(vr->cuboids, output, vr->matMagnitude, vr->matOrientation);
    
    return 0;
}
