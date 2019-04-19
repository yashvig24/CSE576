// by Nansong Yi 
#include <iostream>
#include "math.h"


class ImagePoints {
    public:
        int size;
        double** image;
        int* belongto;
    public:
        ImagePoints(double** image, int size) {
            this->image = image;
            this->size = size;
            this->belongto = new int [size];
        }
};

class KmeanCluster {
    public:
        int num_clusters;
        int img_size;
        double** image;
        double** centers;
        double** sumValue; 
        int* numOfPixel; 
    public: 
        KmeanCluster(double** image, int img_size, int num_clusters);
        void RandInit();
        void PixelInit();
        double L1Dist(double* p1, double* p2);
        void UpdateCenter();
        void ResetSum();
        bool IsBad();
};