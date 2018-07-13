#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H
#include <QMdiSubWindow>
#include "fourier.h"
#include <QCloseEvent>
class ImageWindow : public QMdiSubWindow
{


public:
    ImageWindow(QImage,int);
    ImageWindow(Complex*,QImage, int);
    QImage img;
    Complex * fft;
    int no;
    void closeEvent(QCloseEvent*);
    bool isClosed;
};

#endif // IMAGEWINDOW_H
