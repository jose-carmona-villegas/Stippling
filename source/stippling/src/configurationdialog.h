/**
 * @file configurationdialog.h
 * @brief ConfigurationDialog header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 18/December/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H


#include <configuration.h>
#include <util.h>

#include <QDialog>
#include <QColorDialog>

namespace Ui
{
    class ConfigurationDialog;
}

/**
 * @brief ConfigurationDialog class.
 * Represents a specific window, designed to show and allow modifications of the configuration.
 */
class ConfigurationDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ConfigurationDialog *ui; /**< The user interface. Holds references to the objects belonging to this window. */

    Configuration * _externalConfiguration; /**< Reference to the configuration being shown/interacted with. */
    Configuration * _configuration;  /**< Reference to the local copy of the configuration being shown/interacted with. */

    /**
     * @brief Returns the negative of the color provided.
     * @param c Color whose negative is to be obtained.
     * @return The negative of the color provided.
     */
    QColor negative(QColor c);

    /**
     * @brief Returns a Style Sheet string that sets the text and background colors to the given ones.
     * @param background Background color to be assigned.
     * @param text Text color to be assigned.
     * @return Style Sheet string that sets the text and background colors to the given ones.
     */
    QString toStyleString(QColor background, QColor text);

private slots:
    /**
     * @brief Validates and sets (if valid) the color from the button that holds the entity wires color.
     */
    void setColorButtonEntityWires();
    /**
     * @brief Validates and sets (if valid) the color from the button that holds the entity position control point color.
     */
    void setColorButtonEntityPositionCP();
    /**
     * @brief Validates and sets (if valid) the color from the button that holds the entity resize control point color.
     */
    void setColorButtonEntityResizeCP();
    /**
     * @brief Validates and sets (if valid) the color from the button that holds the entity rotation control point color.
     */
    void setColorButtonEntityRotateCP();
    /**
     * @brief Validates and sets (if valid) the color from the button that holds the entity scale control point color.
     */
    void setColorButtonEntityScaleCP();

    /**
     * @brief Validates and sets (if valid) the color from the button that holds the floor wire color.
     */
    void setColorButtonFloorWires();

    /**
     * @brief Validates and sets (if valid) the Packing Factor from the QSpinBox that holds it.
     */
    void setPackingFactor();

    /**
     * @brief Validates and sets (if valid) the RNG seed from the QSpinBox that holds it.
     */
    void setRngSeed();

    /**
     * @brief Validates and sets (if valid) the Stipple dot dispersion from the QSpinBox that holds it.
     */
    void setStippleDotDispersion();

    /**
     * @brief Validates and sets (if valid) the unmodelled stippling percentage from the QSpinBox that holds it.
     */
    void setUnmodelledStipplingChance();

    /**
     * @brief Validates and sets (if valid) the modelled stippling percentage from the QSpinBox that holds it.
     */
    void setModelledStipplingChance();

    /**
     * @brief Sets the chosen edge detection method from the QComboBox that holds it.
     */
    void setEdgeDetectionMethod();

    void setUseTileRendering();
    void setTileWidth();
    void setTileHeight();

    void applyChangesOnConfiguration();

public:
    /**
     * @brief Default constructor.
     * Initializes the dialog and its UI. If a parent it's provided, the dialog is added to the parent's children.
     * @param parent Window parent. Used by Qt to free resources when a the owner is freed.
     */
    explicit ConfigurationDialog(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~ConfigurationDialog();

    /**
     * @brief Loads a configuration and displays it.
     * @param configuration Reference to the configuration that will be displayed.
     */
    void loadConfiguration(Configuration * configuration);

    void accept();
};

#endif // CONFIGURATIONDIALOG_H
