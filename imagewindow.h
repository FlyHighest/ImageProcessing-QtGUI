#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H
#include <QMdiSubWindow>
#include <QCloseEvent>
class ImageWindow : public QMdiSubWindow
{
public:
    ImageWindow(QImage,int);
    QImage img;
    int no;
    void closeEvent(QCloseEvent*);
};

#endif // IMAGEWINDOW_H
