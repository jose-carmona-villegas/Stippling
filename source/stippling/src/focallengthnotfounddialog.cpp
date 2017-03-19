#include "focallengthnotfounddialog.h"
#include "ui_focallengthnotfounddialog.h"

FocalLengthNotFoundDialog::FocalLengthNotFoundDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FocalLengthNotFoundDialog)
{
    ui->setupUi(this);

    ui->radioButtonSetFOV->setChecked(true);

    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);

    validFL = false;
    validSensorX = false;
    validSensorY = false;
    validCropFactor = false;

    ui->lineEditFOV->setText("45");
}

FocalLengthNotFoundDialog::~FocalLengthNotFoundDialog()
{
    if(ui != 0)
    {
        delete ui;
        ui = 0;
    }
}

void FocalLengthNotFoundDialog::setOwner(QWidget * owner)
{
    _owner = owner;
}

float FocalLengthNotFoundDialog::getFOV() const
{
    return fov;
}

Camera FocalLengthNotFoundDialog::getCamera() const
{
    if(autocalculationMode)
    {
        return Camera("Custom camera", sensorX, sensorY, cropFactor);
    }
    else
    {
        return Camera(fov);
    }

}

float FocalLengthNotFoundDialog::calculateFOV()
{
    return 2.0f * atan(sensorX / (2.0f * focalLength * cropFactor)) * 180.0f / PI;
}

void FocalLengthNotFoundDialog::on_lineEditFocalLength_textChanged(const QString &arg1)
{
    float temp = arg1.toFloat(&validFL);

    if(validFL)
    {
        focalLength = temp;
    }

    if(validFL && validSensorX && validSensorY && validCropFactor)
    {
        ui->lineEditFOV->setText(QString::number(calculateFOV()));
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
    }
    else
    {
        ui->lineEditFOV->setText("");
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
    }
}

void FocalLengthNotFoundDialog::on_lineEditSensorX_textChanged(const QString &arg1)
{
    float temp = arg1.toFloat(&validSensorX);

    if(validSensorX)
    {
        sensorX = temp;
    }

    if(validFL && validSensorX && validSensorY && validCropFactor)
    {
        ui->lineEditFOV->setText(QString::number(calculateFOV()));
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
    }
    else
    {
        ui->lineEditFOV->setText("");
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
    }
}

void FocalLengthNotFoundDialog::on_lineEditSensorY_textChanged(const QString &arg1)
{
    float temp = arg1.toFloat(&validSensorY);

    if(validSensorY)
    {
        sensorY = temp;
    }

    if(validFL && validSensorX && validSensorY && validCropFactor)
    {
        ui->lineEditFOV->setText(QString::number(calculateFOV()));
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
    }
    else
    {
        ui->lineEditFOV->setText("");
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
    }
}

void FocalLengthNotFoundDialog::on_lineEditCropFactor_textChanged(const QString &arg1)
{
    float temp = arg1.toFloat(&validCropFactor);

    if(validCropFactor)
    {
        cropFactor = temp;
    }

    if(validFL && validSensorX && validSensorY && validCropFactor)
    {
        ui->lineEditFOV->setText(QString::number(calculateFOV()));
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
    }
    else
    {
        ui->lineEditFOV->setText("");
        ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
    }
}

void FocalLengthNotFoundDialog::on_lineEditFOV_textChanged(const QString &arg1)
{
    if(!autocalculationMode)
    {
        float temp = arg1.toFloat(&validFOV);

        if(validFOV)
        {
            fov = temp;
            ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(true);
        }
        else
        {
            ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled(false);
        }
    }
}

void FocalLengthNotFoundDialog::on_buttonBox_accepted()
{
    if(autocalculationMode)
    {
        fov = calculateFOV();
    }
}

void FocalLengthNotFoundDialog::setFovEditionChoice()
{
    // Only two options, no need to search for the checked one...
    if(ui->radioButtonAutocalculateFOV->isChecked())
    {
        // Autocalculation mode
        autocalculationMode = true;
        ui->lineEditFocalLength->setEnabled(true);
        ui->lineEditSensorX->setEnabled(true);
        ui->lineEditSensorY->setEnabled(true);
        ui->lineEditCropFactor->setEnabled(true);
        ui->lineEditFOV->setEnabled(false);
        // Clean the unused fields
        ui->lineEditFOV->setText("");
    }
    else
    {
        // Direct setting mode
        autocalculationMode = false;
        ui->lineEditFocalLength->setEnabled(false);
        ui->lineEditSensorX->setEnabled(false);
        ui->lineEditSensorY->setEnabled(false);
        ui->lineEditCropFactor->setEnabled(false);
        ui->lineEditFOV->setEnabled(true);
        // Clean the unused fields
        ui->lineEditFocalLength->setText("");
        ui->lineEditSensorX->setText("");
        ui->lineEditSensorY->setText("");
        ui->lineEditCropFactor->setText("");
    }
}

void FocalLengthNotFoundDialog::on_radioButtonAutocalculateFOV_toggled(bool checked)
{
    setFovEditionChoice();
}

void FocalLengthNotFoundDialog::on_radioButtonSetFOV_toggled(bool checked)
{
    setFovEditionChoice();
}
