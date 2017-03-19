/**
 * @file configurationdialog.cpp
 * @brief ConfigurationDialog source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 18/December/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "configurationdialog.h"
#include "ui_configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);

    connect(ui->colorButtonEntityWires, SIGNAL(released()), this, SLOT(setColorButtonEntityWires()));
    connect(ui->colorButtonEntityPositionCP, SIGNAL(released()), this, SLOT(setColorButtonEntityPositionCP()));
    connect(ui->colorButtonEntityResizeCP, SIGNAL(released()), this, SLOT(setColorButtonEntityResizeCP()));
    connect(ui->colorButtonEntityRotateCP, SIGNAL(released()), this, SLOT(setColorButtonEntityRotateCP()));
    connect(ui->colorButtonEntityScaleCP, SIGNAL(released()), this, SLOT(setColorButtonEntityScaleCP()));

    connect(ui->colorButtonFloorWires, SIGNAL(released()), this, SLOT(setColorButtonFloorWires()));

    connect(ui->packingFactor, SIGNAL(valueChanged(int)), this, SLOT(setPackingFactor()));

    connect(ui->rngSeed, SIGNAL(valueChanged(int)), this, SLOT(setRngSeed()));
    connect(ui->stippleDotDispersion, SIGNAL(valueChanged(int)), this, SLOT(setStippleDotDispersion()));

    connect(ui->unmodeledStipplingChance, SIGNAL(valueChanged(int)), this, SLOT(setUnmodelledStipplingChance()));
    connect(ui->modeledStipplingChance, SIGNAL(valueChanged(int)), this, SLOT(setModelledStipplingChance()));

    ui->edgeDetectionMethod->addItem("Sobel", "Sobel");
    ui->edgeDetectionMethod->addItem("Canny", "Canny");
    connect(ui->edgeDetectionMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(setEdgeDetectionMethod()));

    connect(ui->useTileRendering, SIGNAL(toggled(bool)), this, SLOT(setUseTileRendering()));
    connect(ui->tileWidth, SIGNAL(valueChanged(int)), this, SLOT(setTileWidth()));
    connect(ui->tileHeight, SIGNAL(valueChanged(int)), this, SLOT(setTileHeight()));
}

ConfigurationDialog::~ConfigurationDialog()
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

QColor ConfigurationDialog::negative(QColor c)
{
    return QColor(255-c.red(), 255-c.green(), 255-c.blue());
}

QString ConfigurationDialog::toStyleString(QColor background, QColor text)
{
    QString style = "";

    QString bgR = QString::number(background.red());
    QString bgG = QString::number(background.green());
    QString bgB = QString::number(background.blue());
    QString bgA = QString::number(background.alpha());

    QString txR = QString::number(text.red());
    QString txG = QString::number(text.green());
    QString txB = QString::number(text.blue());
    QString txA = QString::number(text.alpha());

    style += "background:rgba("+bgR+","+bgG+","+bgB+","+bgA+");";
    style += "color:rgba("+txR+","+txG+","+txB+","+txA+");";

    return style;
}

void ConfigurationDialog::loadConfiguration(Configuration * configuration)
{
    _externalConfiguration = configuration;
    _configuration = new Configuration();



    _configuration->setEntityWires(Util::toQColor(_externalConfiguration->entityWires()));
    _configuration->setPositionCP(Util::toQColor(_externalConfiguration->positionCP()));
    _configuration->setResizeCP(Util::toQColor(_externalConfiguration->resizeCP()));
    _configuration->setRotateCP(Util::toQColor(_externalConfiguration->rotateCP()));
    _configuration->setScaleCP(Util::toQColor(_externalConfiguration->scaleCP()));

    _configuration->setFloorWires(Util::toQColor(_externalConfiguration->floorWires()));

    _configuration->setPackingFactor(_externalConfiguration->packingFactor());
    _configuration->setRngSeed(_externalConfiguration->rngSeed());
    _configuration->setStippleDotDispersion(_externalConfiguration->stippleDotDispersion());
    _configuration->setUnmodelledStipplingChance(_externalConfiguration->unmodelledStipplingChance());
    _configuration->setModelledStipplingChance(_externalConfiguration->modelledStipplingChance());

    _configuration->setEdgeDetectionMethod(_externalConfiguration->edgeDetectionMethod());

    _configuration->setUseTileRendering(_externalConfiguration->useTileRendering());
    _configuration->setTileWidth(_externalConfiguration->tileWidth());
    _configuration->setTileHeight(_externalConfiguration->tileHeight());





    ui->colorButtonEntityWires->setStyleSheet(toStyleString(Util::toQColor(_configuration->entityWires()),
                                                            negative(Util::toQColor(_configuration->entityWires()))));

    ui->colorButtonEntityPositionCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->positionCP()),
                                                            negative(Util::toQColor(_configuration->positionCP()))));

    ui->colorButtonEntityResizeCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->resizeCP()),
                                                            negative(Util::toQColor(_configuration->resizeCP()))));

    ui->colorButtonEntityRotateCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->rotateCP()),
                                                            negative(Util::toQColor(_configuration->rotateCP()))));

    ui->colorButtonEntityScaleCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->scaleCP()),
                                                            negative(Util::toQColor(_configuration->scaleCP()))));

    ui->colorButtonFloorWires->setStyleSheet(toStyleString(Util::toQColor(_configuration->floorWires()),
                                                            negative(Util::toQColor(_configuration->floorWires()))));

    ui->packingFactor->setValue(_configuration->packingFactor());

    ui->rngSeed->setValue(int(_configuration->rngSeed()));

    ui->stippleDotDispersion->setValue(int(_configuration->stippleDotDispersion()));

    ui->unmodeledStipplingChance->setValue(_configuration->unmodelledStipplingChance());
    ui->modeledStipplingChance->setValue(_configuration->modelledStipplingChance());

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

    ui->useTileRendering->setChecked(_configuration->useTileRendering());
    ui->tileWidth->setValue(_configuration->tileWidth());
    ui->tileHeight->setValue(_configuration->tileHeight());
}


void ConfigurationDialog::setColorButtonEntityWires()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setEntityWires(color);
        ui->colorButtonEntityWires->setStyleSheet(toStyleString(Util::toQColor(_configuration->entityWires()),
                                                                negative(Util::toQColor(_configuration->entityWires()))));
    }
}

void ConfigurationDialog::setColorButtonEntityPositionCP()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setPositionCP(color);
        ui->colorButtonEntityPositionCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->positionCP()),
                                                                negative(Util::toQColor(_configuration->positionCP()))));
    }
}


void ConfigurationDialog::setColorButtonEntityResizeCP()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setResizeCP(color);
        ui->colorButtonEntityResizeCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->resizeCP()),
                                                                negative(Util::toQColor(_configuration->resizeCP()))));
    }
}

void ConfigurationDialog::setColorButtonEntityRotateCP()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setRotateCP(color);
        ui->colorButtonEntityRotateCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->rotateCP()),
                                                                negative(Util::toQColor(_configuration->rotateCP()))));
    }
}

void ConfigurationDialog::setColorButtonEntityScaleCP()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setScaleCP(color);
        ui->colorButtonEntityScaleCP->setStyleSheet(toStyleString(Util::toQColor(_configuration->scaleCP()),
                                                                negative(Util::toQColor(_configuration->scaleCP()))));
    }
}

void ConfigurationDialog::setColorButtonFloorWires()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        _configuration->setFloorWires(color);
        ui->colorButtonFloorWires->setStyleSheet(toStyleString(Util::toQColor(_configuration->floorWires()),
                                                                negative(Util::toQColor(_configuration->floorWires()))));
    }
}

void ConfigurationDialog::setPackingFactor()
{
    int packingFactor = ui->packingFactor->text().toInt();

    _configuration->setPackingFactor(packingFactor);
}

void ConfigurationDialog::setRngSeed()
{
    unsigned int rngSeed = ui->rngSeed->text().toUInt();

    _configuration->setRngSeed(rngSeed);
}

void ConfigurationDialog::setStippleDotDispersion()
{
    unsigned int stippleDotDispersion = ui->stippleDotDispersion->text().toUInt();

    _configuration->setStippleDotDispersion(stippleDotDispersion);
}

void ConfigurationDialog::setUnmodelledStipplingChance()
{
    int unmodelledImageToStipple = ui->unmodeledStipplingChance->text().toInt();

    _configuration->setUnmodelledStipplingChance(unmodelledImageToStipple);
}

void ConfigurationDialog::setModelledStipplingChance()
{
    int modelledImageToStipple = ui->modeledStipplingChance->text().toInt();

    _configuration->setModelledStipplingChance(modelledImageToStipple);
}

void ConfigurationDialog::setEdgeDetectionMethod()
{
    QString value = ui->edgeDetectionMethod->currentText();

    EdgeDetectionMethod method = SOBEL;
    if(value == "Sobel")
    {
        method = SOBEL;
    }
    else if(value == "Canny")
    {
        method = CANNY;
    }

    _configuration->setEdgeDetectionMethod(method);
}

void ConfigurationDialog::setUseTileRendering()
{
    _configuration->setUseTileRendering(ui->useTileRendering->isChecked());
}

void ConfigurationDialog::setTileWidth()
{
    int tileWidth = ui->tileWidth->text().toInt();

    _configuration->setTileWidth(tileWidth);
}

void ConfigurationDialog::setTileHeight()
{
    int tileHeight = ui->tileHeight->text().toInt();

    _configuration->setTileHeight(tileHeight);
}

void ConfigurationDialog::applyChangesOnConfiguration()
{
    _externalConfiguration->setEntityWires(Util::toQColor(_configuration->entityWires()));
    _externalConfiguration->setPositionCP(Util::toQColor(_configuration->positionCP()));
    _externalConfiguration->setResizeCP(Util::toQColor(_configuration->resizeCP()));
    _externalConfiguration->setRotateCP(Util::toQColor(_configuration->rotateCP()));
    _externalConfiguration->setScaleCP(Util::toQColor(_configuration->scaleCP()));

    _externalConfiguration->setFloorWires(Util::toQColor(_configuration->floorWires()));

    _externalConfiguration->setPackingFactor(_configuration->packingFactor());
    _externalConfiguration->setRngSeed(_configuration->rngSeed());
    _externalConfiguration->setStippleDotDispersion(_configuration->stippleDotDispersion());
    _externalConfiguration->setUnmodelledStipplingChance(_configuration->unmodelledStipplingChance());
    _externalConfiguration->setModelledStipplingChance(_configuration->modelledStipplingChance());

    _externalConfiguration->setEdgeDetectionMethod(_configuration->edgeDetectionMethod());

    _externalConfiguration->setUseTileRendering(_configuration->useTileRendering());
    _externalConfiguration->setTileWidth(_configuration->tileWidth());
    _externalConfiguration->setTileHeight(_configuration->tileHeight());
}


void ConfigurationDialog::accept()
{
    applyChangesOnConfiguration();

    QDialog::accept();
}

