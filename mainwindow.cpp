#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //绑定ui界面
    mdiArea=new QMdiArea;
    this->setCentralWidget(mdiArea);
    statusBarMessage();
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
    if(fileName!=NULL){
        QImage img(fileName);
        this->showimage(img);
    }


}

//每次showimage调用，代表生成或者加载了一张新的图像，为它开启一个imagewindow，同时将imagewindow的指针
//保存起来，在windowMap当中。每个ImageWindow有唯一的编号。
void MainWindow::showimage(QImage img)
{
    int count=windowMap.size();
    ImageWindow * subwindow=new ImageWindow(img,count);
    subwindow->setParent(mdiArea);
    subwindow->setWindowTitle(QString::number(count));
    QLabel * lb=new QLabel(this);
    lb->setPixmap(QPixmap::fromImage(img));
    lb->resize(img.width(),img.height());
    QScrollArea * scrollArea=new QScrollArea(this->mdiArea);
    scrollArea->setWidget(lb);
    subwindow->setWidget(scrollArea);
    subwindow->show();

    this->windowMap.insert(WMap::value_type(count,subwindow));
    statusBarMessage();
}

void MainWindow::on_ac_showall_triggered()
{
    WMap::iterator iter;

    for(iter = this->windowMap.begin(); iter != windowMap.end(); iter++){
        ImageWindow * w=(ImageWindow *)iter->second;
        w->setVisible(true);
    }


}

QString MainWindow::statusBarMessage()
{
    int count=windowMap.size();
    QString str;
    if(count==0)
        str= QString("当前处理图像编号:")+QString("无")+QString(" | 当前图像总数:")+QString("无");
    else
        str= QString("当前处理图像编号:")+QString::number(current)+QString(" | 当前图像总数:")+QString::number(count);
    ui->statusBar->showMessage(str);
}


