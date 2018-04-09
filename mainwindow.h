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
typedef unsigned char uchar;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ac_openfile_triggered();

    void on_ac_showimage_triggered();

private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
};

#endif // MAINWINDOW_H
