#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(OpenImage()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(SaveImage()));
    connect(ui->harrisButton, SIGNAL(clicked()), this, SLOT(HarrisCornerImage()));
    connect(ui->matchButton, SIGNAL(clicked()), this, SLOT(MatchImages()));
    connect(ui->RANSACButton, SIGNAL(clicked()), this, SLOT(RANSAC()));
    connect(ui->stitchButton, SIGNAL(clicked()), this, SLOT(StitchImages()));

    ui->harrisSpinBox->setValue(2.0);
    ui->harrisThresSpinBox->setValue(50.0);
    ui->RANSACThresSpinBox->setValue(5.0);
    ui->iterationsBox->setValue(200);

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawDisplayImage()
{
    ui->img1Display->setPixmap(QPixmap::fromImage(m_InImage1Display));
    ui->img2Display->setPixmap(QPixmap::fromImage(m_InImage2Display));
    ui->imgSDisplay->setPixmap(QPixmap::fromImage(m_StitchedImage));
}



void MainWindow::OpenImage()
{
    const QString title;

    QString fileName = QFileDialog::getOpenFileName(this, title);

    if(ui->tabWidget->currentIndex() == 0)
    {
        if (!fileName.isEmpty())
            m_InImage1.load(fileName);

        m_InImage1Display = m_InImage1.copy();
    }

    if(ui->tabWidget->currentIndex() == 1)
    {
        if (!fileName.isEmpty())
            m_InImage2.load(fileName);

        m_InImage2Display = m_InImage2.copy();
    }

    DrawDisplayImage();

}

void MainWindow::SaveImage()
{
    const QString title;

    QString fileName = QFileDialog::getSaveFileName(this, title);

    if(ui->tabWidget->currentIndex() ==  0)
    {
        if (!fileName.isEmpty())
            m_InImage1Display.save(fileName);
    }

    if(ui->tabWidget->currentIndex() ==  1)
    {
        if (!fileName.isEmpty())
            m_InImage2Display.save(fileName);
    }

    if(ui->tabWidget->currentIndex() ==  2)
    {
        if (!fileName.isEmpty())
            m_StitchedImage.save(fileName);
    }
}

void MainWindow::HarrisCornerImage()
{
    double sigma = ui->harrisSpinBox->value();
    double thres = ui->harrisThresSpinBox->value();

    HarrisCornerDetector(m_InImage1, sigma, thres, &m_IntPts1, m_NumIntPts1, m_InImage1Display);
    HarrisCornerDetector(m_InImage2, sigma, thres, &m_IntPts2, m_NumIntPts2, m_InImage2Display);

    DrawDisplayImage();
}

void MainWindow::MatchImages()
{
    m_InImage1Display = m_InImage1.copy();
    m_InImage2Display = m_InImage2.copy();

    MatchCornerPoints(m_InImage1, m_IntPts1, m_NumIntPts1,
               m_InImage2, m_IntPts2, m_NumIntPts2,
               &m_Matches, m_NumMatches, m_InImage1Display, m_InImage2Display);

    DrawDisplayImage();
}

void MainWindow::RANSAC()
{
    int numIterations = ui->iterationsBox->value();
    double inlierThreshold = ui->RANSACThresSpinBox->value();

    m_InImage1Display = m_InImage1.copy();
    m_InImage2Display = m_InImage2.copy();

    RANSAC(m_Matches, m_NumMatches, numIterations, inlierThreshold,
           m_Hom, m_HomInv, m_InImage1Display, m_InImage2Display);

    DrawDisplayImage();
}

void MainWindow::StitchImages()
{
    Stitch(m_InImage1, m_InImage2, m_Hom, m_HomInv, m_StitchedImage);

    ui->tabWidget->setCurrentIndex(2);
    DrawDisplayImage();
}

double* MainWindow::PaddingImage(double* image, int imageWidth, int imageHeight, int kernelWidth, int kernelHeight) {
/*
 * return a zero-padding image
*/    
    int khh = (kernelHeight / 2); //kernelHalfHeight
    int khw = (kernelWidth / 2); //kernelHalfWidth
    int new_width = imageWidth + 2 * khw;
    int new_height = imageHeight + 2 * khh;
    double* image_pad = new double[new_height*new_width];
    for (int i = 0; i < new_height*new_width; i++) {
        image_pad[i] = 0.0;
    }
    for (int i = 0; i < imageHeight; i++) {
        int i_pad = i + khh;
        for (int j = 0; j < imageWidth; j++) {
            int j_pad = j + khw;
            image_pad[i_pad*new_width + j_pad] = image[i*imageWidth + j];
        }
    }
    return image_pad;
}

void MainWindow::Convolution(double* image, int imageWidth, int imageHeight, double *kernel, int kernelWidth, int kernelHeight, bool add)
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
    double* image_pad = PaddingImage(image, imageWidth, imageHeight, kernelWidth, kernelHeight);
    // std::cout << "hi" << std::endl;
    int khh = (kernelHeight / 2); //kernelHalfHeight
    int khw = (kernelWidth / 2); //kernelHalfWidth
    int new_width = imageWidth + 2 * khw;
    int new_height = imageHeight + 2 * khh;

    for (int r = 0; r < imageHeight; r++) {
        for (int c = 0; c < imageWidth; c++) {
            double val = 0.0;
            for(int rd = -khh; rd <= khh; rd++) {
                int i_pad = r + rd + khh;
                for(int cd = -khw; cd <= khw; cd++) { 
                    int j_pad = c + cd + khw;
                    double pixel= image_pad[i_pad * new_width + j_pad];
                    double weight = kernel[(rd + khh)*kernelWidth + cd + khw];
                    val += weight * pixel;
                }
            }
            if (add) 
                image[r*imageWidth + c] = val + 128;
            else 
                image[r*imageWidth + c] = val;
        }
    }
    delete [] image_pad;
}

double* MainWindow::FirstDerivImage_x(double* image, int w, int h, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0) {
        return NULL;
    }

    double *buffer = new double [w*h];
    memcpy(buffer, image, w*h*sizeof(double));

    double kernel[3] = {-1.0, 0.0, 1.0};
    Convolution(buffer, w, h, kernel, 3, 1, false);
    // GaussianBlurImage(buffer, w, h, sigma);

    return buffer;
    // delete[] kernel; // why this doesn't work?
}

double* MainWindow::FirstDerivImage_y(double* image, int w, int h, double sigma)
/*
 * image: input image in matrix form of size (imageWidth*imageHeight)*3 having double values
 * sigma: standard deviation for the Gaussian kernel
*/
{
    if (sigma <= 0) {
        return NULL;
    }

    double *buffer = new double [w*h];
    memcpy(buffer, image, w*h*sizeof(double));

    double kernel[3] = {-1.0, 0.0, 1.0};
    Convolution(buffer, w, h, kernel, 1, 3, false);
    // GaussianBlurImage(buffer, w, h, sigma);

    return buffer;
    // delete[] kernel; // why this doesn't work?
}

void MainWindow::ConvertDouble2QImage(double* Image, QImage *image, int imageWidth, int imageHeight)
{
    for (int r = 0; r < imageHeight; r++)
        for (int c = 0; c < imageWidth; c++) {
            // std::cout << "height:" << imageHeight;
            // std::cout << "width:" << imageWidth;
            image->setPixel(c, r, restrictColor(Image[r*imageWidth+c], Image[r*imageWidth+c], Image[r*imageWidth+c]));
        }
}

QRgb MainWindow::restrictColor(double red, double green, double blue)
{
    int r = (int)(floor(red+0.5));
    int g = (int)(floor(green+0.5));
    int b = (int)(floor(blue+0.5));

    return qRgb(max(0, min(255, r)), max(0, min(255, g)), max(0, min(255, b)));
}

double* MainWindow::ImageMul(double* Ix, double* Iy, int w, int h)
{
    double* result = new double[w*h];
    for (int i=0; i<w*h; i++) {
        result[i] = Ix[i] * Iy[i];
    }
    return result;
}
