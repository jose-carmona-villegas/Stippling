/**
 * @file glwidget3dengine.h
 * @brief GLWidget3DEngine header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */


#ifndef _GLWIDGET3DENGINE_H
#define _GLWIDGET3DENGINE_H

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

// Dependancies on glew (QT conflicts with glew when including GLEW before QGLWidget)
#include "GL/glew.h"
#include "glentity.h"
#include "framebuffer.hh"
// End of conflicting glew dependancies


#include <stdlib.h>
#include <math.h>

#include <QtOpenGL/QGLWidget>
#include <QVector>
#include <QTextStream>
#include <QtGui/QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QString>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "controlpoint.h"
#include "prism.h"
#include "cylinder.h"
#include "util.h"
#include "idmanager.h"
#include "floorplane.h"
#include "enums.h"
#include "infiniteplane.h"
#include "infiniteplaney0.h"
#include "infiniteplanez0.h"
#include "configuration.h"
#include "glcamera.h"
#include "entitytreecontroller.h"
#include "heightindicator.h"
#include "stippledot.h"




class GLWidget3DEngine : public QGLWidget
{

    Q_OBJECT // must include this if you use Qt signals/slots

private:
    GLuint vertexArrayID;
    // Shader id
    GLuint colorShaderID;
    GLuint textureShaderID;
    GLuint zColorShaderID;
    GLuint pickingShaderID;
    GLuint stippleShaderID;
    GLuint xyColorShaderID;
    GLuint xzColorShaderID;

    // Uniform id
    GLuint matrixID;
    GLuint textureID;
    GLuint pickingColorID;

    // Projection matrix
    glm::mat4 projection;
    // View matrix
    glm::mat4 view;
    // Model matrix
    glm::mat4 model;
    // ModelViewProjection matrix
    glm::mat4 mvp;



    // Viewport size
    int viewportWidth;
    int viewportHeight;

    // Visualization Scene Viewing transformations
    glm::vec3 _pos;
    glm::vec3 _scale;

    QPoint _mouseIncr;



    // Projection mode
    ProjectionMode projectionMode;

    GLfloat zNearVirtual, zFarVirtual, zNearVisualization, zFarVisualization;



    GLuint virtualSceneFBOTextureID;

    // Image
    bool _isImageLoaded;
    cv::Mat image;
    GLuint imageTextureID;

    // Already loaded texture indicators (used to delete textures that won't be used anymore)
    bool _isImageTextureBeenLoaded;

    // Virtual scene camera parameters
    float _cameraFOV;
    float _cameraDistance;
    float _cameraDistanceModifier;



    GLCamera _visualizationSceneOrthographicCamera;

    GLCamera _virtualScenePerspectiveCamera;
    GLCamera _virtualSceneOrthographicCamera;




    // Mouse
    QPoint lastMousePos;
    QVector3D lastMousePos2;
    bool isLeftButtonPressed;
    bool isMiddleButtonPressed;
    bool isRightButtonPressed;
    // Keyboard
    bool isControlPressed;
    bool isDeletePressed;


    // Set only when the GLWidget is constructed. Not meant to be changed after instance construction.
    glm::vec4 _backgroundColor;


    // Selection mode flag
    SelectionMode _selectionMode;
    // Selected Object id
    int _selectedID;
    IDManager _idManager;



    GLEntity virtualSceneObj;
    GLEntity imageObj;
    FloorPlane floorPlane;
    GLEntity axesObj;


    EntityTreeController * _entities;



    enum InteractionState {NO_INTERACTION, ADDING_PRISM, ADDING_CYLINDER, ADDING_OPERATION, CONTROL_POINT_INTERACTION};
    InteractionState _interactionState;
    Entity3D * tempEntity;



    S3DFBO * fbo;


    EntitiesRenderMode _entitiesRenderMode;
    FloorRenderMode _floorRenderMode;


    InfinitePlaneZ0 _xyPlane;
    InfinitePlaneY0 _xzPlane;
    InfinitePlane * _coordCapturePlane;
    float _coordCapture_xLeft;
    float _coordCapture_xRight;
    float _coordCapture_yBottom;
    float _coordCapture_yTop;
    float _coordCapture_zNear;
    float _coordCapture_zFar;


    Configuration * _configuration;



    bool _testCoordCaptureRendering;
    float _extraSpaceBehindFocalPoint;


    bool _doNotUpdateGL;

private:

    cv::Mat fboTexturetoImage(S3DFBO * fbo, int height, int width);

    QString checkGLError();

    void printGLError();

    void glSetup();

    GLuint prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath);

    void initializeImage();

    void initializeAxes();

    void initializeLights();

    void interactionControlPointExistingEntity(QPoint mousePosition);

    void interactionVisualizationSceneMovement(QPoint mousePosition);

    void cleanInteraction();

    EntityTreeNode * createNodesFromXML(QDomNode xmlNode);

public:

    bool isImageLoaded() const;

    GLWidget3DEngine(QWidget * parent = NULL);

    ~GLWidget3DEngine();

    void initializeGL ();

    void resizeGL (int w, int h);

    void resetProjection_VisualizationScene();

    void resetProjection_VirtualScene();

    void paintGL ();

    void paintVisualizationScene(bool isCoordCaptureModeON);

    void paintVirtualScene(bool isCoordCaptureModeON, bool renderImageAsBackground = true, bool renderHeightIndicators = true);

    void selectGL (int x, int y);

    glm::vec4 offscreenCoordColorCapture(int x, int y, InfinitePlane * p, float xLeft, float xRight,
                                         float yBottom, float yTop, float zNear, float zFar);

    glm::vec4 xzOffscreenCoordSelection(int x, int y, float yCoord);

    glm::vec4 xyOffscreenCoordSelection(int x, int y, float zCoord);

    void setVirtualSceneFBO(GLuint textureID);

    void setImage(cv::Mat loadedImage, float fov);

    void setFOV(float fov);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void resetViewingTransformations();

    void interactionSelectionTool();

    void interactionAddPrism();

    void interactionAddCylinder();

    void interactionAddOperation();

    void setEntitiesRenderMode(EntitiesRenderMode entitiesRenderMode);

    void setFloorRenderMode(FloorRenderMode floorRenderMode);

    void setConfiguration(Configuration * configuration);

    void reApplyConfiguration();

    void setCameraHeight(int height);

    void setCameraPitch(float angle);

    void setCameraDistance(float distance);

    void setEntityTreeController(EntityTreeController * entities);

    void importEntitiesFromXML(QString xml);

    cv::Mat renderSceneToImageAsSolidAllIlluminated();

    cv::Mat renderSceneToImageAsSolidOneEntityIlluminated(Entity3D * illuminated);

    void generateSolidRenderings();
};



#endif  /* _GLWIDGET3DENGINE_H */
