#include "imagewindow.h"

ImageWindow::ImageWindow(QImage image, int no)
{
    this->fft=NULL;
    this->img=QImage(image);
    this->no=no;
    this->isClosed=false;
}
ImageWindow::ImageWindow(Complex * fft,QImage image,int no)
{
    this->fft=fft;
    this->img=QImage(image);
    this->no=no;
    this->isClosed=false;
}

void ImageWindow::closeEvent(QCloseEvent *event)
{

    if(isClosed)
        event->accept();
    else{
        this->setVisible(false);
        event->ignore();
    }

}


