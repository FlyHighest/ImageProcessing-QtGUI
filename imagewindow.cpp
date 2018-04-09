#include "imagewindow.h"

ImageWindow::ImageWindow(QImage image,int no)
{
    this->img=QImage(image);
    this->no=no;
}

void ImageWindow::closeEvent(QCloseEvent *event)
{
    this->setVisible(false);
    event->ignore();

}
