#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QCloseEvent>

//const QString copyString = " αντιγράφτηκε";

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    void resetProgressBar(int maxFiles);
    void setCanClose(bool condition);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void increase();

private:
    Ui::ProgressDialog *ui;
    bool canClose;
};

#endif // PROGRESSDIALOG_H
