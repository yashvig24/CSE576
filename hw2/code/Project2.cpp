#include "mainwindow.h"
#include "math.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "Matrix.h"
#include <iostream>
#include <vector>


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
Detect Harris corners.
    image - input image
    sigma - standard deviation of Gaussian used to blur corner detector
    thres - Threshold for detecting corners
    cornerPts - returned corner points
    numCornerPts - number of corner points returned
    imageDisplay - image returned to display (for debugging)
*******************************************************************************/
// helper function to do NMS
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
    // Once you know the number of corner points allocate an array as follows:
    // Store corners into cornerPts[]
    for (int i=0; i<w*h; i++) {
        if(ResMatrix[i]!=0.0){
            ResMatrix[i] = 255;
            numCornerPts++;
        }
    }
    numCornerPts = max(numCornerPts - 4, 0); // eluminate 4 corners and make sure it's valid when processing a single image.
    *cornerPts = new CIntPt [numCornerPts];
    int cnt=0;
    for (int i=0; i<w*h; i++) {
        if(ResMatrix[i]!=0.0){
            int c = i%w;
            int r = i/w;
            if ((r==0&&c==0)|| (r==0&&c==w-1) || (r==h-1&&c==0) || (r==h-1&&c==w-1))
                std::cout<<"cao"<<std::endl;
            else{
                (*cornerPts)[cnt].m_Y = r;
                (*cornerPts)[cnt].m_X = c;
                cnt++;                            
            }                
        }
    }    
    // Access the values using: (*cornerPts)[i].m_X = 5.0;
    //
    // The position of the corner point is (m_X, m_Y)
    // The descriptor of the corner point is stored in m_Desc
    // The length of the descriptor is m_DescSize, if m_DescSize = 0, then it is not valid.

    // Once you are done finding the corner points, display them on the image
    DrawCornerPoints(*cornerPts, numCornerPts, imageDisplay);
    // ConvertDouble2QImage(ResMatrix, &imageDisplay, w, h);

    delete [] buffer;
    delete [] ResMatrix;
    delete [] tmp;
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
double L1Dist(double* p1, double* p2) {
    double sum=0.0;
    for (int i=0; i<DESC_SIZE; i++) {
        sum+=fabs(p1[i]-p2[i]);
    }
    return sum;
}

void MainWindow::MatchCornerPoints(QImage image1, CIntPt *cornerPts1, int numCornerPts1,
                             QImage image2, CIntPt *cornerPts2, int numCornerPts2,
                             CMatches **matches, int &numMatches, QImage &image1Display, QImage &image2Display)
{
    numMatches = 0;

    // Compute the descriptors for each corner point.
    // You can access the descriptor for each corner point using cornerPts1[i].m_Desc[j].
    // If cornerPts1[i].m_DescSize = 0, it was not able to compute a descriptor for that point
    ComputeDescriptors(image1, cornerPts1, numCornerPts1);
    ComputeDescriptors(image2, cornerPts2, numCornerPts2);

    // Add your code here for finding the best matches for each point.

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

        if (dist < 100) {
            CIntPt* pair = new CIntPt [2];
            pair[0] = Pts_min[i];
            pair[1] = Pts_max[match_id];
            matches_vec.push_back(pair);
            numMatches++;
        }
    }
    std::cout << "numMatches = "<< numMatches <<std::endl;
    std::cout << "vector_size = "<< matches_vec.size() <<std::endl;
    *matches = new CMatches [numMatches];
    int i=0;
    for (auto pair = matches_vec.begin(); pair != matches_vec.end(); ++pair) {
        std::cout<<"round "<<i<<std::endl;
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
    // for (int i=0; i<numMatches; i++) {
    //     CIntPt* pair = matches_vec.pop_back();
    //     *matches[i].m_X1 = pair[0].m_X;
    //     *matches[i].m_Y1 = pair[0].m_Y;
    //     *matches[i].m_X2 = pair[1].m_X;
    //     *matches[i].m_Y2 = pair[1].m_Y;
    //     if (numCornerPts1 > numCornerPts2) {
    //         *matches[i].m_X1 = pair[1].m_X;
    //         *matches[i].m_Y1 = pair[1].m_Y;
    //         *matches[i].m_X2 = pair[0].m_X;
    //         *matches[i].m_Y2 = pair[0].m_Y;
    //     }
    // }
    
    // Once you uknow the number of matches allocate an array as follows:
    // *matches = new CMatches [numMatches];
    //
    // The position of the corner point in iamge 1 is (m_X1, m_Y1)
    // The position of the corner point in image 2 is (m_X2, m_Y2)

    // Draw the matches
    std::cout<<"it's ok"<<std::endl;
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
    // Add your code here.
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
    // Add your code here.

    return 0;
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

    // After you're done computing the inliers, display the corresponding matches.
    //DrawMatches(inliers, numInliers, image1Display, image2Display);

}

/*******************************************************************************
Stitch together two images using the homography transformation
    image1 - first input image
    image2 - second input image
    hom - homography transformation (image1 -> image2)
    homInv - inverse homography transformation (image2 -> image1)
    stitchedImage - returned stitched image
*******************************************************************************/
void MainWindow::Stitch(QImage image1, QImage image2, double hom[3][3], double homInv[3][3], QImage &stitchedImage)
{
    // Width and height of stitchedImage
    int ws = 0;
    int hs = 0;

    // Add your code to compute ws and hs here.

    stitchedImage = QImage(ws, hs, QImage::Format_RGB32);
    stitchedImage.fill(qRgb(0,0,0));

    // Add you code to warp image1 and image2 to stitchedImage here.
}

