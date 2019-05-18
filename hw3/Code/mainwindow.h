#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    std::vector<double*> queryfeature;
    std::vector<std::vector<double*>> databasefeatures;
    double *distances;
    int num_images;
    int featurevectorlength;
    std::vector<std::string> names;
    std::vector<std::string> names_copy;
    double xScroll;
    double yScroll;

    void DrawDisplayImage();
    void CalculateDistances(bool isOne);
    void SortDistances();
    void ViewDatabase();
    std::vector<double*> ExtractFeatureVector(QImage image);

private slots:
    void LoadDatabase();
    void OpenImage();
    void QueryDatabase();
    void Reset();
    
    void DownSample(QImage img_gray, int *img_down, int w, int h);
    double *** InitGLCM(int num_regions, int size);
    void updateGLCM(double ***glcm, int regionIndex, int r, int c, int d_y, int d_x, int w, int h, int* img_down);
    void normGLCM(double ***glcm, int num_regions, int size);
    void analyzeGLCM(double ***glcm, int num_regions, double *energy, double *entropy, double *contrast);
    int CleanNoise(int *nimg, int w, int h, int num_regions, int region_thresh);
    void computeCentroid(double **centroid, int num_regions, int *nimg, int w, int h);
    void computeBBox(double **boundry, int *nimg, int num_regions, int w, int h);
};

#endif // MAINWINDOW_H
