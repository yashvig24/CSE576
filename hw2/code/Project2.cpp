#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "Matrix.h"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

/*******************************************************************************
    The following are helper routines with code already written.
    The routines you'll need to write for the assignment are below.
*******************************************************************************/


/*******************************************************************************
Blur a single channel floating point image with a Gaussian.
    image - input and output image
    w - image width
    h - image height
    sigma - standard deviation of Gaussian
*******************************************************************************/
void MainWindow::GaussianBlurImage(double *image, int w, int h, double sigma)
{
    int r, c, rd, cd, i;
    int radius = max(1, (int) (sigma*3.0));
    int size = 2*radius + 1;
    double *buffer = new double [w*h];

    memcpy(buffer, image, w*h*sizeof(double));

    if(sigma == 0.0)
        return;

    double *kernel = new double [size];

    for(i=0;i<size;i++)
    {
        double dist = (double) (i - radius);

        kernel[i] = exp(-(dist*dist)/(2.0*sigma*sigma));
    }

    double denom = 0.000001;

    for(i=0;i<size;i++)
        denom += kernel[i];
    for(i=0;i<size;i++)
        kernel[i] /= denom;

    for(r=0;r<h;r++)
    {
        for(c=0;c<w;c++)
        {
            double val = 0.0;
            double denom = 0.0;

            for(rd=-radius;rd<=radius;rd++)
                if(r + rd >= 0 && r + rd < h)
                {
                     double weight = kernel[rd + radius];

                     val += weight*buffer[(r + rd)*w + c];
                     denom += weight;
                }

            val /= denom;

            image[r*w + c] = val;
        }
    }

    memcpy(buffer, image, w*h*sizeof(double));

    for(r=0;r<h;r++)
    {
        for(c=0;c<w;c++)
        {
            double val = 0.0;
            double denom = 0.0;

            for(cd=-radius;cd<=radius;cd++)
                if(c + cd >= 0 && c + cd < w)
                {
                     double weight = kernel[cd + radius];

                     val += weight*buffer[r*w + c + cd];
                     denom += weight;
                }

            val /= denom;

            image[r*w + c] = val;
        }
    }

    delete [] kernel;
    delete [] buffer;
}


/*******************************************************************************
Bilinearly interpolate image (helper function for Stitch)
    image - input image
    (x, y) - location to interpolate
    rgb - returned color values
*******************************************************************************/
bool MainWindow::BilinearInterpolation(QImage *image, double x, double y, double rgb[3])
{

    int r = (int) y;
    int c = (int) x;
    double rdel = y - (double) r;
    double cdel = x - (double) c;
    QRgb pixel;
    double del;

    rgb[0] = rgb[1] = rgb[2] = 0.0;

    if(r >= 0 && r < image->height() - 1 && c >= 0 && c < image->width() - 1)
    {
        pixel = image->pixel(c, r);
        del = (1.0 - rdel)*(1.0 - cdel);
        rgb[0] += del*(double) qRed(pixel);
        rgb[1] += del*(double) qGreen(pixel);
        rgb[2] += del*(double) qBlue(pixel);

        pixel = image->pixel(c+1, r);
        del = (1.0 - rdel)*(cdel);
        rgb[0] += del*(double) qRed(pixel);
        rgb[1] += del*(double) qGreen(pixel);
        rgb[2] += del*(double) qBlue(pixel);

        pixel = image->pixel(c, r+1);
        del = (rdel)*(1.0 - cdel);
        rgb[0] += del*(double) qRed(pixel);
        rgb[1] += del*(double) qGreen(pixel);
        rgb[2] += del*(double) qBlue(pixel);

        pixel = image->pixel(c+1, r+1);
        del = (rdel)*(cdel);
        rgb[0] += del*(double) qRed(pixel);
        rgb[1] += del*(double) qGreen(pixel);
        rgb[2] += del*(double) qBlue(pixel);
    }
    else
        return false;

    return true;
}


/*******************************************************************************
Draw detected Harris corners
    cornerPts - corner points
    numCornerPts - number of corner points
    imageDisplay - image used for drawing

    Draws a red cross on top of detected corners
*******************************************************************************/
void MainWindow::DrawCornerPoints(CIntPt *cornerPts, int numCornerPts, QImage &imageDisplay)
{
   int i;
   int r, c, rd, cd;
   int w = imageDisplay.width();
   int h = imageDisplay.height();
   for(i=0;i<numCornerPts;i++)
   {
       c = (int) cornerPts[i].m_X;
       r = (int) cornerPts[i].m_Y;

       for(rd=-2;rd<=2;rd++)
           if(r+rd >= 0 && r+rd < h && c >= 0 && c < w)
               imageDisplay.setPixel(c, r + rd, qRgb(255, 0, 0));

       for(cd=-2;cd<=2;cd++)
           if(r >= 0 && r < h && c + cd >= 0 && c + cd < w)
               imageDisplay.setPixel(c + cd, r, qRgb(255, 0, 0));
   }
}

/*******************************************************************************
Compute corner point descriptors
    image - input image
    cornerPts - array of corner points
    numCornerPts - number of corner points

    If the descriptor cannot be computed, i.e. it's too close to the boundary of
    the image, its descriptor length will be set to 0.

    I've implemented a very simple 8 dimensional descriptor.  Feel free to
    improve upon this.
*******************************************************************************/
void MainWindow::ComputeDescriptors(QImage image, CIntPt *cornerPts, int numCornerPts)
{
    int r, c, cd, rd, i, j;
    int w = image.width();
    int h = image.height();
    double *buffer = new double [w*h];
    QRgb pixel;

    // Descriptor parameters
    double sigma = 2.0;
    int rad = 4;

    // Computer descriptors from green channel
    for(r=0;r<h;r++)
       for(c=0;c<w;c++)
        {
            pixel = image.pixel(c, r);
            buffer[r*w + c] = (double) qGreen(pixel);
        }

    // Blur
    GaussianBlurImage(buffer, w, h, sigma);

    // Compute the desciptor from the difference between the point sampled at its center
    // and eight points sampled around it.
    for(i=0;i<numCornerPts;i++)
    {
        int c = (int) cornerPts[i].m_X;
        int r = (int) cornerPts[i].m_Y;

        if(c >= rad && c < w - rad && r >= rad && r < h - rad)
        {
            double centerValue = buffer[(r)*w + c];
            int j = 0;

            for(rd=-1;rd<=1;rd++)
                for(cd=-1;cd<=1;cd++)
                    if(rd != 0 || cd != 0)
                {
                    cornerPts[i].m_Desc[j] = buffer[(r + rd*rad)*w + c + cd*rad] - centerValue;
                    j++;
                }

            cornerPts[i].m_DescSize = DESC_SIZE;
        }
        else
        {
            cornerPts[i].m_DescSize = 0;
        }
    }

    delete [] buffer;
}

/*******************************************************************************
Draw matches between images
    matches - matching points
    numMatches - number of matching points
    image1Display - image to draw matches
    image2Display - image to draw matches

    Draws a green line between matches
*******************************************************************************/
void MainWindow::DrawMatches(CMatches *matches, int numMatches, QImage &image1Display, QImage &image2Display)
{
    int i;
    // Show matches on image
    QPainter painter;
    painter.begin(&image1Display);
    QColor green(0, 250, 0);
    QColor red(250, 0, 0);

    for(i=0;i<numMatches;i++)
    {
        painter.setPen(green);
        painter.drawLine((int) matches[i].m_X1, (int) matches[i].m_Y1, (int) matches[i].m_X2, (int) matches[i].m_Y2);
        painter.setPen(red);
        painter.drawEllipse((int) matches[i].m_X1-1, (int) matches[i].m_Y1-1, 3, 3);
    }

    QPainter painter2;
    painter2.begin(&image2Display);
    painter2.setPen(green);

    for(i=0;i<numMatches;i++)
    {
        painter2.setPen(green);
        painter2.drawLine((int) matches[i].m_X1, (int) matches[i].m_Y1, (int) matches[i].m_X2, (int) matches[i].m_Y2);
        painter2.setPen(red);
        painter2.drawEllipse((int) matches[i].m_X2-1, (int) matches[i].m_Y2-1, 3, 3);
    }

}


/*******************************************************************************
Given a set of matches computes the "best fitting" homography
    matches - matching points
    numMatches - number of matching points
    h - returned homography
    isForward - direction of the projection (true = image1 -> image2, false = image2 -> image1)
*******************************************************************************/
bool MainWindow::ComputeHomography(CMatches *matches, int numMatches, double h[3][3], bool isForward)
{
    int error;
    int nEq=numMatches*2;

    dmat M=newdmat(0,nEq,0,7,&error);
    dmat a=newdmat(0,7,0,0,&error);
    dmat b=newdmat(0,nEq,0,0,&error);

    double x0, y0, x1, y1;

    for (int i=0;i<nEq/2;i++)
    {
        if(isForward == false)
        {
            x0 = matches[i].m_X1;
            y0 = matches[i].m_Y1;
            x1 = matches[i].m_X2;
            y1 = matches[i].m_Y2;
        }
        else
        {
            x0 = matches[i].m_X2;
            y0 = matches[i].m_Y2;
            x1 = matches[i].m_X1;
            y1 = matches[i].m_Y1;
        }


        //Eq 1 for corrpoint
        M.el[i*2][0]=x1;
        M.el[i*2][1]=y1;
        M.el[i*2][2]=1;
        M.el[i*2][3]=0;
        M.el[i*2][4]=0;
        M.el[i*2][5]=0;
        M.el[i*2][6]=(x1*x0*-1);
        M.el[i*2][7]=(y1*x0*-1);

        b.el[i*2][0]=x0;
        //Eq 2 for corrpoint
        M.el[i*2+1][0]=0;
        M.el[i*2+1][1]=0;
        M.el[i*2+1][2]=0;
        M.el[i*2+1][3]=x1;
        M.el[i*2+1][4]=y1;
        M.el[i*2+1][5]=1;
        M.el[i*2+1][6]=(x1*y0*-1);
        M.el[i*2+1][7]=(y1*y0*-1);

        b.el[i*2+1][0]=y0;

    }
    int ret=solve_system (M,a,b);
    if (ret!=0)
    {
        freemat(M);
        freemat(a);
        freemat(b);

        return false;
    }
    else
    {
        h[0][0]= a.el[0][0];
        h[0][1]= a.el[1][0];
        h[0][2]= a.el[2][0];

        h[1][0]= a.el[3][0];
        h[1][1]= a.el[4][0];
        h[1][2]= a.el[5][0];

        h[2][0]= a.el[6][0];
        h[2][1]= a.el[7][0];
        h[2][2]= 1;
    }

    freemat(M);
    freemat(a);
    freemat(b);

    return true;
}


/*******************************************************************************
*******************************************************************************
*******************************************************************************

    The routines you need to implement are below

*******************************************************************************
*******************************************************************************
*******************************************************************************/



/*******************************************************************************
Helper function to set non-maximum pixels to be zero for NMS
*******************************************************************************/
void setZero(double* ResMatrix, double* tmp, int r, int c, int w, int h) {
    for (int rd = -2; rd < 2; rd++) {
        for (int cd = -2; cd <2; cd++) {
            if (r+rd>=0 && r+rd < h && c+cd>=0 && c+cd<w && tmp[(r+rd)*w+c+cd] > tmp[r*w+c]){
                ResMatrix[r*w+c] = 0.0;
                return;
            }
        }
    }
}
/*******************************************************************************
Detect Harris corners.
    image - input image
    sigma - standard deviation of Gaussian used to blur corner detector
    thres - Threshold for detecting corners
    cornerPts - returned corner points
    numCornerPts - number of corner points returned
    imageDisplay - image returned to display (for debugging)
*******************************************************************************/
void MainWindow::HarrisCornerDetector(QImage image, double sigma, double thres, CIntPt **cornerPts, int &numCornerPts, QImage &imageDisplay)
{
    int r, c;
    int w = image.width();
    int h = image.height();
    double *buffer = new double [w*h];
    QRgb pixel;

    numCornerPts = 0;

    // Compute the corner response using just the green channel
    for(r=0;r<h;r++)
       for(c=0;c<w;c++)
        {
            pixel = image.pixel(c, r);

            buffer[r*w + c] = (double) qGreen(pixel);
        }

    // Write your Harris corner detection code here.
    double* Ix = FirstDerivImage_x(buffer, w, h, sigma);
    double* Iy = FirstDerivImage_y(buffer, w, h, sigma);
    double* Ix2 = ImageMul(Ix, Ix, w, h);
    double* Iy2 = ImageMul(Iy, Iy, w, h);
    double* IxIy = ImageMul(Ix, Iy, w, h);

    GaussianBlurImage(Ix2, w, h, sigma);
    GaussianBlurImage(Iy2, w, h, sigma);
    GaussianBlurImage(IxIy, w, h, sigma);
    double* ResMatrix = new double[w*h];
    for (int i=0; i<w*h; i++){
        ResMatrix[i] = (Ix2[i] * Iy2[i] - IxIy[i] * IxIy[i])/(Ix2[i] + Iy2[i] + 0.00001);
        if (ResMatrix[i] < thres)
            ResMatrix[i] = 0;
    }
    // NMS
    double *tmp = new double [w*h];
    memcpy(tmp, ResMatrix, w*h*sizeof(double));
    for(r=0;r<h;r++)
    {
        for(c=0;c<w;c++)
        {
            setZero(ResMatrix, tmp, r, c, w, h); //setZero if it's not the local maximum;
        }
    }
    // Store corners into cornerPts[]
    for (int i=0; i<w*h; i++) {
        if(ResMatrix[i]!=0.0){
            ResMatrix[i] = 255;
            numCornerPts++;
        }
    }
    numCornerPts = max(numCornerPts - 4, 0); // eliminate 4 corners and make sure it's valid when processing a single image.
    *cornerPts = new CIntPt [numCornerPts];
    int cnt=0;
    for (int i=0; i<w*h; i++) {
        if(ResMatrix[i]!=0.0){
            int c = i%w;
            int r = i/w;
            if ((r==0&&c==0)|| (r==0&&c==w-1) || (r==h-1&&c==0) || (r==h-1&&c==w-1))  // eliminate 4 corners
                std::cout<<"does not count"<<std::endl;
            else{
                (*cornerPts)[cnt].m_Y = r;
                (*cornerPts)[cnt].m_X = c;
                cnt++;
            }
        }
    }
    DrawCornerPoints(*cornerPts, numCornerPts, imageDisplay);
    // ConvertDouble2QImage(ResMatrix, &imageDisplay, w, h);  // to visulize temporal variables to help debug
    delete [] buffer;
    delete [] ResMatrix;
    delete [] tmp;
}
/*******************************************************************************
Helper function to compute L1 norm.
*******************************************************************************/
double L1Dist(double* p1, double* p2) {
    double sum=0.0;
    for (int i=0; i<DESC_SIZE; i++) {
        sum+=fabs(p1[i]-p2[i]);
    }
    return sum;
}
/*******************************************************************************
Find matching corner points between images.
    image1 - first input image
    cornerPts1 - corner points corresponding to image 1
    numCornerPts1 - number of corner points in image 1
    image2 - second input image
    cornerPts2 - corner points corresponding to image 2
    numCornerPts2 - number of corner points in image 2
    matches - set of matching points to be returned
    numMatches - number of matching points returned
    image1Display - image used to display matches
    image2Display - image used to display matches
*******************************************************************************/
void MainWindow::MatchCornerPoints(QImage image1, CIntPt *cornerPts1, int numCornerPts1,
                             QImage image2, CIntPt *cornerPts2, int numCornerPts2,
                             CMatches **matches, int &numMatches, QImage &image1Display, QImage &image2Display)
{
    numMatches = 0;

    // Compute the descriptors for each corner point.

    ComputeDescriptors(image1, cornerPts1, numCornerPts1);
    ComputeDescriptors(image2, cornerPts2, numCornerPts2);

    int num_min = numCornerPts1;
    int num_max = numCornerPts2;
    CIntPt* Pts_min = cornerPts1;
    CIntPt* Pts_max = cornerPts2;
    if (numCornerPts1 > numCornerPts2) {
        num_min = numCornerPts2;
        num_max = numCornerPts1;
        Pts_min = cornerPts2;
        Pts_max = cornerPts1;
    }

    std::vector<CIntPt*> matches_vec;
    for (int i=0; i<num_min; i++) {
        int match_id = 0;
        double dist = L1Dist(Pts_min[i].m_Desc, Pts_max[0].m_Desc);
        for (int j=1; j<num_max; j++) {
            if (L1Dist(Pts_min[i].m_Desc, Pts_max[j].m_Desc) < dist){
                match_id = j;
                dist = L1Dist(Pts_min[i].m_Desc, Pts_max[j].m_Desc);
            }
        }
        if (dist < 50) {  //threshold for matching
            CIntPt* pair = new CIntPt [2];
            pair[0] = Pts_min[i];
            pair[1] = Pts_max[match_id];
            matches_vec.push_back(pair);
            numMatches++;
        }
    }

    *matches = new CMatches [numMatches];
    int i=0;
    for (auto pair = matches_vec.begin(); pair != matches_vec.end(); ++pair) {
        // std::cout<<"round "<<i<<std::endl;
        (*matches)[i].m_X1 = (*pair)[0].m_X;
        (*matches)[i].m_Y1 = (*pair)[0].m_Y;
        (*matches)[i].m_X2 = (*pair)[1].m_X;
        (*matches)[i].m_Y2 = (*pair)[1].m_Y;
        if (numCornerPts1 > numCornerPts2) {
            (*matches)[i].m_X1 = (*pair)[1].m_X;
            (*matches)[i].m_Y1 = (*pair)[1].m_Y;
            (*matches)[i].m_X2 = (*pair)[0].m_X;
            (*matches)[i].m_Y2 = (*pair)[0].m_Y;
        }
        i++;
    }
    DrawMatches(*matches, numMatches, image1Display, image2Display);
}

/*******************************************************************************
Project a point (x1, y1) using the homography transformation h
    (x1, y1) - input point
    (x2, y2) - returned point
    h - input homography used to project point
*******************************************************************************/
void MainWindow::Project(double x1, double y1, double &x2, double &y2, double h[3][3])
{
    double u = h[0][0]*x1 + h[0][1]*y1 + h[0][2]*1;
    double v = h[1][0]*x1 + h[1][1]*y1 + h[1][2]*1;
    double w = h[2][0]*x1 + h[2][1]*y1 + h[2][2]*1;

    x2 = (u / w);
    y2 = (v / w);  //why we need to divide w ???????
}
/*******************************************************************************
Count the number of inliers given a homography.  This is a helper function for RANSAC.
    h - input homography used to project points (image1 -> image2
    matches - array of matching points
    numMatches - number of matchs in the array
    inlierThreshold - maximum distance between points that are considered to be inliers

    Returns the total number of inliers.
*******************************************************************************/
int MainWindow::ComputeInlierCount(double h[3][3], CMatches *matches, int numMatches, double inlierThreshold)
{
    int numInliers = 0;
    double x2Projected, y2Projected, distance;
    for (int i = 0; i < numMatches; i++) {
        Project(matches[i].m_X1, matches[i].m_Y1, x2Projected, y2Projected, h);
        distance = sqrt(pow(matches[i].m_X2-x2Projected, 2) + pow(matches[i].m_Y2-y2Projected, 2));
        if (distance < inlierThreshold)
            numInliers++;
    }
    return numInliers;
}
/*******************************************************************************
Helper function to generate random submatches
*******************************************************************************/
CMatches* MainWindow::genRandomMatches(CMatches *matches, int max) {
    std::unordered_set<int> HashSet;
    while (HashSet.size() < 4) {        //random genrate 4 indices of matches.
        HashSet.insert(rand()%max);
    }
    CMatches *subMatches = new CMatches [4];
    int cnt = 0;
    for (auto itr = HashSet.begin(); itr != HashSet.end(); itr++) {
        subMatches[cnt] = matches[*itr];
        cnt++;
    }
    return subMatches;
}
/*******************************************************************************
Helper function to get final Matches
*******************************************************************************/
CMatches* MainWindow::getInliers(double h[3][3], CMatches *matches, int numMatches, double inlierThreshold, int maxInliers)
{
    double x2Projected, y2Projected, distance;
    CMatches* finalMatches = new CMatches [maxInliers];
    int cnt = 0;
    for (int i = 0; i < numMatches; i++) {
        Project(matches[i].m_X1, matches[i].m_Y1, x2Projected, y2Projected, h);
        distance = sqrt(pow(matches[i].m_X2-x2Projected, 2) + pow(matches[i].m_Y2-y2Projected, 2));
        if (distance < inlierThreshold){
            finalMatches[cnt] = matches[i];
            cnt++;
        }
    }
    return finalMatches;
}
/*******************************************************************************
Compute homography transformation between images using RANSAC.
    matches - set of matching points between images
    numMatches - number of matching points
    numIterations - number of iterations to run RANSAC
    inlierThreshold - maximum distance between points that are considered to be inliers
    hom - returned homography transformation (image1 -> image2)
    homInv - returned inverse homography transformation (image2 -> image1)
    image1Display - image used to display matches
    image2Display - image used to display matches
*******************************************************************************/
void MainWindow::RANSAC(CMatches *matches, int numMatches, int numIterations, double inlierThreshold,
                        double hom[3][3], double homInv[3][3], QImage &image1Display, QImage &image2Display)
{
    // Add your code here.
    int maxInliers = 0;
    CMatches* bestMatches;
    for (int i=0; i<numIterations; i++) {
        CMatches* subMatches = genRandomMatches(matches, numMatches);
        ComputeHomography(subMatches, 4, hom, true);
        int numInliers = ComputeInlierCount(hom, matches, numMatches, inlierThreshold);
        if (numInliers > maxInliers) {
            bestMatches = subMatches;
            maxInliers = numInliers;
        }
    }
    ComputeHomography(bestMatches, 4, hom, true);
    CMatches* finalMatches = getInliers(hom, matches, numMatches, inlierThreshold, maxInliers);
    ComputeHomography(finalMatches, maxInliers, hom, true);
    ComputeHomography(finalMatches, maxInliers, homInv, false);
    // After you're done computing the inliers, display the corresponding matches.
    DrawMatches(finalMatches, maxInliers, image1Display, image2Display);
}

/*******************************************************************************
Stitch together two images using the homography transformation
    image1 - first input image
    image2 - second input image
    hom - homography transformation (image1 -> image2)
    homInv - inverse homography transformation (image2 -> image1)
    stitchedImage - returned stitched image
*******************************************************************************/

/*******************************************************************************
I refered the implementation of stitch from github: https://github.com/AtOMiCNebula/UW-CSEP576/
Specifically, I refered how he computed the size of stitchedImage: ws, hs.
*******************************************************************************/
void MainWindow::Stitch(QImage image1, QImage image2, double hom[3][3], double homInv[3][3], QImage &stitchedImage)
{
    // Width and height of stitchedImage
    int ws = 0;
    int hs = 0;

    int w1 = image1.width();
    int h1 = image1.height();
    int w2 = image2.width();
    int h2 = image2.height();

    // Project four corners of image2 into image1's coordinate system
    double cornerX_NW, cornerY_NW;
    Project(0, 0, cornerX_NW, cornerY_NW, homInv);
    double cornerX_NE, cornerY_NE;
    Project(w2-1, 0, cornerX_NE, cornerY_NE, homInv);
    double cornerX_SE, cornerY_SE;
    Project(w2-1, h2-1, cornerX_SE, cornerY_SE, homInv);
    double cornerX_SW, cornerY_SW;
    Project(0, h2-1, cornerX_SW, cornerY_SW, homInv);

    // Calculate the width and height of stitchedImage using projected corners
    double minWidth = min(min(cornerX_NW, cornerX_NE), min(cornerX_SE, cornerX_SW));
    double minHeight = min(min(cornerY_NW, cornerY_NE), min(cornerY_SE, cornerY_SW));
    double maxWidth = max(max(cornerX_NW, cornerX_NE), max(cornerX_SE, cornerX_SW));
    double maxHeight = max(max(cornerY_NW, cornerY_NE), max(cornerY_SE, cornerY_SW));
    int stitchedXOffset = abs(min(0, static_cast<int>(floor(minWidth))));
    int stitchedYOffset = abs(min(0, static_cast<int>(floor(minHeight))));
    ws = stitchedXOffset + max(w1, static_cast<int>(ceil(maxWidth)));
    hs = stitchedYOffset + max(h1, static_cast<int>(ceil(maxHeight)));

    stitchedImage = QImage(ws, hs, QImage::Format_RGB32);
    stitchedImage.fill(qRgb(0,0,0));

    // Copy image1 into stitchedImage
    for (int y = 0; y < h1; y++) {
        for (int x = 0; x < w1; x++) {
            stitchedImage.setPixel(stitchedXOffset+x, stitchedYOffset+y, image1.pixel(x, y));
        }
    }
    // BrainStorm: how to implement image blend, if the two img are not parallel ??????
    for (int y = 0; y < hs; y++) {
        for (int x = 0; x < ws; x++) {
            double x2Projected, y2Projected;
            Project(x-stitchedXOffset, y-stitchedYOffset, x2Projected, y2Projected, hom);
            if (0 <= x2Projected && x2Projected < w2 && 0 <= y2Projected && y2Projected < h2) {
                double rgb[3];
                BilinearInterpolation(&image2, x2Projected, y2Projected, rgb);
                stitchedImage.setPixel(x, y, qRgb(static_cast<int>(floor(rgb[0])),
                                                  static_cast<int>(floor(rgb[1])),
                                                  static_cast<int>(floor(rgb[2]))));
            }
        }
    }
}

