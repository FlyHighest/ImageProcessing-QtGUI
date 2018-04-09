#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //绑定ui界面
    mdiArea=new QMdiArea;
    this->setCentralWidget(mdiArea);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ac_openfile_triggered()
{
    qDebug("on_ac_openfile_triggered");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        "/",
                                                        tr("Images (*.bmp *.raw *.jpg)"));
    //if(fileName!=NULL)
        //this->img_handler->open_image(fileName);

}

void MainWindow::showimage_triggered(QImage img)
{
    QLabel * lb=new QLabel(this);
    lb->setPixmap(QPixmap::fromImage(img));
    lb->resize(img.width(),img.height());
    QScrollArea * scrollArea=new QScrollArea(this->mdiArea);
    scrollArea->setWidget(lb);
    QMdiSubWindow * sw1=mdiArea->addSubWindow(scrollArea);

    sw1->show();
}
