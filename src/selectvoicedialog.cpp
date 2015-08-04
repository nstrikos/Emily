#include "selectvoicedialog.h"
#include "ui_selectvoicedialog.h"

#include "constants.h"

SelectVoiceDialog::SelectVoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectVoiceDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem(spikeVoiceDisplay);
    ui->comboBox->addItem(prudenceVoiceDisplay);
    ui->comboBox->addItem(poppyVoiceDisplay);
    ui->comboBox->addItem(obadiahVoiceDisplay);
    ui->comboBox->addItem(rmsVoiceDisplay);
    ui->comboBox->addItem(bdlVoiceDisplay);
    ui->comboBox->addItem(herculesVoiceDisplay);
    ui->comboBox->addItem(googleVoiceDisplay);
    ui->comboBox->addItem(emilyVoiceDisplay);
    ui->comboBox->addItem(pavoqueVoiceDisplay);
    ui->comboBox->addItem(pierreVoiceDisplay);
    ui->comboBox->addItem(luciaVoiceDisplay);
    ui->comboBox->addItem(turkishVoiceDisplay);
    ui->comboBox->addItem(teluguVoiceDisplay);
}

SelectVoiceDialog::~SelectVoiceDialog()
{
    delete ui;
}

QString SelectVoiceDialog::getSelectedVoice()
{
    return selectedVoice;
}

void SelectVoiceDialog::on_buttonBox_accepted()
{
    currentSelection = ui->comboBox->currentText();

    if (currentSelection == spikeVoiceDisplay)
        selectedVoice = spikeVoice;
    else if (currentSelection == prudenceVoiceDisplay)
        selectedVoice = prudenceVoice;
    else if (currentSelection == poppyVoiceDisplay)
        selectedVoice = poppyVoice;
    else if (currentSelection == obadiahVoiceDisplay)
        selectedVoice = obadiahVoice;
    else if (currentSelection == rmsVoiceDisplay)
        selectedVoice = rmsVoice;
    else if (currentSelection == bdlVoiceDisplay)
        selectedVoice = bdlVoice;
    else if (currentSelection == emilyVoiceDisplay)
        selectedVoice = emilyVoice;
    else if (currentSelection == googleVoiceDisplay)
        selectedVoice = googleVoice;
    else if (currentSelection == herculesVoiceDisplay)
        selectedVoice = herculesVoice;
    else if (currentSelection == pavoqueVoiceDisplay)
        selectedVoice = pavoqueVoice;
    else if (currentSelection == pierreVoiceDisplay)
        selectedVoice = pierreVoice;
    else if (currentSelection == luciaVoiceDisplay)
        selectedVoice = luciaVoice;
    else if (currentSelection == turkishVoiceDisplay)
        selectedVoice = turkishVoice;
    else if (currentSelection == teluguVoiceDisplay)
        selectedVoice = teluguVoice;
}

void SelectVoiceDialog::on_buttonBox_rejected()
{
    resetCombobox();
}

void SelectVoiceDialog::resetCombobox()
{
    for (int i = 0; i < ui->comboBox->count(); i++ )
    {
        if (currentSelection == ui->comboBox->itemText(i))
        {
            ui->comboBox->setCurrentIndex(i);
            break;
        }
    }
}

void SelectVoiceDialog::initCombobox(QString voice)
{
    selectedVoice = voice;

    if (selectedVoice == spikeVoice)
        currentSelection = spikeVoiceDisplay;
    else if (selectedVoice == prudenceVoice)
        currentSelection = prudenceVoiceDisplay;
    else if (selectedVoice == poppyVoice)
        currentSelection = poppyVoiceDisplay;
    else if (selectedVoice == obadiahVoice)
        currentSelection = obadiahVoiceDisplay;
    else if (selectedVoice == rmsVoice)
        currentSelection = rmsVoiceDisplay;
    else if (selectedVoice == bdlVoice)
        currentSelection = bdlVoiceDisplay;
    else if (selectedVoice == emilyVoice)
        currentSelection = emilyVoiceDisplay;
    else if (selectedVoice == googleVoice)
        currentSelection = googleVoiceDisplay;
    else if (selectedVoice == herculesVoice)
        currentSelection = herculesVoiceDisplay;
    else if (selectedVoice == pavoqueVoice)
        currentSelection = pavoqueVoiceDisplay;
    else if (selectedVoice == pierreVoice)
        currentSelection = pierreVoiceDisplay;
    else if (selectedVoice == luciaVoice)
        currentSelection = luciaVoiceDisplay;
    else if (selectedVoice == turkishVoice)
        currentSelection = turkishVoiceDisplay;
    else if (selectedVoice == teluguVoice)
        currentSelection = teluguVoiceDisplay;

    resetCombobox();
}
