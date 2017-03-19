#ifndef FOCALLENGTHNOTFOUNDDIALOG_H
#define FOCALLENGTHNOTFOUNDDIALOG_H

#include <util.h>
#include <camera.h>

#include <QDialog>
#include <QPushButton>

#include <QtCore/qmath.h>

namespace Ui
{
    class FocalLengthNotFoundDialog;
}

class FocalLengthNotFoundDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FocalLengthNotFoundDialog(QWidget *parent = 0);
    ~FocalLengthNotFoundDialog();
    void setOwner(QWidget * owner);
    float getFOV() const;
    Camera getCamera() const;

private slots:
    void on_lineEditFocalLength_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_lineEditSensorX_textChanged(const QString &arg1);

    void on_lineEditSensorY_textChanged(const QString &arg1);

    void on_lineEditCropFactor_textChanged(const QString &arg1);

    void on_radioButtonAutocalculateFOV_toggled(bool checked);

    void on_radioButtonSetFOV_toggled(bool checked);

    void on_lineEditFOV_textChanged(const QString &arg1);

private:
    float calculateFOV();

    void setFovEditionChoice();

    Ui::FocalLengthNotFoundDialog *ui;

    QWidget * _owner;

    float focalLength;
    float sensorX;
    float sensorY;
    float cropFactor;
    float fov;


    bool validFL;
    bool validSensorX;
    bool validSensorY;
    bool validCropFactor;
    bool validFOV;

    bool autocalculationMode;
};

#endif // FOCALLENGTHNOTFOUNDDIALOG_H
