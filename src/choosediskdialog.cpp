#include "choosediskdialog.h"
#include "ui_choosediskdialog.h"

ChooseDiskDialog::ChooseDiskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDiskDialog)
{
    ui->setupUi(this);
    searchDrivesAndAddtoCombobox();
}

ChooseDiskDialog::~ChooseDiskDialog()
{
    delete ui;
}

QString ChooseDiskDialog::getDrivePath()
{
    return drivePath;
}

bool ChooseDiskDialog::searchDrivesAndAddtoCombobox()
{
    //This is the main method of the class
    //clears the combobox
    //searches for available drives and add them to combobox
    //if there are any available drives returns true

    ui->comboBox->clear();
    QDir dir;

    //We start at i=1 to avoid choosing drive C
    for (int i = 1; i < dir.drives().size(); i++)
    {
        QFileInfo item = dir.drives().at(i);
        ui->comboBox->addItem(diskString + item.canonicalFilePath());
    }
    if (ui->comboBox->count() == 0)
    {
        return false;
    }
    else
        return true;
}

void ChooseDiskDialog::on_okButton_clicked()
{
    drivePath = ui->comboBox->currentText();
    drivePath.replace(diskString, "");
    this->accept();
}

void ChooseDiskDialog::on_cancelButton_clicked()
{
    this->close();
}
