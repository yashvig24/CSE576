#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QMainWindow>

template <class T> const T& max ( const T& a, const T& b ) {
  return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

template <class T> const T& min ( const T& a, const T& b ) {
  return (b>a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

namespace Ui {
    class MainWindow;
}

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
        KmeanCluster(double** image, int img_size, int num_clusters) {
            this->img_size = img_size;
            this->num_clusters = num_clusters;
            this->image = image;
            this->centers = new double* [num_clusters];
            this->sumValue = new double* [num_clusters];
            this->numOfPixel = new int[num_clusters];
        }

        void RandInit() {
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

        void PixelInit() {
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

        double L1Dist(double* p1, double* p2) {
            return (fabs(p1[0]-p2[0]) + fabs(p1[1]-p2[1]) + fabs(p1[2]-p2[2]));
        }

        void UpdateCenter(){
            for (int n=0; n<num_clusters; n++) {
                for (int k=0; k<3; k++) {
                    centers[n][k] = sumValue[n][k]/numOfPixel[n];
                }
            }
        }

        void ResetSum(){
            for (int n=0; n<num_clusters; n++) {
                numOfPixel[n] = 0;
                for (int k=0; k<3; k++) {
                    sumValue[n][k] = 0;
                }
            }
        }

        bool IsBad() {
            int num=0;
            for (int n=0; n<num_clusters; n++) {
                if (numOfPixel[n]<10) {
                    num++;
                }
            }
            std::cout << "bad num = " << num << std::endl;
            if (num>1)
                return true;
            else
                return false;
        }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage inImage;
    QImage outImage;
    QImage displayImage;
    double** Image;
    int imageWidth;
    int imageHeight;
    double xScroll;
    double yScroll;
    bool zoomChanged;

    void DrawDisplayImage();
    void BlackWhiteImage(QImage *image);
    void AddNoise(QImage *image, double mag, bool colorNoise);
    void MeanBlurImage(QImage *image, int radius);
    void HalfImage(QImage &image);

    void Convolution(double** image, double *kernel, int kernelWidth, int kernelHeight, bool add);
    void GaussianBlurImage(double** image, double sigma);
    void SeparableGaussianBlurImage(double** image, double sigma);
    void FirstDerivImage_x(double** image, double sigma);
    void FirstDerivImage_y(double** image, double sigma);
    void SecondDerivImage(double** image, double sigma);
    void SharpenImage(double** image, double sigma, double alpha);
    void SobelImage(double** image);
    void BilinearInterpolation(double** image, double x_s, double y_s, double rgb[3]);
    void RotateImage(double** image, double orien);
    void FindPeaksImage(double** image, double thres);

    void HoughImage(double** image);
    void SmartKMeans(double** image);
    void RandomSeedImage(double** image, int num_clusters);
    void PixelSeedImage(double** image, int num_clusters);
    void HistogramSeedImage(double** image, int num_clusters);
    void BilateralImage(double** image, double sigmaS, double sigmaI);
    void MedianImage(double** image, int radius);


private slots:
    double* getPixel(double** image, int x, int y);
    double** ImageCopy(double** image);
    double** PaddingImage(double** image, int kernelWidth, int kernelHeight);
    void OpenImage();
    void SaveImage();
    void SaveDisplayImage();
    void ResetImage();
    void ToggleImage();
    void AddNoise();
    void BlackWhiteImage();
    void MeanBlurImage();
    void GaussianBlurImage();
    void FirstDerivImage_x();
    void FirstDerivImage_y();
    void SecondDerivImage();
    void SharpenImage();
    void SobelImage();
    void HalfImage();
    void RotateImage();
    void Brightness(int val);
    void Zoom(int val);
    void Scroll(int val);
    void BilateralImage();
    void FindPeaksImage();
    void MedianImage();
    void HoughImage();
    void SmartKMeans();
    void RandomSeedImage();
    void PixelSeedImage();
    void HistogramSeedImage();
    void ConvertQImage2Double(QImage image);
    void ConvertDouble2QImage(QImage *image);
};

#endif // MAINWINDOW_H
