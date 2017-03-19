/**
 * @file mainwindow.h
 * @brief MainWindow header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <camera.h>
#include <util.h>
#include <focallengthnotfounddialog.h>
#include <configuration.h>
#include <configurationdialog.h>
#include <entitytreecontroller.h>
#include "glwidgetstippling.h"
#include "dotgenerationworker.h"

#include <QMainWindow>
#include <QProcess>
#include <QRegExp>
#include <QDomElement>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QActionGroup>

#include <QSplitter>
#include <QFileSystemModel>
#include <QListView>
#include <QStandardItemModel>
#include <QComboBox>
#include <QTreeWidgetItem>

#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "enums.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    int maxWidth_glWidget;
    int maxHeight_glWidget;

    float aspectRatio;

    QVector<Camera> cameras;
    Camera activeCamera;

    QActionGroup * camerasGroup;
    QActionGroup * entitiesRenderModeActionGroup;
    QActionGroup * floorRenderModeActionGroup;

    float _focalLength;
    float _fov;
    FocalLengthNotFoundDialog * flNotFound;

    cv::Mat image;

    Configuration _configuration;
    ConfigurationDialog * configDialog;

    EntityTreeController entities;


    enum Mode {MODE_3DENGINE, MODE_STIPPLING};
    Mode _mode;


    void loadCameraSpecifications();
    QString getTagWord(QString cmd, QStringList args, int word);
    QString getTagFull(QString cmd, QStringList args, int word);
    void loadImageStage3();
    void loadImageStage4();
    void setEntitiesRenderMode(EntitiesRenderMode entitiesRenderMode);
    void setFloorRenderMode(FloorRenderMode floorRenderMode);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loadImageStage2();
    void updateConfiguration();
    void togglePanelCameraControls();
    void updateCameraControlsPanelMenu();

private slots:
    // File menu
    void loadImage();
    void importCSGTree();
    void exportCSGTree();
    void quit();

    // Edit menu
    void openConfigurationDialog();

    // View menu
    void resetViewingTransformations();
    void setEntitiesRenderWireframe();
    void setEntitiesRenderSolid();
    void setEntitiesRenderIlluminatedSolid();
    void setFloorRenderHidden();
    void setFloorRenderGrid();
    void setFloorRenderSolid();
    void setFloorRenderGridSolid();

    // Top bar
    void selectionTool();
    void addOperation();
    void addCylinder();
    void addPrism();

    void setActiveCamera();

    void updateCamera();

    void startStipplingProcessFloydSteinbergDithering();
    void startStipplingProcessStuckiDithering();

    void reApplyStipplingDotDispersion();

    void generateFinalImage();

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

private:
    void resizeOpenGLContainer();
    void startStipplingProcess(DotGenerationWorker::DitheringMethod method);
};

#endif // MAINWINDOW_H
