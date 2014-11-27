#include "progressdialog.h"
#include "ui_progressdialog.h"
#include <math.h>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    canClose = false;
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::increase()
{
    int value = ui->progressBar->value();
    value++;
    ui->progressBar->setValue(value);
    this->setWindowTitle(ui->progressBar->text() + copyString);
}

void ProgressDialog::resetProgressBar(int maxFiles)
{
    canClose = false;
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(maxFiles);
    this->setWindowTitle(ui->progressBar->text() + copyString);
}

void ProgressDialog::closeEvent(QCloseEvent * event)
{
    if (canClose == false)
        event->ignore();
    else
        event->accept();
}

void ProgressDialog::setCanClose(bool condition)
{
    this->canClose = condition;
}
