#ifndef CHOOSEDISKDIALOG_H
#define CHOOSEDISKDIALOG_H

#include <QDialog>
#include <QDir>

const QString diskString = "Δίσκος ";

namespace Ui {
class ChooseDiskDialog;
}

class ChooseDiskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseDiskDialog(QWidget *parent = 0);
    ~ChooseDiskDialog();
    QString getDrivePath();
    bool searchDrivesAndAddtoCombobox();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ChooseDiskDialog *ui;
    QString drivePath;
};

#endif // CHOOSEDISKDIALOG_H
