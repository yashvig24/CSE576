// by Nansong Yi 
#include <iostream>
#include "math.h"
#include "cluster.h"

KmeanCluster::KmeanCluster(double** image, int img_size, int num_clusters) {
    this->img_size = img_size;
    this->num_clusters = num_clusters;
    this->image = image;
    this->centers = new double* [num_clusters];
    this->sumValue = new double* [num_clusters];
    this->numOfPixel = new int[num_clusters];
}

void KmeanCluster::RandInit() {
    for (int i = 0; i < num_clusters; i++) {
        centers[i] = new double [3];
        sumValue[i] = new double [3];
        numOfPixel[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            centers[i][j] = rand() % 256;
            sumValue[i][j] = 0;
        }
    }
}

void KmeanCluster::PixelInit() {
    for (int i = 0; i < num_clusters; i++) {
        centers[i] = new double [3];
        sumValue[i] = new double [3];
        numOfPixel[i] = 0;
        int randid = rand() % img_size;
        for (int j = 0; j < 3; j++)
        {
            centers[i][j] = image[randid][j];
            sumValue[i][j] = 0;
        }
    }
}

double KmeanCluster::L1Dist(double* p1, double* p2) {
    return (fabs(p1[0]-p2[0]) + fabs(p1[1]-p2[1]) + fabs(p1[2]-p2[2]));
}

void KmeanCluster::UpdateCenter(){
    for (int n=0; n<num_clusters; n++) {
        for (int k=0; k<3; k++) {
            centers[n][k] = sumValue[n][k]/numOfPixel[n];
        }
    }
}

void KmeanCluster::ResetSum(){
    for (int n=0; n<num_clusters; n++) {
        numOfPixel[n] = 0;
        for (int k=0; k<3; k++) {
            sumValue[n][k] = 0;
        }
    }
}

void KmeanCluster::ResetCluster(int id) {
        numOfPixel[id] = 0;
        for (int j = 0; j < 3; j++)
        {
            centers[id][j] = rand() % 256;
            sumValue[id][j] = 0;
        }
}

bool KmeanCluster::IsBad() {
    int num = 0;
    for (int n=0; n<num_clusters; n++) {
        if (numOfPixel[n]<10) {
            num++;
            ResetCluster(n);
        }
    }
    std::cout << "bad num = " << num << std::endl;
    if (num>1)
        return true;
    else
        return false;
}