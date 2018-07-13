#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QMdiArea>
#include <QtDebug>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <imagewindow.h>
#include <QInputDialog>
#include <QProcess>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <QCursor>

#include <scalesizedialog.h>
#include "fourier.h"
using namespace std;
typedef unsigned char uchar;
typedef map<int, ImageWindow *> WMap;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showimage(QImage);
    int current;
    void showimage(Complex*,QImage);
    WMap windowMap;
    bool inProcessing;
    void updateListWidget();
    void histogramEqualization(int ** value,int width,int height);
private slots:
    void on_ac_openfile_triggered();

    void on_ac_showall_triggered();

    void on_ac_setcurrent_triggered();

    void on_ac_delete_triggered();

    void on_toolButton_grey_clicked();

    void on_toolButton_reverse_clicked();

    void on_toolButton_add_clicked();

    void on_toolButton_he_clicked();

    void on_ac_saveimage_triggered();

    void on_toolButton_rotate_clicked();

    void on_toolButton_scale_clicked();

    void rateScale(double rate);

    void freeScale(int w,int h);
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_exp_clicked();

    void on_toolButton_median_clicked();

    void on_toolButton_edge_clicked();

    void on_actionRoberts_triggered();
    void edge_detection(int kernel_a[][3], int kernel_b[][3]);
    void on_actionSobel_triggered();

    void on_actionPrewitt_triggered();

    void on_actionLaplace_triggered();

    void on_toolButton_ruihua_clicked();

private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    void statusBarMessage();
};

#endif // MAINWINDOW_H
