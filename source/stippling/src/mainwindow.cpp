/**
 * @file mainwindow.cpp
 * @brief MainWindow class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _mode = MODE_3DENGINE;

    _configuration = Configuration();
    ui->openGLViewport->setConfiguration(&_configuration);
    ui->stippling_openGLViewport->setConfiguration(&_configuration);
    ui->superiorView_openGLViewport->setConfiguration(&_configuration);

    ui->openGLViewport->setEntityTreeController(&entities);
    ui->treeWidget_Entities->setEntityTreeController(&entities);
    ui->stippling_openGLViewport->setEntityTreeController(&entities);
    ui->superiorView_openGLViewport->setEntityTreeController(&entities);

    maxWidth_glWidget = 800;
    maxHeight_glWidget = 600;

    aspectRatio = maxWidth_glWidget / (float) maxHeight_glWidget;

    ui->openGLViewport->resize(maxWidth_glWidget, maxHeight_glWidget);
    ui->openGLContainer->resize(maxWidth_glWidget + 60, maxHeight_glWidget + 60);

    activeCamera = Camera("Default (Canon EOS 600D)", 22.3f, 14.9f, 1.6f);
    cameras = QVector<Camera>();

    loadCameraSpecifications();

    // File menu
    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(loadImage()));
    connect(ui->actionImport_CSG_Tree, SIGNAL(triggered()), this, SLOT(importCSGTree()));
    connect(ui->actionExport_CSG_Tree, SIGNAL(triggered()), this, SLOT(exportCSGTree()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
    // Edit menu
    connect(ui->actionConfiguration, SIGNAL(triggered()), this, SLOT(openConfigurationDialog()));
    // View menu
    connect(ui->actionReset_viewing_transformations, SIGNAL(triggered()), this, SLOT(resetViewingTransformations()));

    connect(ui->actionHiddenFloor, SIGNAL(triggered()), this, SLOT(setFloorRenderHidden()));
    connect(ui->actionGrid_only, SIGNAL(triggered()), this, SLOT(setFloorRenderGrid()));
    connect(ui->actionSolid_only, SIGNAL(triggered()), this, SLOT(setFloorRenderSolid()));
    connect(ui->actionGrid_and_solid, SIGNAL(triggered()), this, SLOT(setFloorRenderGridSolid()));

    connect(ui->actionAs_Wireframe, SIGNAL(triggered()), this, SLOT(setEntitiesRenderWireframe()));
    connect(ui->actionAs_Solid, SIGNAL(triggered()), this, SLOT(setEntitiesRenderSolid()));
    connect(ui->actionAs_Illuminated_Solid, SIGNAL(triggered()), this, SLOT(setEntitiesRenderIlluminatedSolid()));

    connect(ui->actionCamera_controls, SIGNAL(triggered()), this, SLOT(togglePanelCameraControls()));
    // Stippling menu
    connect(ui->actionWith_Floyd_Steinberg_dithering, SIGNAL(triggered()), this, SLOT(startStipplingProcessFloydSteinbergDithering()));
    connect(ui->actionWith_Stucki_dithering, SIGNAL(triggered()), this, SLOT(startStipplingProcessStuckiDithering()));
    connect(ui->actionRe_apply_Stipple_Dot_dispersion, SIGNAL(triggered()), this, SLOT(reApplyStipplingDotDispersion()));
    connect(ui->actionGenerate_final_image, SIGNAL(triggered()), this, SLOT(generateFinalImage()));

    // Top bar
    connect(ui->selectionToolButton, SIGNAL(released()), this, SLOT(selectionTool()));
    connect(ui->addOperationButton, SIGNAL(released()), this, SLOT(addOperation()));
    connect(ui->addCylinderButton, SIGNAL(released()), this, SLOT(addCylinder()));
    connect(ui->addPrismButton, SIGNAL(released()), this, SLOT(addPrism()));

    // Camera controls
    connect(ui->doubleSpinBox_Pitch, SIGNAL(valueChanged(double)), this, SLOT(updateCamera()));
    connect(ui->doubleSpinBox_Height, SIGNAL(valueChanged(double)), this, SLOT(updateCamera()));
    connect(ui->doubleSpinBox_CameraDistance, SIGNAL(valueChanged(double)), this, SLOT(updateCamera()));

    // Widgets
    connect(ui->dockWidgetCameraControls, SIGNAL(visibilityChanged(bool)), this, SLOT(updateCameraControlsPanelMenu()));

    // Entity Tree Views (OpenGL and QT)
    connect(&entities, SIGNAL(modifiedTree()), ui->openGLViewport, SLOT(updateGL()));
    connect(&entities, SIGNAL(modifiedTree()), ui->treeWidget_Entities, SLOT(update()));
    connect(&entities, SIGNAL(modifiedTree()), ui->superiorView_openGLViewport, SLOT(update()));
    connect(&entities, SIGNAL(modifiedInformation()), ui->openGLViewport, SLOT(updateGL()));
    connect(&entities, SIGNAL(modifiedInformation()), ui->treeWidget_Entities, SLOT(updateSelected()));
    connect(&entities, SIGNAL(modifiedInformation()), ui->superiorView_openGLViewport, SLOT(update()));




    ui->actionCamera_controls->setCheckable(true);
    ui->actionCamera_controls->setChecked(true);

    entitiesRenderModeActionGroup = new QActionGroup(this);
    ui->actionAs_Wireframe->setCheckable(true);
    ui->actionAs_Wireframe->setActionGroup(entitiesRenderModeActionGroup);
    ui->actionAs_Solid->setCheckable(true);
    ui->actionAs_Solid->setActionGroup(entitiesRenderModeActionGroup);
    ui->actionAs_Illuminated_Solid->setCheckable(true);
    ui->actionAs_Illuminated_Solid->setActionGroup(entitiesRenderModeActionGroup);
    // Set default
    ui->actionAs_Wireframe->setChecked(true);


    floorRenderModeActionGroup = new QActionGroup(this);
    ui->actionGrid_only->setCheckable(true);
    ui->actionGrid_only->setActionGroup(floorRenderModeActionGroup);
    ui->actionSolid_only->setCheckable(true);
    ui->actionSolid_only->setActionGroup(floorRenderModeActionGroup);
    ui->actionGrid_and_solid->setCheckable(true);
    ui->actionGrid_and_solid->setActionGroup(floorRenderModeActionGroup);
    // Set default
    ui->actionGrid_only->setChecked(true);
    setFloorRenderMode(GRID);

    //ui->topBar->hide();
    //ui->openGLContainer->hide();
    ui->stippling_openGLContainer->hide();
    ui->openGLViewport->setFocus();




    ui->dockWidget_entities->installEventFilter(this);
    ui->dockWidgetCameraControls->installEventFilter(this);




    ui->treeWidget_Entities->header()->resizeSection(0, 30); // Empty space
    ui->treeWidget_Entities->header()->resizeSection(1, 100); // Name
    ui->treeWidget_Entities->header()->resizeSection(2, 52); // Visible
    ui->treeWidget_Entities->header()->resizeSection(3, 77); // Properties
    ui->treeWidget_Entities->header()->resizeSection(4, 105); // Control points
    ui->treeWidget_Entities->header()->resizeSection(5, 110); // Type
    //ui->dockWidget_entities->adjustSize();





    ui->actionImport_CSG_Tree->setEnabled(false);
    ui->actionExport_CSG_Tree->setEnabled(false);
    ui->actionReset_viewing_transformations->setEnabled(false);
    ui->menuGenerate_and_render->setEnabled(false);
    ui->actionRe_apply_Stipple_Dot_dispersion->setEnabled(false);
    ui->actionGenerate_final_image->setEnabled(false);

}

MainWindow::~MainWindow()
{
    if(ui != 0)
    {
        delete ui;
        ui = 0;
    }
}

void MainWindow::loadCameraSpecifications()
{
    QFile file(":/camera_specifications/camera_specifications.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Camera specifications file could not be opened." << endl;
        exit( EXIT_FAILURE );
    }
    QByteArray data = file.readAll();
    file.close();

    QString camSpecs = QString(data);

    QDomDocument document;
    QDomElement element;

    document.setContent(camSpecs);
    // Get the root element (cameras)
    element = document.documentElement();


    // Iterate over the cameras
    QDomNode node = element.firstChildElement("camera");
    while(!node.isNull())
    {
        QDomElement name = node.firstChildElement("name");
        QDomElement sensorSizeX = node.firstChildElement("sensorSizeX");
        QDomElement sensorSizeY = node.firstChildElement("sensorSizeY");
        QDomElement cropFactor = node.firstChildElement("cropFactor");

        if(!name.isNull() && !sensorSizeX.isNull() && !sensorSizeY.isNull() && !cropFactor.isNull())
        {
            cameras.append( Camera(name.text(),
                                   sensorSizeX.text().toFloat(),
                                   sensorSizeY.text().toFloat(),
                                   cropFactor.text().toFloat()) );
        }
        node = node.nextSiblingElement("camera");
    }

    camerasGroup = new QActionGroup(this);
    foreach(Camera c, cameras)
    {
        QAction* action = ui->menuCamera_Model->addAction(c.name);
        action->setCheckable(true);
        action->setActionGroup(camerasGroup);
        if(c.name == "Canon EOS 600D")
        {
            action->setChecked(true);
            setActiveCamera();
        }
        action->connect(action, SIGNAL(triggered()), this, SLOT(setActiveCamera()));
    }
}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty())
    {
        image = cv::imread(fileName.toStdString(), CV_LOAD_IMAGE_COLOR);

        /*
        cv::imshow("image",image);
        out << "The image has " << image.channels() << " channels." << endl;
        out << "See: http://answers.opencv.org/question/3288/problem-reading-png-with-transparency-layer/" << endl;
        */

        if(! image.data ) // Check for invalid input
        {
            QMessageBox::information(this, tr("PROJECT"), tr("Could not open or find the image %1.").arg(fileName));
            return;
        }

        // Tag1, Camera model
        QString cmd1 = "exiv2";
        QStringList args1 = QStringList();
        args1 << "-g";
        args1 << "Exif.Image.Model";
        args1.append(fileName);

        QString tag1 = getTagFull(cmd1, args1, 3);

        out << "Loading image, camera model: (" << tag1 << ")" << endl;

        if(tag1 != "")
        {
            // Model found, search in the database
            bool found = false;
            foreach(QAction * a, camerasGroup->actions())
            {
                if(a->text() == tag1)
                {
                    if(camerasGroup->checkedAction() != 0)
                    {
                        camerasGroup->checkedAction()->setChecked(false);
                    }
                    a->setChecked(true);
                    setActiveCamera();

                    found = true;
                    break;
                }
            }

            if(!found)
            {
                // TODO Let the user know that the right camera model is not present in the database and should be specified/added.
            }
        }
        else
        {
            // TODO Let the user know that the right camera model should be specified.
        }

        // Tag2, Focal Length
        QString cmd2 = "exiv2";
        QStringList args2 = QStringList();
        args2 << "-g";
        args2 << "Exif.Photo.FocalLength";
        args2.append(fileName);

        QString tag2 = getTagWord(cmd2, args2, 3);

        float focalLength;
        if(tag2 != "")
        {
            focalLength = tag2.toFloat();
        }
        else
        {
            flNotFound = new FocalLengthNotFoundDialog();
            // The focal length exiv tag could not be read from the image, the user must input this value.
            flNotFound->show();
            flNotFound->activateWindow();

            connect( flNotFound, SIGNAL(accepted()), this, SLOT(loadImageStage2()) );

            focalLength = -1;
            return;
        }

        _focalLength = focalLength;

        loadImageStage3();
    }
}

void MainWindow::loadImageStage2()
{
    // Remove former Custom cameras
    for(int i=0; i<cameras.size(); ++i)
    {
        if(cameras.at(i).name == "Custom camera")
        {
            cameras.remove(i);
        }
    }
    foreach(QAction * a, camerasGroup->actions())
    {
        if(a->text() == "Custom camera")
        {
            camerasGroup->removeAction(a);
        }
    }

    _fov = flNotFound->getFOV();
    Camera custom = flNotFound->getCamera();
    cameras.append(custom);

    // Add the action to the menu
    QAction* action = ui->menuCamera_Model->addAction(custom.name);
    action->setCheckable(true);
    action->setActionGroup(camerasGroup);

    action->setChecked(true);
    setActiveCamera();

    action->connect(action, SIGNAL(triggered()), this, SLOT(setActiveCamera()));

    if(flNotFound != 0)
    {
        delete(flNotFound);
        flNotFound = 0;
    }

    loadImageStage4();
}

void MainWindow::loadImageStage3()
{
    _fov = 2.0f * atan(activeCamera.sensorSizeX / (2.0f * _focalLength * activeCamera.cropFactor)) * 180.0f / PI;
    loadImageStage4();
}

void MainWindow::loadImageStage4()
{
    out << "Using the camera: " << activeCamera.name << endl;

    // Load the texture to the GLWidget so it can later be painted
    ui->openGLViewport->setImage(image, _fov);
    ui->superiorView_openGLViewport->setImage(image, _fov);
    aspectRatio = image.cols / (float) image.rows;

    maxWidth_glWidget = ui->centralWidget->size().width() - 100;
    maxHeight_glWidget = ui->centralWidget->size().height() - 120;
    //out << ui->centralWidget->size().width() << endl;
    //out << ui->centralWidget->size().height() << endl;

    if(aspectRatio > 1)
    {
        // Width > height
        ui->openGLViewport->resize(maxWidth_glWidget, (int)(maxWidth_glWidget/aspectRatio));
        ui->openGLContainer->resize(maxWidth_glWidget + 60, (int)(maxWidth_glWidget/aspectRatio) + 60);
        ui->stippling_openGLViewport->resize(maxWidth_glWidget, (int)(maxWidth_glWidget/aspectRatio));
        ui->stippling_openGLContainer->resize(maxWidth_glWidget + 60, (int)(maxWidth_glWidget/aspectRatio) + 60);
    }
    else
    {
        // Height > width
        ui->openGLViewport->resize(maxHeight_glWidget*aspectRatio, maxHeight_glWidget);
        ui->openGLContainer->resize((maxHeight_glWidget*aspectRatio) + 60, maxHeight_glWidget + 60);
        ui->stippling_openGLViewport->resize(maxHeight_glWidget*aspectRatio, maxHeight_glWidget);
        ui->stippling_openGLContainer->resize((maxHeight_glWidget*aspectRatio) + 60, maxHeight_glWidget + 60);
    }

    //ui->doubleSpinBox_Height->setMinimum(double(-image.rows));
    //ui->doubleSpinBox_Height->setMaximum(double(image.rows));
    ui->doubleSpinBox_Height->setValue(0.0f);
    ui->doubleSpinBox_Pitch->setValue(0.0f);
    ui->doubleSpinBox_CameraDistance->setValue(0.0f);


    ui->actionImport_CSG_Tree->setEnabled(true);
    ui->actionExport_CSG_Tree->setEnabled(true);
    ui->actionReset_viewing_transformations->setEnabled(true);
    ui->menuGenerate_and_render->setEnabled(true);

    //ui->topBar->show();
    //ui->openGLContainer->show();
}

void MainWindow::importCSGTree()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        QString selFilter = tr("XML (*.xml)");
        QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Import CSG Tree"),
                QDir::currentPath(),
                tr("XML (*.xml)"),
                &selFilter
        );

        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qWarning() << "Camera specifications file could not be opened." << endl;
                exit( EXIT_FAILURE );
            }
            QByteArray data = file.readAll();
            file.close();

            QString xml = QString(data);

            // Since an initial design decision was to make the entities self representable (the
            // entities know how to render themselves), the responsibility of adding them belongs
            // to the GLWidget (it has the OpenGL resources needed) and not to the
            // EntityTreeController, as theoretically should be (still, once created, the GLWidget
            // asks the EntityTreeController to add them to the model).
            // Making the entities self representable has simplified the model and its use, which
            // in turn has accelerated its development.

            ui->openGLViewport->importEntitiesFromXML(xml);
        }
    }
}

void MainWindow::exportCSGTree()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        QFileDialog fileDialog(this);
        // fileDialog.setDefaultSuffix(".xml"); // Does not work under linux
        QString fileName = fileDialog.getSaveFileName(this,
                 tr("Export CSG Tree"), "",
                 tr("XML (*.xml);"));


        while(fileName.isEmpty())
        {
            // fileDialog.setDefaultSuffix(".xml"); // Does not work under linux
            fileName = fileDialog.getSaveFileName(this,
                         tr("Export CSG Tree"), "",
                         tr("XML (*.xml);"));
        }

        // Fast hack to solve the extension problem
        // TODO Check for capital letters!
        if(!fileName.endsWith(".xml"))
        {
            fileName += ".xml";
        }

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        }
        else
        {
            QString xml = entities.toXML();

            QTextStream fo(&file);
            fo << xml;
        }
    }
}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::openConfigurationDialog()
{
    configDialog = new ConfigurationDialog();

    // Pass the current configuration to the dialog so it can show the updated values.
    configDialog->loadConfiguration(&_configuration);

    configDialog->show();
    configDialog->activateWindow();

    connect( configDialog, SIGNAL(accepted()), this, SLOT(updateConfiguration()) );
}

void MainWindow::updateConfiguration()
{
    // Tell the glWidget to reapply the configuration (it might have changed).
    ui->openGLViewport->reApplyConfiguration();
    ui->stippling_openGLViewport->reApplyConfiguration();

    // Destroy the configuration window.
    if(configDialog != 0)
    {
        delete configDialog;
        configDialog = 0;
    }
}

void MainWindow::resetViewingTransformations()
{
    switch(_mode)
    {
    case MODE_3DENGINE:
        ui->openGLViewport->resetViewingTransformations();
        ui->openGLViewport->updateGL();
        break;
    case MODE_STIPPLING:
        ui->stippling_openGLViewport->resetViewingTransformations();
        ui->stippling_openGLViewport->updateGL();
        break;
    }
}

void MainWindow::setEntitiesRenderWireframe()
{
    setEntitiesRenderMode(WIREFRAME);
}

void MainWindow::setEntitiesRenderSolid()
{
    setEntitiesRenderMode(DARK_SOLID);
}

void MainWindow::setEntitiesRenderIlluminatedSolid()
{
    setEntitiesRenderMode(ILLUMINATED_SOLID);
}

void MainWindow::setEntitiesRenderMode(EntitiesRenderMode entitiesRenderMode)
{
    ui->openGLViewport->setEntitiesRenderMode(entitiesRenderMode);
}

void MainWindow::setFloorRenderHidden()
{
    setFloorRenderMode(NO_FLOOR);
}

void MainWindow::setFloorRenderGrid()
{
    setFloorRenderMode(GRID);
}

void MainWindow::setFloorRenderSolid()
{
    setFloorRenderMode(SOLID);
}

void MainWindow::setFloorRenderGridSolid()
{
    setFloorRenderMode(GRID_SOLID);
}

void MainWindow::setFloorRenderMode(FloorRenderMode floorRenderMode)
{
    ui->openGLViewport->setFloorRenderMode(floorRenderMode);
}

QString MainWindow::getTagWord(QString cmd, QStringList args, int word)
{
    QProcess process;

    process.start(cmd, args, QIODevice::ReadWrite);

    process.waitForStarted();
    process.waitForReadyRead();
    process.waitForFinished();

    QByteArray cmd_output = process.readAllStandardOutput();
    // QByteArray cmd_error_output = process.readAllStandardError();

    QString output = QString(cmd_output);

    QStringList words = output.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(words.size() >= word+1)
    {
        return words.at(word);
    }
    else
    {
        return "";
    }
}

QString MainWindow::getTagFull(QString cmd, QStringList args, int word)
{
    QProcess process;

    process.start(cmd, args, QIODevice::ReadWrite);

    process.waitForStarted();
    process.waitForReadyRead();
    process.waitForFinished();

    QByteArray cmd_output = process.readAllStandardOutput();
    // QByteArray cmd_error_output = process.readAllStandardError();

    QString output = QString(cmd_output);

    QStringList words = output.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(words.size() >= word+1)
    {
        QString result = "";
        for(int i=word; i<words.size(); ++i)
        {
            result += words.at(i);
            if(i != words.size()-1)
            {
                result += " ";
            }
        }
        return result;
    }
    else
    {
        return "";
    }
}

void MainWindow::selectionTool()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        ui->openGLViewport->interactionSelectionTool();
    }
}

void MainWindow::addOperation()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        ui->openGLViewport->interactionAddOperation();
    }
}

void MainWindow::addCylinder()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        ui->openGLViewport->interactionAddCylinder();
    }
}

void MainWindow::addPrism()
{
    if(ui->openGLViewport->isImageLoaded())
    {
        ui->openGLViewport->interactionAddPrism();
    }
}

void MainWindow::setActiveCamera()
{
    QString cameraModel;
    foreach(QAction* a, camerasGroup->actions())
    {
        if(a->isChecked())
        {
            cameraModel = a->text();
            break;
        }
    }
    foreach(Camera c, cameras)
    {
        if(c.name == cameraModel)
        {
            activeCamera = c;
            break;
        }
    }

    if(activeCamera.forcedFOV)
    {
        _fov = activeCamera.fov;
    }
    else
    {
        _fov = 2.0f * atan(activeCamera.sensorSizeX / (2.0f * _focalLength * activeCamera.cropFactor)) * 180.0f / PI;
    }

    // Force resetting the scene in the GLWidget according to the new camera's FOV
    ui->openGLViewport->setFOV(_fov);
}

void MainWindow::updateCamera()
{
    int height;
    float pitch;
    float distance;

    if(ui->openGLViewport->isImageLoaded())
    {
        height = float(ui->doubleSpinBox_Height->value());
        ui->openGLViewport->setCameraHeight(height);

        pitch = float(ui->doubleSpinBox_Pitch->value());
        ui->openGLViewport->setCameraPitch(pitch);

        distance = float(ui->doubleSpinBox_CameraDistance->value());
        ui->openGLViewport->setCameraDistance(distance);
    }
}

void MainWindow::togglePanelCameraControls()
{
    if(ui->actionCamera_controls->isChecked())
    {
        ui->dockWidgetCameraControls->show();
    }
    else
    {
        ui->dockWidgetCameraControls->hide();
    }
}

void MainWindow::updateCameraControlsPanelMenu()
{
    bool menuState = ui->actionCamera_controls->isChecked();
    bool panelState = ui->dockWidgetCameraControls->isVisible();
    if(menuState != panelState)
    {
        ui->actionCamera_controls->setChecked(panelState);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    resizeOpenGLContainer();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    quit();
}

void MainWindow::resizeOpenGLContainer()
{
    if(aspectRatio > 1)
    {
        // Width > height
        ui->openGLContainer->resize(maxWidth_glWidget + 60, (int)(maxWidth_glWidget/aspectRatio) + 60);
        ui->stippling_openGLContainer->resize(maxWidth_glWidget + 60, (int)(maxWidth_glWidget/aspectRatio) + 60);
    }
    else
    {
        // Height > width
        ui->openGLContainer->resize((maxHeight_glWidget*aspectRatio) + 60, maxHeight_glWidget + 60);
        ui->stippling_openGLContainer->resize((maxHeight_glWidget*aspectRatio) + 60, maxHeight_glWidget + 60);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize && (obj == ui->dockWidget_entities || obj == ui->dockWidgetCameraControls))
    {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
        //qDebug("Dock Resized (New Size) - Width: %d Height: %d", resizeEvent->size().width(), resizeEvent->size().height());
        resizeOpenGLContainer();
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::startStipplingProcessFloydSteinbergDithering()
{
    startStipplingProcess(DotGenerationWorker::FLOYD_STEINBERG);
}

void MainWindow::startStipplingProcessStuckiDithering()
{
    startStipplingProcess(DotGenerationWorker::STUCKI);
}

void MainWindow::startStipplingProcess(DotGenerationWorker::DitheringMethod method)
{
    ui->openGLViewport->generateSolidRenderings();
    cv::Mat solidRendering = ui->openGLViewport->renderSceneToImageAsSolidAllIlluminated();

    ui->openGLContainer->hide();
    ui->topBar->hide();
    //ui->dockWidget_entities->hide();
    ui->dockWidgetCameraControls->hide();
    ui->dockWidgetSupportVisualization->hide();
    ui->stippling_openGLContainer->show();

	ui->actionOpen_Image->setEnabled(false);
    ui->actionImport_CSG_Tree->setEnabled(false);
    //ui->actionExport_CSG_Tree->setEnabled(false);
    ui->menuCamera_Model->setEnabled(false);
    ui->menuFloor->setEnabled(false);
    ui->menuEntities->setEnabled(false);
    ui->actionCamera_controls->setEnabled(false);

    ui->actionRe_apply_Stipple_Dot_dispersion->setEnabled(true);
    ui->actionGenerate_final_image->setEnabled(true);

    resizeOpenGLContainer();

    ui->stippling_openGLViewport->stippleImage(image, method, solidRendering);

    _mode = MODE_STIPPLING;
}

void MainWindow::reApplyStipplingDotDispersion()
{
    ui->stippling_openGLViewport->applyStippleDotDispersion();
}

void MainWindow::generateFinalImage()
{
    QFileDialog fileDialog(this);
    // fileDialog.setDefaultSuffix(".png"); // Does not work under linux
    QString fileName = fileDialog.getSaveFileName(this,
             tr("Generate final image (png)"), "",
             tr("PNG Image (*.png);"));


    while(fileName.isEmpty())
    {
        // fileDialog.setDefaultSuffix(".png"); // Does not work under linux
        fileName = fileDialog.getSaveFileName(this,
                     tr("Generate final image (png)"), "",
                     tr("PNG Image (*.png);"));
    }

    // Fast hack to solve the extension problem
    // TODO Check for capital letters!
    if(!fileName.endsWith(".png"))
    {
        fileName += ".png";
    }

    ui->stippling_openGLViewport->saveStippledImageToDisk(fileName);
}
