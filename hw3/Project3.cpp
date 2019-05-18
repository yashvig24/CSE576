#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "stdlib.h"
#include <algorithm>


/**************************************************
CODE FOR K-MEANS COLOR IMAGE CLUSTERING (RANDOM SEED)
**************************************************/
// refine the random seed
QRgb getRamdomSeed(std::vector<QRgb> centers)
{

    bool flagFindSeed = false;
    double seedDist = 0;
    QRgb RGBcenter;
    // j = 0;
    while(!flagFindSeed)
    {
        flagFindSeed = true;
        RGBcenter = qRgb(rand() % 256, rand() % 256, rand() % 256);
        for (int i=0; i<centers.size(); i++)
        {
            seedDist = fabs(qRed(RGBcenter) - qRed(centers[i])) + fabs(qBlue(RGBcenter) - qBlue(centers[i])) + fabs(qGreen(RGBcenter) - qGreen(centers[i]));
            if (seedDist < 100) flagFindSeed = false;
        }
    }

    return RGBcenter;
}

void Clustering(QImage *image, int num_clusters, int maxit)
{
        int w = image->width(), h = image->height();
        QImage buffer = image->copy();

        std::vector<QRgb> centers, centers_new;
        QRgb center;

        //initialize random centers
        int n = 1;
        while (n <= num_clusters)
        {
            //QRgb center = qRgb(rand() % 256, rand() % 256, rand() % 256);
            center = getRamdomSeed(centers);
            centers.push_back(center);
            centers_new.push_back(center);
            n++;
        }

        //iterative part
        int it = 0;
        std::vector<int> ids;
        while (it < maxit)
        {
                ids.clear();
                //assign pixels to clusters
                for (int r = 0; r < h; r++)
                	for (int c = 0; c < w; c++)
                	{
                        int maxd = 999999, id = 0;
                        for (int n = 0; n < num_clusters; n++)
                        {
                                QRgb pcenter = centers[n];
                                QRgb pnow = buffer.pixel(c, r);
                                int d = abs(qRed(pcenter) - qRed(pnow)) + abs(qGreen(pcenter) - qGreen(pnow)) + abs(qBlue(pcenter) - qBlue(pnow));
                                if (d < maxd)
                                {
                                        maxd = d; id = n;
                                }
                        }
                        ids.push_back(id);
                	}

                //update centers
                std::vector<int> cnt, rs, gs, bs;
                for (int n = 0; n < num_clusters; n++)
                {
                        rs.push_back(0); gs.push_back(0); bs.push_back(0); cnt.push_back(0);
                }
                for (int r = 0; r < h; r++)
                    for (int c = 0; c < w; c++)
                    {
                        QRgb pixel = buffer.pixel(c,r);
                        rs[ids[r * w + c]] += qRed(pixel);
                        gs[ids[r * w + c]] += qGreen(pixel);
                        bs[ids[r * w + c]] += qBlue(pixel);
                        cnt[ids[r * w + c]]++;
                    }
                for (int n = 0; n < num_clusters; n++)
                    if (cnt[n] == 0) // no pixels in a cluster
                        continue;
                    else
                        centers_new[n] = qRgb(rs[n]/cnt[n], gs[n]/cnt[n], bs[n]/cnt[n]);

                centers = centers_new; it++;
        }
        //render results
        for (int r = 0; r < h; r++)
            for (int c = 0; c < w; c++)
                image->setPixel(c, r, qRgb(ids[r * w + c],ids[r * w + c],ids[r * w + c]));
}

/**************************************************
CODE FOR FINDING CONNECTED COMPONENTS
**************************************************/

#include "utils.h"

#define MAX_LABELS 80000

#define I(x,y)   (image[(y)*(width)+(x)])
#define N(x,y)   (nimage[(y)*(width)+(x)])

void uf_union( int x, int y, unsigned int parent[] )
{
    while ( parent[x] )
        x = parent[x];
    while ( parent[y] )
        y = parent[y];
    if ( x != y ) {
        if ( y < x ) parent[x] = y;
        else parent[y] = x;
    }
}

int next_label = 1;

int uf_find( int x, unsigned int parent[], unsigned int label[] )
{
    while ( parent[x] )
        x = parent[x];
    if ( label[x] == 0 )
        label[x] = next_label++;
    return label[x];
}

void conrgn(int *image, int *nimage, int width, int height)
{
    unsigned int parent[MAX_LABELS], labels[MAX_LABELS];
    int next_region = 1, k;

    memset( parent, 0, sizeof(parent) );
    memset( labels, 0, sizeof(labels) );

    for ( int y = 0; y < height; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            k = 0;
            if ( x > 0 && I(x-1,y) == I(x,y) )
                k = N(x-1,y);
            if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) < k )
                k = N(x,y-1);
            if ( k == 0 )
            {
                k = next_region; next_region++;
            }
            if ( k >= MAX_LABELS )
            {
                fprintf(stderr, "Maximum number of labels reached. Increase MAX_LABELS and recompile.\n"); exit(1);
            }
            N(x,y) = k;
            if ( x > 0 && I(x-1,y) == I(x,y) && N(x-1,y) != k )
                uf_union( k, N(x-1,y), parent );
            if ( y > 0 && I(x,y-1) == I(x,y) && N(x,y-1) != k )
                uf_union( k, N(x,y-1), parent );
        }
    }
    for ( int i = 0; i < width*height; ++i )
        if ( nimage[i] != 0 )
            nimage[i] = uf_find( nimage[i], parent, labels );

    next_label = 1; // reset its value to its initial value
    return;
}


/**************************************************
 **************************************************
TIME TO WRITE CODE
**************************************************
**************************************************/



//int dimX = 100; int dimY = 100; int dimZ = 100;
//int*** array;    // 3D array definition;
//// begin memory allocation
//array = new int**[dimX];
//for(int x = 0; x < dimX; ++x) {
//    array[x] = new int*[dimY];
//    for(int y = 0; y < dimY; ++y) {
//        array[x][y] = new int[dimZ];
//        for(int z = 0; z < dimZ; ++z) { // initialize the values to whatever you want the default to be
//            array[x][y][z] = 0;
//        }
//    }
//}

void BlackWhiteImage(QImage *image)
{
    for(int r=0;r<image->height();r++)
        for(int c=0;c<image->width();c++)
        {
            QRgb pixel = image->pixel(c, r);
            double red = (double) qRed(pixel);
            double green = (double) qGreen(pixel);
            double blue = (double) qBlue(pixel);

            // Compute intensity from colors - these are common weights
            double intensity = 0.3*red + 0.6*green + 0.1*blue;

            image->setPixel(c, r, qRgb( (int) intensity, (int) intensity, (int) intensity));
        }
}

// clean the region from *img which area< regionThre
int noiseCleaning(int regionThre, int num_regions, int w, int h, int *nimg )
{
    //std::vector<double*> regionNum;
    // count the size of each region
    int r, c;
    int* regionNum = new int[num_regions + 1];
    for (int i=0; i<num_regions + 1; i++)
        regionNum[i] =0;

    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
        {
            regionNum[nimg[r*w+c]] ++;
        }

    // the regionChangedInd rocord the index of region after thresholding [1-num_regions]
    int reginCount = 0;
    int * regionChangedInd = new int [num_regions + 1];
    for (int i=1; i< num_regions+1; i++)
    {
        if (regionNum[i] > regionThre)
        {
            reginCount++;
            regionChangedInd[i] = reginCount;

        }
        else
        {
            regionChangedInd[i] = 0;
        }
    }
    //    for (int i=0; i< num_regions+1; i++)
    //        std::cout<<i<<" -> "<<regionChangedInd[i]<<endl;
    // create the new *nimg
    int pos;
    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
        {
            pos = r*w + c;
            nimg[pos] = regionChangedInd[nimg[pos]];
        }

    delete[] regionNum;
    delete[] regionChangedInd;

    return reginCount;
}

//calculate the co-occurrence matriax of current point and then store the value to its region, this function works in the loop
void calCurrentCoMat(int ***coMat, int regionNum, int r, int c, int dr, int dc, int w, int h, QImage image)
{
    // boundry of region
    int pixelCurrent, pixelNext;

    if ((r<h-dr) && (c<w-dc))
    {
        pixelCurrent = (int) qRed(image.pixel(c, r));
        pixelNext = (int) qRed(image.pixel(c+dc, r+dr));
        coMat[regionNum][pixelCurrent][pixelNext]++;
    }

    return;

}

// normailized co-occurrence matrix
void normizeCoMat (int ***coMat, double ***normCoMat, int num_regions )
{
    int i, r, c;
    for (i=0; i<num_regions; i++)
    {
        double pixelSum = 0.0;
        for (r=0; r<256; r++)
        {
            for (c=0; c<256; c++)
            {
                pixelSum += (double) coMat[i][r][c];
            }
        }

        for (r=0; r<256; r++)
        {
            for (c=0; c<256; c++)
            {
                normCoMat[i][r][c] = (double) coMat[i][r][c]/pixelSum;
            }
        }

    }
    return;
}

// energy, entropy, contrast,
void calCoMatParameters(double ***normCoMat, int num_regions, double *energy, double *entropy, double *contrast, double *correlation)
{
    int i, r, c;
    for (i=0; i<num_regions; i++)
    {
        //double pixelSum = 0;
        for (r=0; r<256; r++)
        {
            for (c=0; c<256; c++)
            {
                energy[i] += pow(normCoMat[i][r][c], 2);
                if (normCoMat[i][r][c] > 0)
                    entropy[i] += -(normCoMat[i][r][c]*log2(normCoMat[i][r][c]));
                contrast[i] += (r - c)*(r - c)*normCoMat[i][r][c];

                correlation[i] += fabs(r - 127.5)*fabs(c-127.5)/(5482.7)*normCoMat[i][r][c];
            }
        }

    }

    return;
}


//
// centroid and boundry, works in a loop
// boundry[n_region][0] leftupper point row
// boundry[n_region][1] leftupper point column
// boundry[n_region][2] rightdown point row
// boundry[n_region][3] rightdown point column
// boundry[n_region][4] area of bound
void calCoMatCentroidAndBoundry(int r, int c, int w, int *nimg, double **centroid, double **boundry)
{
    int n_region = nimg[r*w + c] - 1;
    centroid[n_region][0] += r;  //row
    centroid[n_region][1] += c; //column
    centroid[n_region][2] ++;   //count the size of each region

    if (boundry[n_region][0] > r) boundry[n_region][0] = r;
    if (boundry[n_region][1] > c) boundry[n_region][1] = c;
    if (boundry[n_region][2] < r) boundry[n_region][2] = r;
    if (boundry[n_region][3] < c) boundry[n_region][3] = c;

    return;
}

// normalize the centroid and boundry box
void normCentroidAndBoundry(int num_regions, int w, int h, double **centroid, double **boundry)
{
    for (int i = 0; i<num_regions; i++)
    {
        centroid[i][0] /= (centroid[i][2]*h);
        centroid[i][1] /= (centroid[i][2]*w);

        boundry[i][4] = (boundry[i][2]-boundry[i][0]) * (boundry[i][3]-boundry[i][1]) /(w*h) ;

        boundry[i][0] /= h;
        boundry[i][1] /= w;
        boundry[i][2] /= h;
        boundry[i][3] /= w;

    }
}


/**************************************************
Code to compute the features of a given image (both database images and query image)
**************************************************/

std::vector<double*> MainWindow::ExtractFeatureVector(QImage image)
{
    /********** STEP 1 **********/

    // Display the start of execution of this step in the progress box of the application window
    // You can use these 2 lines to display anything you want at any point of time while debugging

    ui->progressBox->append(QString::fromStdString("Clustering.."));
    QApplication::processEvents();

    // Perform K-means color clustering
    // This time the algorithm returns the cluster id for each pixel, not the rgb values of the corresponding cluster center
    // The code for random seed clustering is provided. You are free to use any clustering algorithm of your choice from HW 1
    // Experiment with the num_clusters and max_iterations values to get the best result

    int num_clusters = 5;
    int max_iterations = 50;
    QImage image_copy = image;
    Clustering(&image_copy,num_clusters,max_iterations);


    /********** STEP 2 **********/


    ui->progressBox->append(QString::fromStdString("Connecting components.."));
    QApplication::processEvents();

    // Find connected components in the labeled segmented image
    // Code is given, you don't need to change

    int r, c, w = image_copy.width(), h = image_copy.height();
    int *img = (int*)malloc(w*h * sizeof(int));
    memset( img, 0, w * h * sizeof( int ) );
    int *nimg = (int*)malloc(w*h *sizeof(int));
    memset( nimg, 0, w * h * sizeof( int ) );

    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
            img[r*w + c] = qRed(image_copy.pixel(c,r));

    conrgn(img, nimg, w, h);

    int num_regions=0;
    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
            num_regions = (nimg[r*w+c]>num_regions)? nimg[r*w+c]: num_regions;

//    ui->progressBox->append(QString::fromStdString("#regions = "+std::to_string(num_regions)));
//    QApplication::processEvents();

    // The resultant image of Step 2 is 'nimg', whose values range from 1 to num_regions

    // WRITE YOUR REGION THRESHOLDING AND REFINEMENT CODE HERE

    //noise cleaning

    int regionThre = 80;

    num_regions = noiseCleaning(regionThre, num_regions, w, h, nimg);


    ui->progressBox->append(QString::fromStdString("#regions = "+std::to_string(num_regions)));
    QApplication::processEvents();

    /********** STEP 3 **********/


    ui->progressBox->append(QString::fromStdString("Extracting features.."));
    QApplication::processEvents();

    // Extract the feature vector of each region

    /*---------------------------------------------------------------------------------*/
    /*---------------------------------------------------------------------------------*/

    // pre define variables
    int*** coMat = new int** [num_regions];
    double*** normCoMat = new double** [num_regions];
    for (int i=0; i<num_regions; i++)
    {
        coMat[i] = new int*[256];
        normCoMat[i] = new double*[256];
        for (int j=0; j<256; j++)
        {
            coMat[i][j] = new int[256];
            normCoMat[i][j] = new double[256];
            for (int k=0; k<256; k++)
            {
                coMat[i][j][k] = 0;
                normCoMat[i][j][k] = 0.0;
            }
        }
    }

    // define centroid
    double **centroid = new double*[num_regions];
    for (int i=0; i<num_regions; i++)
    {
        centroid[i] = new double[3];    //[0]row [1]column [2] region size
        for (int j=0; j<3; j++)
            centroid[i][j] = 0;
    }

    // define boundry box
    double **boundry = new double*[num_regions];
    for (int i=0; i<num_regions; i++)
    {
        boundry[i] = new double[5];
        boundry[i][0] = h;
        boundry[i][1] = w;
        boundry[i][2] = 0;
        boundry[i][3] = 0;
        boundry[i][4] = 0;
    }

    // calculate the co-occurance matrix
    // calculate region centroid and boundry
    QImage image_gray = image;
    BlackWhiteImage(&image_gray);
    int dr = 1, dc = 1;
    for (r=0; r<h; r++)
        for (c=0; c<w; c++)
        {
            int indexOfRegion = nimg[r*w+c] - 1;    //
            if (indexOfRegion>=0)
            {
                calCurrentCoMat(coMat, indexOfRegion, r, c, dr, dc, w, h, image_gray);
                calCoMatCentroidAndBoundry(r, c, w, nimg, centroid, boundry);
            }
        }

    normCentroidAndBoundry(num_regions, w, h, centroid, boundry);

    // normize the co-occurance matrix
    normizeCoMat(coMat, normCoMat, num_regions);

    // pre define variables
    double* energy = new double[num_regions];
    double* entropy = new double[num_regions];
    double* contrast = new double[num_regions];
    double* correlation = new double[num_regions];

    for (int i=0; i<num_regions; i++)
    {
        energy[i] = 0.0;
        entropy[i] = 0.0;
        contrast[i] = 0.0;
        correlation[i] = 0.0;
    }

    // calculate the energy, entropy, contrast
    calCoMatParameters(normCoMat, num_regions, energy, entropy, contrast, correlation);
    /*---------------------------------------------------------------------------------*/
    /*---------------------------------------------------------------------------------*/


    // Set length of feature vector according to the number of features you plan to use.
    featurevectorlength = 15;

    // Initializations required to compute feature vector

    std::vector<double*> featurevector; // final feature vector of the image; to be returned
    double **features = new double* [num_regions]; // stores the feature vector for each connected component
    for(int i=0;i<num_regions; i++)
        features[i] = new double[featurevectorlength](); // initialize with zeros

    // Sample code for computing the mean RGB values and size of each connected component

    for(int r=0; r<h; r++)
        for (int c=0; c<w; c++)
        {
            if ((nimg[r*w+c] - 1) < 0) continue;

            features[nimg[r*w+c]-1][0] += 1; // stores the number of pixels for each connected component
            features[nimg[r*w+c]-1][1] += (double) qRed(image.pixel(c,r));
            features[nimg[r*w+c]-1][2] += (double) qGreen(image.pixel(c,r));
            features[nimg[r*w+c]-1][3] += (double) qBlue(image.pixel(c,r));
        }

    // define the weights for each feature
    double *weights = new double [featurevectorlength];
    for (int i=0; i<featurevectorlength; i++)
        weights[i] = 1.0;

    weights[4] = 50.0;
    weights[5] = 1.0/10;
    weights[6] = 1.0/4000;

    weights[9] = 0.3;
    weights[10] = 0.3;
    weights[11] = 0.3;
    weights[12] = 0.3;

    weights[14] = 0.8;

    // Save the mean RGB and size values as image feature after normalization
    for(int m=0; m<num_regions; m++)
    {
        for(int n=1; n<4; n++)
            features[m][n] /= features[m][0]*255.0;
        features[m][0] /= (double) w*h;

        // energy, entropy, contrast
        features[m][4] = energy[m];
        features[m][5] = entropy[m];
        features[m][6] = contrast[m];
        // centroid
        features[m][7] = centroid[m][0];
        features[m][8] = centroid[m][1];
       // boundry box
        for(int n=9; n<9+5; n++)
            features[m][n] = boundry[m][n-9];
        // texture correlation
        features[m][14] = correlation[m];

        // modify by weights
        for (int i=0; i<featurevectorlength; i++)
            features[m][i] *= weights[i];

        // output the result
       // for (int i=0; i<featurevectorlength; i++)
       //    std::cout<<"feature "<<i<<" = "<<features[m][i]<<endl;

        featurevector.push_back(features[m]);
    }

//    int dimX = 100; int dimY = 100; int dimZ = 100;
//    int*** array;    // 3D array definition;
//    // begin memory allocation
//    array = new int**[dimX];
//    for(int x = 0; x < dimX; ++x) {
//        array[x] = new int*[dimY];
//        for(int y = 0; y < dimY; ++y) {
//            array[x][y] = new int[dimZ];
//            for(int z = 0; z < dimZ; ++z) { // initialize the values to whatever you want the default to be
//                array[x][y][z] = 0;
//            }
//        }
//    }

    // Return the created feature vector
    ui->progressBox->append(QString::fromStdString("***Done***"));
    QApplication::processEvents();

    // free the memory
    for (int i=0; i<num_regions; i++)
    {
        for (int j=0; j<256; j++)
        {
            delete[] coMat[i][j];
            delete[] normCoMat[i][j];
        }
    }
    for (int i=0; i<num_regions; i++)
    {
        delete[] coMat[i];
        delete[] normCoMat[i];
    }
    delete[] coMat;
    delete[] normCoMat;

    for (int i=0; i<num_regions; i++)
    {
        delete[] centroid[i];
        delete[] boundry[i];
    }
    delete[] centroid;
    delete[] boundry;


    delete[] img;
    delete[] nimg;
    delete[] energy;
    delete[] entropy;
    delete []contrast;
    delete[] weights;
    //delete regionNum;
    return featurevector;
}


/***** Code to compute the distance between two images *****/

// Function that implements distance measure 1

// L1 distance
double distance1(double* vector1, double* vector2, int length)
{

    double disSum = 0.0;
    for (int i=0 ;i< length; i++)
    {
        disSum += fabs(vector1[i] - vector2[i]);
    }
    return disSum;
    // default, for trial only; change according to your distance measure
    //return ((double) rand() / (double) RAND_MAX);
}

/*
// Canberra distance
double distance1(double* vector1, double* vector2, int length)
{

    double disSum = 0.0;
    for (int i=0 ;i< length; i++)
    {
        disSum += fabs(vector1[i] - vector2[i])/(vector1[i] + vector2[i]);
    }
    return disSum;
    // default, for trial only; change according to your distance measure
    //return ((double) rand() / (double) RAND_MAX);
}
*/

// Function that implements distance measure 2
// L2
double distance2(double* vector1, double* vector2, int length)
{

    double disSum = 0.0;
    for (int i=0 ;i< length; i++)
    {
        disSum += pow((vector1[i] - vector2[i]), 2);
    }
    disSum = sqrt(disSum);
    return disSum;

    // default, for trial only; change according to your distance measure
    //return ((double) rand() / (double) RAND_MAX);
}

// Function to calculate the distance between two images
// Input argument isOne takes true for distance measure 1 and takes false for distance measure 2

void MainWindow::CalculateDistances(bool isOne)
{
    for(int n=0; n<num_images; n++) // for each image in the database
    {
        distances[n] = 0.0; // initialize to 0 the distance from query image to a database image

        for (int i = 0; i < queryfeature.size(); i++) // for each region in the query image
        {
            double current_distance = (double) RAND_MAX, new_distance;

            for (int j = 0; j < databasefeatures[n].size(); j++) // for each region in the current database image
            {
                if (isOne)
                    new_distance = distance1(queryfeature[i], databasefeatures[n][j], featurevectorlength);
                else
                    new_distance = distance2(queryfeature[i], databasefeatures[n][j], featurevectorlength);

                current_distance = std::min(current_distance, new_distance); // distance between the best matching regions
            }

            distances[n] = distances[n] + current_distance; // sum of distances between each matching pair of regions
        }

        distances[n] = distances[n] / (double) queryfeature.size(); // normalize by number of matching pairs

        // Display the distance values
        ui->progressBox->append(QString::fromStdString("Distance to image "+std::to_string(n+1)+" = "+std::to_string(distances[n])));
        QApplication::processEvents();
    }
}
