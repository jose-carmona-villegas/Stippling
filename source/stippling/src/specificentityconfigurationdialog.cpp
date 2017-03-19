/**
 * @file specificentityconfigurationdialog.cpp
 * @brief SpecificEntityConfigurationDialog source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 5/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "specificentityconfigurationdialog.h"
#include "ui_specificentityconfigurationdialog.h"



void SpecificEntityConfigurationDialog::setPercentageSilhouetteDispersion()
{
    int percentageSilhouetteDispersion = ui->percentageSilhouetteDispersion->text().toInt();

    _configuration->setPercentageSilhouetteDispersion(percentageSilhouetteDispersion);
}

void SpecificEntityConfigurationDialog::setPercentageInternalGeneration()
{
    int percentageInternalGeneration = ui->percentageInternalGeneration->text().toInt();

    _configuration->setPercentageInternalGeneration(percentageInternalGeneration);
}

void SpecificEntityConfigurationDialog::setEdgeDetectionMethod()
{
    QString value = ui->edgeDetectionMethod->currentText();

    bool hasSpecificEdgeDetectionMethod = false;
    EdgeDetectionMethod method = SOBEL;
    if(value == "Same as general configuration")
    {
        hasSpecificEdgeDetectionMethod = false;
        method = SOBEL;
    }
    else if(value == "Sobel")
    {
        hasSpecificEdgeDetectionMethod = true;
        method = SOBEL;
    }
    else if(value == "Canny")
    {
        hasSpecificEdgeDetectionMethod = true;
        method = CANNY;
    }

    _configuration->setHasSpecificEdgeDetectionMethod(hasSpecificEdgeDetectionMethod);
    _configuration->setEdgeDetectionMethod(method);
}





void SpecificEntityConfigurationDialog::applyChangesOnConfiguration()
{
    // Changes accepted, update the external configuration
    _externalConfiguration->setPercentageSilhouetteDispersion(_configuration->percentageSilhouetteDispersion());
    _externalConfiguration->setPercentageInternalGeneration(_configuration->percentageInternalGeneration());
    _externalConfiguration->setHasSpecificEdgeDetectionMethod(_configuration->hasSpecificEdgeDetectionMethod());
    _externalConfiguration->setEdgeDetectionMethod(_configuration->edgeDetectionMethod());
}







SpecificEntityConfigurationDialog::SpecificEntityConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpecificEntityConfigurationDialog)
{
    ui->setupUi(this);

    ui->edgeDetectionMethod->addItem("Same as general configuration", "Same as general configuration");
    ui->edgeDetectionMethod->addItem("Sobel", "Sobel");
    ui->edgeDetectionMethod->addItem("Canny", "Canny");

    connect(ui->percentageSilhouetteDispersion, SIGNAL(valueChanged(int)), this, SLOT(setPercentageSilhouetteDispersion()));
    connect(ui->percentageInternalGeneration, SIGNAL(valueChanged(int)), this, SLOT(setPercentageInternalGeneration()));
    connect(ui->edgeDetectionMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(setEdgeDetectionMethod()));
}

SpecificEntityConfigurationDialog::~SpecificEntityConfigurationDialog()
{
    if(ui != 0)
    {
        delete ui;
        ui = 0;
    }

    if(_configuration != 0)
    {
        delete _configuration;
        _configuration = 0;
    }
}

void SpecificEntityConfigurationDialog::loadConfiguration(SpecificEntityConfiguration * configuration)
{
    _externalConfiguration = configuration;
    _configuration = new SpecificEntityConfiguration();

    // Load the passed configuration onto the internal (temporary) configuration.
    _configuration->setPercentageSilhouetteDispersion(_externalConfiguration->percentageSilhouetteDispersion());
    _configuration->setPercentageInternalGeneration(_externalConfiguration->percentageInternalGeneration());
    _configuration->setHasSpecificEdgeDetectionMethod(_externalConfiguration->hasSpecificEdgeDetectionMethod());
    _configuration->setEdgeDetectionMethod(_externalConfiguration->edgeDetectionMethod());

    ui->percentageSilhouetteDispersion->setValue(_configuration->percentageSilhouetteDispersion());
    ui->percentageInternalGeneration->setValue(_configuration->percentageInternalGeneration());
    if(_configuration->hasSpecificEdgeDetectionMethod())
    {
        QString index = "Sobel";
        if(_configuration->edgeDetectionMethod() == SOBEL)
        {
            index = "Sobel";
        }
        else if(_configuration->edgeDetectionMethod() ==  CANNY)
        {
            index = "Canny";
        }
        ui->edgeDetectionMethod->setCurrentIndex(ui->edgeDetectionMethod->findText(index));
    }
    else
    {
        QString index = "Same as general configuration";
        ui->edgeDetectionMethod->setCurrentIndex(ui->edgeDetectionMethod->findText(index));
    }
}

void SpecificEntityConfigurationDialog::accept()
{
    applyChangesOnConfiguration();

    QDialog::accept();
}

