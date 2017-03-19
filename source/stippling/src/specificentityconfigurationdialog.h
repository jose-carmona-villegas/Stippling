/**
 * @file specificentityconfigurationdialog.h
 * @brief SpecificEntityConfigurationDialog header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 5/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef SPECIFICENTITYCONFIGURATIONDIALOG_H
#define SPECIFICENTITYCONFIGURATIONDIALOG_H

#include "specificentityconfiguration.h"
#include <util.h>

#include <QDialog>

namespace Ui
{
    class SpecificEntityConfigurationDialog;
}

class SpecificEntityConfigurationDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::SpecificEntityConfigurationDialog *ui; /**< The user interface. Holds references to the objects belonging to this window. */

    SpecificEntityConfiguration * _externalConfiguration; /**< Reference to the configuration being shown/interacted with. */
    SpecificEntityConfiguration * _configuration; /**< Reference to the local copy of the configuration being shown/interacted with. */

private slots:
    void setPercentageSilhouetteDispersion();
    void setPercentageInternalGeneration();
    void setEdgeDetectionMethod();

    void applyChangesOnConfiguration();

public:
    explicit SpecificEntityConfigurationDialog(QWidget *parent = 0);

    ~SpecificEntityConfigurationDialog();

    /**
     * @brief Loads a configuration and displays it.
     * @param configuration Reference to the configuration that will be displayed.
     */
    void loadConfiguration(SpecificEntityConfiguration * configuration);

    void accept();
};

#endif // SPECIFICENTITYCONFIGURATIONDIALOG_H
