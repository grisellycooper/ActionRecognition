#ifndef COOCCURRENCE_H
#define COOCCURRENCE_H

#include "common.h"

class CoOccurrenceMatrix {
    cv::Mat_ <float> M0, M45, M90, M135;
    int nbins;
    int distance;

public:
    CoOccurrenceMatrix(int nbins, int distance) {
        this->nbins = nbins;
        this->distance = distance;

        M0.create(nbins, nbins);
        M45.create(nbins, nbins);
        M90.create(nbins, nbins);
        M135.create(nbins, nbins);
    }
};

#endif  // COOCCURRENCE_H
