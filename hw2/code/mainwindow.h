#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

template <class T> const T& max ( const T& a, const T& b ) {
  return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

template <class T> const T& min ( const T& a, const T& b ) {
  return (b>a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

#define DESC_SIZE   8

class CIntPt
{
public:
    double m_X;  // X position of corner point
    double m_Y;  // Y position of corner point
    double m_Desc[DESC_SIZE]; // The corner points descriptor
    int m_DescSize; // The size of the desciptor
};

class CMatches
{
public:
    double m_X1;  // The position (m_X1, m_Y1) of the match in image1
    double m_Y1;
    double m_X2;  // The position (m_X2, m_Y2) of the match in image2
    double m_Y2;
};


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QImage m_InImage1;
    QImage m_InImage2;
    QImage m_InImage1Display;
    QImage m_InImage2Display;
    QImage m_StitchedImage;
    QImage m_DisplayImage;

    CIntPt *m_IntPts1;
    int m_NumIntPts1;
    CIntPt *m_IntPts2;
    int m_NumIntPts2;
    CMatches *m_Matches;
    int m_NumMatches;
    double m_Hom[3][3];
    double m_HomInv[3][3];

    void DrawDisplayImage();

    void GaussianBlurImage(double *image, int w, int h, double sigma);
    void HarrisCornerDetector(QImage image, double sigma, double thres, CIntPt **cornerPts, int &numCornerPts, QImage &imageDisplay);
    void DrawCornerPoints(CIntPt *cornerPts, int numCornerPts, QImage &imageDisplay);
    void MatchCornerPoints(QImage image1, CIntPt *cornerPts1, int numCornerPts1,
                     QImage image2, CIntPt *cornerPts2, int numCornerPts2,
                     CMatches **matches, int &numMatches, QImage &image1Display, QImage &image2Display);
    void ComputeDescriptors(QImage image, CIntPt *cornerPts, int numCornerPts);
    void DrawMatches(CMatches *matches, int numMatches, QImage &image1Display, QImage &image2Display);
    void RANSAC(CMatches *matches, int numMatches, int numIterations, double inlierThreshold,
                double hom[3][3], double homInv[3][3], QImage &image1Display, QImage &image2Display);
    bool ComputeHomography(CMatches *matches, int numMatches, double h[3][3], bool isForward);
    void Project(double x1, double y1, double &x2, double &y2, double h[3][3]);
    int ComputeInlierCount(double h[3][3], CMatches *matches, int numMatches, double inlierThreshold);
    void Stitch(QImage image1, QImage image2, double hom[3][3], double homInv[3][3], QImage &stitchedImage);
    bool BilinearInterpolation(QImage *image, double x, double y, double rgb[3]);

private slots:
    void OpenImage();
    void SaveImage();
    void HarrisCornerImage();
    void MatchImages();
    void RANSAC();
    void StitchImages();
    double* PaddingImage(double* image, int imageWidth, int imageHeight, int kernelWidth, int kernelHeight);
    void Convolution(double* image, int imageWidth, int imageHeight, double *kernel, int kernelWidth, int kernelHeight, bool add=false);
    double* FirstDerivImage_x(double* image, int w, int h, double sigma);
    double* FirstDerivImage_y(double* image, int w, int h, double sigma);
    void ConvertDouble2QImage(double* Image, QImage *image, int imageWidth, int imageHeight);
    QRgb restrictColor(double red, double green, double blue);

    double* ImageMul(double* Ix, double* Iy, int w, int h);
    CMatches* getInliers(double h[3][3], CMatches *matches, int numMatches, double inlierThreshold, int maxInliers);
    CMatches* genRandomMatches(CMatches *matches, int max);

};

#endif // MAINWINDOW_H
