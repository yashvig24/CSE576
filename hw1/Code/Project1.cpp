#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <iostream>
#include <typeinfo>       // operator typeid
#include <limits>

#include "cluster.h" // by Nansong Yi 


//using namespace std;
/***********************************************************************
  This is the only file you need to change for your assignment. The
  other files control the UI (in case you want to make changes.)
************************************************************************/

/***********************************************************************
  The first eight functions provide example code to help get you started
************************************************************************/

// Convert an image to grayscale
void MainWindow::BlackWhiteImage(QImage *image)
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

// Add random noise to the image
void MainWindow::AddNoise(QImage *image, double mag, bool colorNoise)
{
    int noiseMag = mag*2;

    for(int r=0;r<image->height();r++)
        for(int c=0;c<image->width();c++)
        {
            QRgb pixel = image->pixel(c, r);
            int red = qRed(pixel), green = qGreen(pixel), blue = qBlue(pixel);

            // If colorNoise, add color independently to each channel
            if(colorNoise)
            {
                red += rand()%noiseMag - noiseMag/2;
                green += rand()%noiseMag - noiseMag/2;
                blue += rand()%noiseMag - noiseMag/2;
            }
            // otherwise add the same amount of noise to each channel
            else
            {
                int noise = rand()%noiseMag - noiseMag/2;
                red += noise; green += noise; blue += noise;
            }
            image->setPixel(c, r, qRgb(max(0, min(255, red)), max(0, min(255, green)), max(0, min(255, blue))));
        }
}

// Downsample the image by 1/2
void MainWindow::HalfImage(QImage &image)
{
    int w = image.width();
    int h = image.height();
    QImage buffer = image.copy();

    // Reduce the image size.
    image = QImage(w/2, h/2, QImage::Format_RGB32);

    // Copy every other pixel
    for(int r=0;r<h/2;r++)
        for(int c=0;c<w/2;c++)
             image.setPixel(c, r, buffer.pixel(c*2, r*2));
}

// Round float values to the nearest integer values and make sure the value lies in the range [0,255]
QRgb restrictColor(double red, double green, double blue)
{
    int r = (int)(floor(red+0.5));
    int g = (int)(floor(green+0.5));
    int b = (int)(floor(blue+0.5));

    return qRgb(max(0, min(255, r)), max(0, min(255, g)), max(0, min(255, b)));
}

// Normalize the values of the kernel to sum-to-one
void NormalizeKernel(double *kernel, int kernelWidth, int kernelHeight)
{
    double denom = 0.000001; int i;
    for(i=0; i<kernelWidth*kernelHeight; i++)
        denom += kernel[i];
    for(i=0; i<kernelWidth*kernelHeight; i++)
        kernel[i] /= denom;
}

// Here is an example of blurring an image using a mean or box filter with the specified radius.
// This could be implemented using separable filters to make it much more efficient, but it's not done here.
// Note: This function is written using QImage form of the input image. But all other functions later use the double form
void MainWindow::MeanBlurImage(QImage *image, int radius)
{
    if(radius == 0)
        return;
    int size = 2*radius + 1; // This is the size of the kernel

    // Note: You can access the width and height using 'imageWidth' and 'imageHeight' respectively in the functions you write
    int w = image->width();
    int h = image->height();

    // Create a buffer image so we're not reading and writing to the same image during filtering.
    // This creates an image of size (w + 2*radius, h + 2*radius) with black borders (zero-padding)
    QImage buffer = image->copy(-radius, -radius, w + 2*radius, h + 2*radius);

    // Compute the kernel to convolve with the image
    double *kernel = new double [size*size];

    for(int i=0;i<size*size;i++)
        kernel[i] = 1.0;

    // Make sure kernel sums to 1
    NormalizeKernel(kernel, size, size);

    // For each pixel in the image...
    for(int r=0;r<h;r++)
    {
        for(int c=0;c<w;c++)
        {
            double rgb[3];
            rgb[0] = rgb[1] = rgb[2] = 0.0;

            // Convolve the kernel at each pixel
            for(int rd=-radius;rd<=radius;rd++)
                for(int cd=-radius;cd<=radius;cd++)
                {
                     // Get the pixel value
                     //For the functions you write, check the ConvertQImage2Double function to see how to get the pixel value
                     QRgb pixel = buffer.pixel(c + cd + radius, r + rd + radius);

                     // Get the value of the kernel
                     double weight = kernel[(rd + radius)*size + cd + radius];

                     rgb[0] += weight*(double) qRed(pixel);
                     rgb[1] += weight*(double) qGreen(pixel);
                     rgb[2] += weight*(double) qBlue(pixel);
                }
            // Store the pixel in the image to be returned
            // You need to store the RGB values in the double form of the image
            image->setPixel(c, r, restrictColor(rgb[0],rgb[1],rgb[2]));
        }
    }
    // Clean up (use this carefully)
    delete[] kernel;
}

// Convert QImage to a matrix of size (imageWidth*imageHeight)*3 having double values
void MainWindow::ConvertQImage2Double(QImage image)
{
    // Global variables to access image width and height
    imageWidth = image.width();
    imageHeight = image.height();

    // Initialize the global matrix holding the image
    // This is how you will be creating a copy of the original image inside a function
    // Note: 'Image' is of type 'double**' and is declared in the header file (hence global variable)
    // So, when you create a copy (say buffer), write "double** buffer = new double ....."
    Image = new double* [imageWidth*imageHeight];
    for (int i = 0; i < imageWidth*imageHeight; i++)
            Image[i] = new double[3];

    // For each pixel
    for (int r = 0; r < imageHeight; r++)
        for (int c = 0; c < imageWidth; c++)
        {
            // Get a pixel from the QImage form of the image
            QRgb pixel = image.pixel(c,r);

            // Assign the red, green and blue channel values to the 0, 1 and 2 channels of the double form of the image respectively
            Image[r*imageWidth+c][0] = (double) qRed(pixel);
            Image[r*imageWidth+c][1] = (double) qGreen(pixel);
            Image[r*imageWidth+c][2] = (double) qBlue(pixel);
        }
}

// Convert the matrix form of the image back to QImage for display
void MainWindow::ConvertDouble2QImage(QImage *image)
{
    for (int r = 0; r < imageHeight; r++)
        for (int c = 0; c < imageWidth; c++) {
            // std::cout << "height:" << imageHeight;
            // std::cout << "width:" << imageWidth;
            image->setPixel(c, r, restrictColor(Image[r*imageWidth+c][0], Image[r*imageWidth+c][1], Image[r*imageWidth+c][2]));
        }
}


/**************************************************
 TIME TO WRITE CODE
**************************************************/

/**************************************************
 TASK 1
**************************************************/

// Convolve the image with the kernel

void MainWindow::Convolution(double** image, double *kernel, int kernelWidth, int kernelHeight, bool add)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * kernel: 1-D array of kernel values
 * kernelWidth: width of the kernel
 * kernelHeight: height of the kernel
 * add: a boolean variable (taking values true or false)
*/
{   
//    std::cout << "hi" << std::endl;
    // padding mode -> mode=0: zero-padding, mode=1: fixed padding.
    if (kernelWidth%2==0 || kernelHeight%2==0) {
        std::cout << "kernel not odd" << std::endl;
        return;
    }
    double** image_pad = PaddingImage(image, kernelWidth, kernelHeight, 1);
    // std::cout << "hi" << std::endl;
    int khh = (kernelHeight / 2); //kernelHalfHeight
    int khw = (kernelWidth / 2); //kernelHalfWidth
    int new_width = imageWidth + 2 * khw;
    int new_height = imageHeight + 2 * khh;

    for (int r = 0; r < imageHeight; r++) {
        for (int c = 0; c < imageWidth; c++) {
            
            double rgb[3];
            rgb[0] = rgb[1] = rgb[2] = 0.0;

            for(int rd = -khh; rd <= khh; rd++) {
                int i_pad = r + rd + khh;
                for(int cd = -khw; cd <= khw; cd++) { 
                  
                    int j_pad = c + cd + khw;
                    double* pixel= image_pad[i_pad * new_width + j_pad];
                    // Get the value of the kernel
                    double weight = kernel[(rd + khh)*kernelWidth + cd + khw];
                    rgb[0] += weight * pixel[0];
                    rgb[1] += weight * pixel[1];
                    rgb[2] += weight * pixel[2];
                }
            }
            if (add) 
                for (int k = 0; k < 3; k++)
                    image[r*imageWidth + c][k] = rgb[k] + 128;
            else
                for (int k = 0; k < 3; k++)
                    image[r*imageWidth + c][k] = rgb[k];   
        }
    }
}

/**************************************************
 TASK 2
**************************************************/

// Apply the 2-D Gaussian kernel on an image to blur it
void MainWindow::GaussianBlurImage(double** image, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{   
    if (sigma <= 0) {
        return;
    }
    int radius = 3 * (int) sigma;
    int size = 2*radius + 1; // This is the size of the kernel
    double *kernel = new double [size*size];
    for(int rd=-radius;rd<=radius;rd++) {
        for(int cd=-radius;cd<=radius;cd++) {
            kernel[(rd + radius)*size + cd + radius] = exp(-(rd*rd + cd*cd ) / (2*sigma*sigma)) / (2*M_PI*sigma*sigma);
        }        
    }
    NormalizeKernel(kernel, size, size);
    Convolution(image, kernel, size, size, false);
    delete[] kernel;
}

/**************************************************
 TASK 3
**************************************************/

// Perform the Gaussian Blur first in the horizontal direction and then in the vertical direction
void MainWindow::SeparableGaussianBlurImage(double** image, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0) {
        return;
    }
    int radius = 3 * (int) sigma;
    int size = 2*radius + 1; // This is the size of the kernel
    double *kernel = new double [size];
    for(int cd=-radius; cd<=radius; cd++) {
        kernel[cd + radius] = exp(-cd*cd / (2*sigma*sigma)) / (2*M_PI*sigma*sigma);
    }
    NormalizeKernel(kernel, size, 1);
    Convolution(image, kernel, size, 1, false);
    Convolution(image, kernel, 1, size, false);
    delete[] kernel;
}

/********** TASK 4 (a) **********/

// Compute the First derivative of an image along the horizontal direction and then apply Gaussian blur.
void MainWindow::FirstDerivImage_x(double** image, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0) {
        return;
    }
    double kernel[3] = {-1.0, 0.0, 1.0};
    Convolution(image, kernel, 3, 1, true);
    GaussianBlurImage(image, sigma);
    // delete[] kernel; // why this doesn't work?
}

/********** TASK 4 (b) **********/

// Compute the First derivative of an image along the vertical direction and then apply Gaussian blur.
void MainWindow::FirstDerivImage_y(double** image, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0) {
        return;
    }
    double kernel[3] = {-1.0, 0.0, 1.0};
    Convolution(image, kernel, 1, 3, true);
    GaussianBlurImage(image, sigma); 
}

/********** TASK 4 (c) **********/

// Compute the Second derivative of an image using the Laplacian operator and then apply Gaussian blur
void MainWindow::SecondDerivImage(double** image, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0)
        return;
    double kernel[9] = 
    {
        0.0, 1.0, 0.0,
        1.0, -4.0, 1.0,
        0.0, 1.0, 0.0  
    };
    Convolution(image, kernel, 3, 3, true);
    GaussianBlurImage(image, sigma); 
}

/**************************************************
 TASK 5
**************************************************/

// Sharpen an image by subtracting the image's second derivative from the original image
void MainWindow::SharpenImage(double** image, double sigma, double alpha)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
 * alpha: constant by which the second derivative image is to be multiplied to before subtracting it from the original image
*/
{   
    double** buffer = ImageCopy(image);
    SecondDerivImage(buffer, sigma);
    for (int i = 0; i < imageWidth*imageHeight; i++) {
        for (int k = 0; k < 3; k++)
            image[i][k] = image[i][k] - alpha * (buffer[i][k] - 128);
    } 
}

/**************************************************
 TASK 6
**************************************************/

// Display the magnitude and orientation of the edges in an image using the Sobel operator in both X and Y directions
void MainWindow::SobelImage(double** image)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * NOTE: image is grayscale here, i.e., all 3 channels have the same value which is the grayscale value
*/
{
    // Add your code here

    // Use the following 3 lines of code to set the image pixel values after computing magnitude and orientation
    // Here 'mag' is the magnitude and 'orien' is the orientation angle in radians to be computed using atan2 function
    // (sin(orien) + 1)/2 converts the sine value to the range [0,1]. Similarly for cosine.
    double gx[9] = 
    {
        -1.0, 0.0, 1.0,
        -2.0, 0.0, 2.0,
        -1.0, 0.0, 1.0 
    }; 
    double gy[9] = 
    {
        -1.0, -2.0, -1.0,
        0.0, 0.0, 0.0,
        1.0, 2.0, 1.0 
    }; 
    double** Mx = ImageCopy(image);
    double** My = ImageCopy(image);
    Convolution(Mx, gx, 3, 3, false);
    Convolution(My, gy, 3, 3, false);
    double mag = 0;
    double orien = 0;    
    for (int i = 0; i < imageWidth*imageHeight; i++) {
        double magX = (0.3*Mx[i][0] + 0.6*Mx[i][1] + 0.1*Mx[i][2])/8;
        double magY = (0.3*My[i][0] + 0.6*My[i][1] + 0.1*My[i][2])/8;
        mag = sqrt(magX*magX + magY*magY);
        orien = atan2(magY, magX);
        image[i][0] = mag*4.0*((sin(orien) + 1.0)/2.0);
        image[i][1] = mag*4.0*((cos(orien) + 1.0)/2.0);
        image[i][2] = mag*4.0 - image[i][0] - image[i][1];        
    }
}

/**************************************************
 TASK 7
**************************************************/

// Compute the RGB values at a given point in an image using bilinear interpolation.
void MainWindow::BilinearInterpolation(double** image, double x_s, double y_s, double rgb[3])
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * x: x-coordinate (corresponding to columns) of the position whose RGB values are to be found
 * y: y-coordinate (corresponding to rows) of the position whose RGB values are to be found
 * rgb[3]: array where the computed RGB values are to be stored
*/
{
    double x = y_s;
    double y = x_s;
    int x1 = static_cast<int>(floor(x));
    int y1 = static_cast<int>(floor(y));
    int x2 = static_cast<int>(ceil(x+0.0000001));
    int y2 = static_cast<int>(ceil(y+0.0000001));

    double* Pixel_11 = getPixel(image, x1, y1);
    double* Pixel_12 = getPixel(image, x1, y2);
    double* Pixel_21 = getPixel(image, x2, y1);
    double* Pixel_22 = getPixel(image, x2, y2);

    for (int i=0; i<3; i++) {
        rgb[i] = (1 / ((x2-x1)*(y2-y1))) * 
                (((x2-x)*Pixel_11[i]+(x-x1)*Pixel_21[i])*(y2-y) +
                 ((x2-x)*Pixel_12[i]+(x-x1)*Pixel_22[i])*(y-y1));
    }
}

/*******************************************************************************
 Here is the code provided for rotating an image. 'orien' is in degrees.
********************************************************************************/

// Rotating an image by "orien" degrees
void MainWindow::RotateImage(double** image, double orien)

{
    double radians = -2.0*3.141*orien/360.0;

    // Make a copy of the original image and then re-initialize the original image with 0
    double** buffer = new double* [imageWidth*imageHeight];
    for (int i = 0; i < imageWidth*imageHeight; i++) {
        buffer[i] = new double [3];
        for(int j = 0; j < 3; j++)
            buffer[i][j] = image[i][j];
        image[i] = new double [3](); // re-initialize to 0
    }

    for (int r = 0; r < imageHeight; r++)
       for (int c = 0; c < imageWidth; c++) {
            // Rotate around the center of the image
            double x0 = (double) (c - imageWidth/2);
            double y0 = (double) (r - imageHeight/2);

            // Rotate using rotation matrix
            double x1 = x0*cos(radians) - y0*sin(radians);
            double y1 = x0*sin(radians) + y0*cos(radians);

            x1 += (double) (imageWidth/2);
            y1 += (double) (imageHeight/2);

            double rgb[3];
            BilinearInterpolation(buffer, x1, y1, rgb);

            // Note: "image[r*imageWidth+c] = rgb" merely copies the head pointers of the arrays, not the values
            image[r*imageWidth+c][0] = rgb[0];
            image[r*imageWidth+c][1] = rgb[1];
            image[r*imageWidth+c][2] = rgb[2];
        }
}

/**************************************************
 TASK 8
**************************************************/

// Find the peaks of the edge responses perpendicular to the edges
void MainWindow::FindPeaksImage(double** image, double thres)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * NOTE: image is grayscale here, i.e., all 3 channels have the same value which is the grayscale value
 * thres: threshold value for magnitude
*/
{
    double gx[9] = 
    {
        -1.0, 0.0, 1.0,
        -2.0, 0.0, 2.0,
        -1.0, 0.0, 1.0 
    }; 
    double gy[9] = 
    {
        -1.0, -2.0, -1.0,
        0.0, 0.0, 0.0,
        1.0, 2.0, 1.0 
    }; 
    double** Mx = ImageCopy(image);
    double** My = ImageCopy(image);
    Convolution(Mx, gx, 3, 3, false);
    Convolution(My, gy, 3, 3, false);
    double mag = 0;
    double orien = 0;    
    for (int i = 0; i < imageWidth*imageHeight; i++) {
        double magX = 0.3*Mx[i][0] + 0.6*Mx[i][1] + 0.1*Mx[i][2];
        double magY = 0.3*My[i][0] + 0.6*My[i][1] + 0.1*My[i][2];
        mag = sqrt(magX*magX + magY*magY);
        orien = atan2(magY, magX);

        int x = i%imageWidth; // column
        int y = i/imageWidth; // row
        double e0x = x + 1*cos(orien+M_PI/2.0);
        double e0y = y + 1*sin(orien+M_PI/2.0);
        double e1x = x + 1*cos(orien-M_PI/2.0);
        double e1y = y + 1*sin(orien-M_PI/2.0);
        double rgb0[3] = {0.0,0.0,0.0};
        double rgb1[3] = {0.0,0.0,0.0};
        BilinearInterpolation(image, e0x, e0y, rgb0);
        BilinearInterpolation(image, e1x, e1y, rgb1);
        double intensity0 = 0.3*rgb0[0] + 0.6*rgb0[1] + 0.1*rgb0[2];
        double intensity1 = 0.3*rgb1[0] + 0.6*rgb1[1] + 0.1*rgb1[2];

        if (mag > thres && mag > intensity0 && mag > intensity1) {
            for (int k=0; k<3; k++)
                image[i][k] = 255;
        }   
        else {
            for (int k=0; k<3; k++)
                image[i][k] = 0;
        }   
    }    
}

/**************************************************
 TASK 9 (a)
**************************************************/

// Perform K-means clustering on a color image using random seeds
void MainWindow::RandomSeedImage(double** image, int num_clusters)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * num_clusters: number of clusters into which the image is to be clustered
*/
{
    // define the parameters
    double epsilon = 30;
    int max_iter = 100;
    int img_size = imageHeight*imageWidth;
    // std::cout << "hi" << std::endl;
    ImagePoints imgpoints = ImagePoints(image, img_size);
    // std::cout << "hi" << std::endl;
    KmeanCluster kclusters = KmeanCluster(image, img_size, num_clusters);
    // std::cout << "hi" << std::endl;
    kclusters.RandInit();
    // std::cout << "hi" << std::endl;
    std::cout << kclusters.num_clusters << std::endl;

    int round = 0;
    int c_id = 0;
    double min, dist;
    while(round < max_iter) {
        // iterate over all points:
        for (int i=0; i<img_size; i++) {
            c_id = 0;
            min = kclusters.L1Dist(image[i], kclusters.centers[0]);
            for (int n=1; n < num_clusters; n++) {
                dist = kclusters.L1Dist(image[i], kclusters.centers[n]);
                if (dist < min) {
                    c_id = n;
                    min = dist;
                }
            }
            imgpoints.belongto[i] = c_id;
            // std::cout << imgpoints.belongto[i] << std::endl;
        }

        for (int i=0; i<img_size; i++) {
            c_id = imgpoints.belongto[i];
            kclusters.numOfPixel[c_id] += 1;
            for (int k=0; k<3; k++)
                kclusters.sumValue[c_id][k] += image[i][k]; 
        }
        kclusters.UpdateCenter();
        if (kclusters.IsBad()) {
            // continue;
            kclusters.RandInit();
            round = 0;
        }
        kclusters.ResetSum();
        round++;
    }

    for (int i=0; i<img_size; i++) {
        int c_id = imgpoints.belongto[i];
        for (int k=0; k<3; k++) {
            image[i][k] = kclusters.centers[c_id][k];
        }
    }    
    // Add your code here
}

/**************************************************
 TASK 9 (b)
**************************************************/

// Perform K-means clustering on a color image using seeds from the image itself
void MainWindow::PixelSeedImage(double** image, int num_clusters)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * num_clusters: number of clusters into which the image is to be clustered
*/
{
    double epsilon = 30;
    int max_iter = 100;
    int img_size = imageHeight*imageWidth;
    ImagePoints imgpoints = ImagePoints(image, img_size);
    KmeanCluster kclusters = KmeanCluster(image, img_size, num_clusters);
    kclusters.PixelInit();

    int round = 0;
    int c_id = 0;
    double min, dist;
    while(round < max_iter) {
        // iterate over all points:
        for (int i=0; i<img_size; i++) {
            c_id = 0;
            min = kclusters.L1Dist(image[i], kclusters.centers[0]);
            for (int n=1; n < num_clusters; n++) {
                dist = kclusters.L1Dist(image[i], kclusters.centers[n]);
                if (dist < min) {
                    c_id = n;
                    min = dist;
                }
            }
            imgpoints.belongto[i] = c_id;
        }

        for (int i=0; i<img_size; i++) {
            c_id = imgpoints.belongto[i];
            kclusters.numOfPixel[c_id] += 1;
            for (int k=0; k<3; k++)
                kclusters.sumValue[c_id][k] += image[i][k]; 
        }
        kclusters.UpdateCenter();
        if (kclusters.IsBad()) {
            std::cout << "my name is nansong" << std::endl;
        }
        kclusters.ResetSum();
        round++;
    }

    for (int i=0; i<img_size; i++) {
        int c_id = imgpoints.belongto[i];
        for (int k=0; k<3; k++) {
            image[i][k] = kclusters.centers[c_id][k];
        }
    }     
}


/**************************************************
 EXTRA CREDIT TASKS
**************************************************/

// Perform K-means clustering on a color image using the color histogram
void MainWindow::HistogramSeedImage(double** image, int num_clusters)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * num_clusters: number of clusters into which the image is to be clustered
*/
{
    // Add your code here
}

// Apply the median filter on a noisy image to remove the noise
void MainWindow::MedianImage(double** image, int radius)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * radius: radius of the kernel
*/
{
    // Add your code here
}

// Apply Bilater filter on an image
void MainWindow::BilateralImage(double** image, double sigmaS, double sigmaI)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigmaS: standard deviation in the spatial domain
 * sigmaI: standard deviation in the intensity/range domain
*/
{
    // Add your code here.  Should be similar to GaussianBlurImage.
}

// Perform the Hough transform
void MainWindow::HoughImage(double** image)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
*/
{
    // Add your code here
}

// Perform smart K-means clustering
void MainWindow::SmartKMeans(double** image)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
*/
{
    // Add your code here
}
