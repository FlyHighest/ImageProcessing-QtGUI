#ifndef SCALESIZEDIALOG_H
#define SCALESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ScaleSizeDialog;
}

class ScaleSizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleSizeDialog(QWidget *parent = 0, int w=1, int h=1);
    ~ScaleSizeDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ScaleSizeDialog *ui;

signals:
    void rateScale(double rate);
    void freeScale(int w,int h);
};

#endif // SCALESIZEDIALOG_H
