#include "scalesizedialog.h"
#include "ui_scalesizedialog.h"

ScaleSizeDialog::ScaleSizeDialog(QWidget *parent,int w,int h) :
    QDialog(parent),
    ui(new Ui::ScaleSizeDialog)
{
    ui->setupUi(this);
    ui->heightBox->setValue(h);
    ui->widthBox->setValue(w);
    connect(this,SIGNAL(rateScale(double)),parent,SLOT(rateScale(double)));
    connect(this,SIGNAL(freeScale(int,int)),parent,SLOT(freeScale(int,int)));

}

ScaleSizeDialog::~ScaleSizeDialog()
{
    delete ui;
}



void ScaleSizeDialog::on_pushButton_2_clicked()
{
    double rate=ui->scaleBox->value();
    emit rateScale(rate);
    this->close();
}

void ScaleSizeDialog::on_pushButton_clicked()
{
    int w=ui->widthBox->value();
    int h=ui->heightBox->value();
    emit freeScale(w,h);
    this->close();
}
