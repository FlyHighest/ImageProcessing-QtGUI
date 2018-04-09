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

#include <map>
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
    WMap windowMap;
private slots:
    void on_ac_openfile_triggered();

    void on_ac_showall_triggered();

private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    QString statusBarMessage();
};

#endif // MAINWINDOW_H
