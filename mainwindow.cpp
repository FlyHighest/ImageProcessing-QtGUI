#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //绑定ui界面
    mdiArea=new QMdiArea;
    mdiArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
   // mdiArea->setParent(ui->widget);
    //    this->setCentralWidget(mdiArea);
    ui->horizontalLayout_2->addWidget(mdiArea);
    statusBarMessage();
    this->inProcessing=false;
    ui->menuBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ac_openfile_triggered()
{
    qDebug("on_ac_openfile_triggered");
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("打开图像文件"),
                                                        "/",
                                                        tr("Images (*.bmp *.jpeg *.jpg *.png)"));
    if(fileName!=NULL){
        QImage img(fileName);
        this->showimage(img);
    }


}

//每次showimage调用，代表生成或者加载了一张新的图像，为它开启一个imagewindow，同时将imagewindow的指针
//保存起来，在windowMap当中。每个ImageWindow有唯一的编号。
void MainWindow::showimage(QImage img)
{
    int number=0;
    for(;true;number++){
        if(windowMap.find(number)!=windowMap.end()){
            continue;
        }else{
            break;
        }
    }
    ImageWindow * subwindow=new ImageWindow(img,number);
    subwindow->setParent(mdiArea);
    subwindow->setWindowTitle(QString::number(number));
    QLabel * lb=new QLabel(this);
    lb->setPixmap(QPixmap::fromImage(img));
    lb->resize(img.width(),img.height());
    QScrollArea * scrollArea=new QScrollArea(this->mdiArea);
    scrollArea->setWidget(lb);
    subwindow->setWidget(scrollArea);
    subwindow->show();


    this->windowMap.insert(WMap::value_type(number,subwindow));
    statusBarMessage();
    updateListWidget();
}
void MainWindow::showimage(Complex * fft,QImage img)
{
    int number=0;
    for(;true;number++){
        if(windowMap.find(number)!=windowMap.end()){
            continue;
        }else{
            break;
        }
    }
    ImageWindow * subwindow=new ImageWindow(fft,img,number);
    subwindow->setParent(mdiArea);
    subwindow->setWindowTitle(QString::number(number));
    QLabel * lb=new QLabel(this);
    lb->setPixmap(QPixmap::fromImage(img));
    lb->resize(img.width(),img.height());
    QScrollArea * scrollArea=new QScrollArea(this->mdiArea);
    scrollArea->setWidget(lb);
    subwindow->setWidget(scrollArea);
    subwindow->show();


    this->windowMap.insert(WMap::value_type(number,subwindow));
    statusBarMessage();
    updateListWidget();
}
void MainWindow::updateListWidget()
{
    ui->listWidget->clear();
    WMap::iterator iter;
    for(iter = this->windowMap.begin(); iter != windowMap.end(); iter++){
        int w=(int)iter->first;
        QListWidgetItem*item=new QListWidgetItem;

        item->setText(QString::number(w));
        if(this->inProcessing && w==this->current)
            item->setIcon(QIcon(":/icons/icons/pen.png"));
        else
            item->setIcon(QIcon(":/icons/icons/pengrey.png"));

        ui->listWidget->addItem(item);
    }

}

void MainWindow::histogramEqualization(int **value,int width,int height)
{
    long counts[256];
    memset(counts,0,sizeof(int)*256);
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            counts[value[i][j]]++;
        }
    }
    int table[256];
    table[0]=255*counts[0]/width/height;
    for(int i=1;i<256;i++){

        counts[i]=counts[i]+counts[i-1];
        table[i]=255*counts[i]/width/height;
    }
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            value[i][j]=table[value[i][j]];
        }
    }
}

//所有点过右上角叉的图像显示出来
void MainWindow::on_ac_showall_triggered()
{
    WMap::iterator iter;

    for(iter = this->windowMap.begin(); iter != windowMap.end(); iter++){
        ImageWindow * w=(ImageWindow *)iter->second;
        w->setVisible(true);
    }


}

void MainWindow::statusBarMessage()
{
    int count=windowMap.size();
    QString str;
    if(count==0)
        str= QString("当前处理图像编号:")+QString("无")+QString(" | 当前图像总数:")+QString("无");
    else if(this->inProcessing)
        str= QString("当前处理图像编号:")+QString::number(current)+QString(" | 当前图像总数:")+QString::number(count);
    else
        str= QString("当前处理图像编号:")+QString("无")+QString(" | 当前图像总数:")+QString::number(count);

    ui->statusBar->showMessage(str);
}



void MainWindow::on_ac_setcurrent_triggered()
{
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    if(items.size()!=1){
        //maybe show some warning here
        return;
    }
    QListWidgetItem * item=items.back();
    QString nostr=item->text();
    this->inProcessing=true;
    this->current=nostr.toInt();
    statusBarMessage();
    updateListWidget();
}

void MainWindow::on_ac_delete_triggered()
{
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    if(items.size()!=1){
        //maybe show some warning here
        return;
    }
    QListWidgetItem * item=items.back();
    QString nostr=item->text();
    int del=nostr.toInt();
    this->windowMap[del]->isClosed=true;
    this->windowMap[del]->close();
    this->windowMap.erase(del);
    if(del==current){
        this->inProcessing=false;

    }
    statusBarMessage();
    updateListWidget();
}



void MainWindow::on_toolButton_grey_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img(this->windowMap[current]->img);
    for(int i=0;i<img.width();i++){
        for(int j=0;j<img.height();j++){
            QRgb c=img.pixel(i,j);
            QColor n;
            n.setRed(qGray(c));
            n.setGreen(qGray(c));
            n.setBlue(qGray(c));
            QRgb nc=n.rgb();
            img.setPixel(i,j,nc);
        }
    }
    showimage(img);
}

void MainWindow::on_toolButton_reverse_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    img.invertPixels();
    showimage(img);
}

void MainWindow::on_toolButton_add_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();

    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    if(items.size()!=1){
        //maybe show some warning here
        return;
    }
    QListWidgetItem * item=items.back();
    QString nostr=item->text();
    int del=nostr.toInt();
    QImage img2=this->windowMap[del]->img;

    if(img.size()!=img2.size()){
        return;
    }
    for(int i=0;i<img.width();i++){
        for(int j=0;j<img.height();j++){
            QRgb c=img.pixel(i,j);
            QRgb c2=img2.pixel(i,j);
            QColor n;
            n.setRed((qRed(c)+qRed(c2))/2);
            n.setGreen((qGreen(c)+qGreen(c2))/2);
            n.setBlue((qBlue(c)+qBlue(c2))/2);
            QRgb nc=n.rgb();
            img.setPixel(i,j,nc);
        }
    }

    showimage(img);
}

void MainWindow::on_toolButton_he_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    if(img.format()==QImage::Format_Grayscale8){
        //灰度图
        int ** value;
        value=(int **)malloc(sizeof(int*)*img.width());
        for(int i=0;i<img.width();i++){
            value[i]=(int*)malloc(sizeof(int)*img.height());
            for(int j=0;j<img.height();j++){

                QRgb c=img.pixel(i,j);
                value[i][j]=qGray(c);
            }
        }
        int width=img.width();
        int height=img.height();
        //this->histogramEqualization(value,img.width(),img.height());
        long counts[256];
        memset(counts,0,sizeof(int)*256);
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                counts[value[i][j]]++;
            }
        }
        QVector<QRgb> colors;

        int table[256];
        table[0]=255*counts[0]/width/height;
        colors.append(qRgb(table[0],table[0],table[0]));
        for(int i=1;i<256;i++){

            counts[i]=counts[i]+counts[i-1];
            table[i]=255*counts[i]/width/height;
            colors.append(qRgb(table[i],table[i],table[i]));
        }

        img=img.convertToFormat(QImage::Format_Grayscale8,colors);
    }else{
        int ** value;
        value=(int **)malloc(sizeof(int*)*img.width());
        for(int i=0;i<img.width();i++){
            value[i]=(int*)malloc(sizeof(int)*img.height());
            for(int j=0;j<img.height();j++){

                QRgb c=img.pixel(i,j);

                QColor co(c);

                value[i][j]=co.value();
            }
        }
        int width=img.width();
        int height=img.height();
        long counts[256];
        memset(counts,0,sizeof(int)*256);
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                counts[value[i][j]]++;
            }
        }
        int table[256];
        table[0]=255*counts[0]/width/height;
        for(int i=1;i<256;i++){

            counts[i]=counts[i]+counts[i-1];
            table[i]=255*counts[i]/width/height;
        }

        for(int i=0;i<img.width();i++){

            for(int j=0;j<img.height();j++){

                QRgb c=img.pixel(i,j);

                QColor co(c);
                QColor coNew=QColor::fromHsv(co.hue(),co.saturation(),table[co.value()]);
                img.setPixel(i,j,coNew.rgb());
            }
        }
    }
    showimage(img);
}

void MainWindow::on_ac_saveimage_triggered()
{
    if(!this->inProcessing)
        return;
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("保存图像文件"),
            "/",
            tr("Images (*.bmp *.jpeg *.jpg *.png)"));
    if(fileName!=NULL){
        this->windowMap[current]->img.save(fileName);
    }

}

void MainWindow::on_toolButton_rotate_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img
            =this->windowMap[current]->img.copy();
    QMatrix matrix;

    bool ok = false;
    double text = QInputDialog::getDouble(this,
                      tr( "旋转角度" ),
                      tr( "请输入旋转角度" ),0, -360, 360, 2
                      , &ok);
    if ( ok  ){
        matrix.rotate(text);
        img=img.transformed(matrix);
        showimage(img);

    }
    else
        return;
}

void MainWindow::on_toolButton_scale_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img
            =this->windowMap[current]->img;
    ScaleSizeDialog ssd(this,img.width(),img.height());
    ssd.exec();
}

void MainWindow::rateScale(double rate)
{
    if(!this->inProcessing)
        return;
    QImage img
            =this->windowMap[current]->img.copy();
    int width=img.width()*rate;
    int height=img.height()*rate;
    img=img.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    showimage(img);
}

void MainWindow::freeScale(int w, int h)
{
    if(!this->inProcessing)
        return;
    QImage img
            =this->windowMap[current]->img.copy();

    img=img.scaled(w,h,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    showimage(img);
}

//fft
void MainWindow::on_toolButton_clicked()
{
    //use python numpy
//    if(!this->inProcessing)
//        return;
//    QImage img=this->windowMap[current]->img.copy();
//    string b = "_img_temp.png";
//    stringstream ss;
//    ss << current << b;

//    img.save(ss.str().c_str());
//    QProcess process;
//    process.execute("fft.exe",QStringList()<<"fft"<<ss.str().c_str());
//    stringstream ss2;
//    ss2<< current << b <<"_fft.png";
//    QImage imgn(ss2.str().c_str());
//    showimage(imgn);
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    //int DFT2D(double *src,Complex *dst,int size_w,int size_h);
    //int IDFT2D(Complex *src,Complex *dst,int size_w,int size_h);
    //int FFT2D(double *src,Complex *dst,int size_w,int size_h);
    //int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h);
    int size_w=img.width();
    int size_h=img.height();
    double * src;
    src=new double[size_w*size_h];

    for(int i=0;i<img.width();i++){

        for(int j=0;j<img.height();j++){
            QRgb c=img.pixel(i,j);

            src[i*size_w+j]=qGray(c);
        }
    }
    Complex * dst;
    dst=new Complex[size_w*size_h];
    if(isBase2(size_w)==-1 || isBase2(size_h)==-1)
        DFT2D(src,dst,size_w,size_h);
    else
        FFT2D(src,dst,size_w,size_h);

    double dst_abs[size_w][size_h];//also shift
    double max=0.0;
    for(int i=0;i<img.width();i++){

        for(int j=0;j<img.height();j++){
            Complex t=dst[i*size_w+j];

            dst_abs[(i+size_w/2)%size_w][(j+size_h/2)%size_h]=log(1.0+sqrt(t.real*t.real+t.imagin*t.imagin));
            if (dst_abs[(i+size_w/2)%size_w][(j+size_h/2)%size_h]>max){
                max=dst_abs[(i+size_w/2)%size_w][(j+size_h/2)%size_h];
            }
        }
    }
    int color;
    for(int i=0;i<img.width();i++){

        for(int j=0;j<img.height();j++){

            color=dst_abs[i][j]/max*255;
            QColor c(color,color,color);
            img.setPixel(i,j,c.rgb());
        }
    }
    this->showimage(dst,img);
}
//ifft
void MainWindow::on_toolButton_2_clicked()
{
//    if(!this->inProcessing)
//        return;
//    int c=current;
//    string b = "_img_temp.png";
//    stringstream ss;
//    ss << c << b;

//    QProcess process;
//    process.execute("fft.exe",QStringList()<<"ifft"<<ss.str().c_str());
//    stringstream ss2;
//    ss2<< c << b <<"_ifft.png";
//    QImage imgn(ss2.str().c_str());
//    showimage(imgn);
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    Complex * src=this->windowMap[current]->fft;
    if(src==NULL){
        return;
    }

    int size_w=img.width();
    int size_h=img.height();


    Complex * dst;
    dst=new Complex[size_w*size_h];
    if(isBase2(size_w)==-1 || isBase2(size_h)==-1)
        IDFT2D(src,dst,size_w,size_h);
    else
        IFFT2D(src,dst,size_w,size_h);

    for(int i=0;i<img.width();i++){

        for(int j=0;j<img.height();j++){
            int color=dst[i*size_w+j].real;
            QColor c(color,color,color);
            img.setPixel(i,j,c.rgb());

        }
    }
    this->showimage(img);
}

void MainWindow::on_toolButton_exp_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    bool ok;
    double d = QInputDialog::getDouble(this, tr("指数变换增强"),
                                          tr("γ:"), 1.0, 0, 100, 5, &ok);
    if(!ok){
        return;
    }
    for(int i=0;i<img.width();i++){

        for(int j=0;j<img.height();j++){
            QRgb rgb=img.pixel(i,j);
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            r=255*pow(((double)r)/255.0,d);
            g=255*pow(((double)g)/255.0,d);
            b=255*pow(((double)b)/255.0,d);
            QColor c(r,g,b);
            img.setPixel(i,j,c.rgb());

        }
    }
    this->showimage(img);
}
//中值滤波
void MainWindow::on_toolButton_median_clicked()
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    bool ok;
    int kernelSize = QInputDialog::getInt(this, tr("模板大小(w * w)"),
                                          tr("w:"), 3, 1, 15, 2, &ok);
    if(!ok){
        return;
    }


    for(int x=kernelSize/2; x<img.width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<img.height()-(kernelSize/2); y++){
            int value[kernelSize*kernelSize];
            int count=0;
            QRgb c1=img.pixel(x,y);
            QColor co(c1);

            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    QRgb c=img.pixel(x+i,y+j);
                    QColor co1(c);
                    value[count++]=co1.value();

                }
            }
            sort(value,value+kernelSize*kernelSize);
            QColor coNew=QColor::fromHsv(co.hue(),co.saturation(),value[kernelSize*kernelSize/2]);
            img.setPixel(x,y, coNew.rgb());

        }
    }

    this->showimage(img);
}

void MainWindow::on_toolButton_edge_clicked()
{
    ui->menu->exec(QCursor::pos());
}

void MainWindow::on_actionRoberts_triggered()
{
    //Roberts
    int kernel_a[3][3]={{1,0,0},
                        {0,-1,0},
                        {0,0,0}};
    int kernel_b[3][3]={{0,0,1},
                        {0,-1,0},
                        {0,0,0}};

    edge_detection(kernel_a,kernel_b);
}

void MainWindow::edge_detection(int kernel_a[][3], int kernel_b[][3])
{
    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    bool ok=true;
    int kernelSize = 3;
    if(!ok){
        return;
    }


    int result_a=0;
    int result_b=0;
    int value=0;
    QColor co;
    for(int x=kernelSize/2; x<img.width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<img.height()-(kernelSize/2); y++){
            result_a=0;
            result_b=0;
            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    co=QColor(this->windowMap[current]->img.pixel(x+i,y+j));

                    value=co.value();
                    result_a+=value*kernel_a[i+1][j+1];
                    result_b+=value*kernel_b[i+1][j+1];
                }
            }

            int result=qBound(0,(abs(result_a)+abs(result_b)),255);

            img.setPixel(x,y, qRgb(result,result,result));

        }
    }

    this->showimage(img);
}

void MainWindow::on_actionSobel_triggered()
{
    int kernel_a[3][3]={{-1,-2,-1},
                        {0,0,0},
                        {1,2,1}};
    int kernel_b[3][3]={{-1,0,1},
                        {-2,0,2},
                        {-1,0,1}};

    edge_detection(kernel_a,kernel_b);
}

void MainWindow::on_actionPrewitt_triggered()
{
    int kernel_a[3][3]={{-1,-1,-1},
                        {0,0,0},
                        {1,1,1}};
    int kernel_b[3][3]={{1,0,-1},
                        {1,0,-1},
                        {1,0,-1}};

    edge_detection(kernel_a,kernel_b);
}

void MainWindow::on_actionLaplace_triggered()
{
    int kernel_a[3][3]={{0,-1,0},
                        {-1,4,-1},
                        {0,-1,0}};
    int kernel_b[3][3]={{0,-1,0},
                       {-1,4,-1},
                       {0,-1,0}};

    edge_detection(kernel_a,kernel_b);
}

void MainWindow::on_toolButton_ruihua_clicked()
{
    int kernel_a[3][3]={{0,-1,0},
                        {-1,4,-1},
                        {0,-1,0}};

    if(!this->inProcessing)
        return;
    QImage img=this->windowMap[current]->img.copy();
    bool ok=true;
    int kernelSize = 3;
    if(!ok){
        return;
    }


    int result_a=0;
    int value=0;
    QColor co;
    int r;
    int g;
    int b;
    for(int x=kernelSize/2; x<img.width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<img.height()-(kernelSize/2); y++){
            result_a=0;
            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    co=QColor(this->windowMap[current]->img.pixel(x+i,y+j));

                    value=co.value();
                    result_a+=value*kernel_a[i+1][j+1];
                    if(i==0&&j==0){
                        r=co.red();
                        g=co.green();
                        b=co.blue();
                    }
                }
            }

            int result=result_a;

            img.setPixel(x,y, qRgb(qBound(0,result+r,255),qBound(0,result+g,255),qBound(0,result+b,255)));

        }
    }

    this->showimage(img);
}
