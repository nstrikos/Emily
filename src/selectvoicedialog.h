#ifndef SELECTVOICEDIALOG_H
#define SELECTVOICEDIALOG_H

#include <QDialog>

namespace Ui {
class SelectVoiceDialog;
}

class SelectVoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectVoiceDialog(QWidget *parent = 0);
    ~SelectVoiceDialog();
    QString getSelectedVoice();
    void initCombobox(QString voice);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SelectVoiceDialog *ui;

    void resetCombobox();

    QString selectedVoice;
    QString currentSelection;
};

#endif // SELECTVOICEDIALOG_H
