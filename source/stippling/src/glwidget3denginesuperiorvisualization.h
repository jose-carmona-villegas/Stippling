/**
 * @file glwidget3denginesuperiorvisualization.h
 * @brief GLWidget3DEngineSuperiorVisualization header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 17/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef GLWIDGET3DENGINESUPERIORVISUALIZATION_H
#define GLWIDGET3DENGINESUPERIORVISUALIZATION_H

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
#include "enums.h"
#include "infiniteplane.h"
#include "infiniteplaney0.h"
#include "infiniteplanez0.h"
#include "configuration.h"
#include "glcamera.h"
#include "entitytreecontroller.h"
#include "heightindicator.h"
#include "stippledot.h"

class GLWidget3DEngineSuperiorVisualization : public QGLWidget
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




    GLfloat _zNear, _zFar;



    // Image
    bool _isImageLoaded;
    int imageRows;
    int imageColumns;




    // Virtual scene camera parameters
    float _cameraFOV;
    float _cameraDistance;
    float _cameraHeight;


    GLCamera _camera;




    // Set only when the GLWidget is constructed. Not meant to be changed after instance construction.
    glm::vec4 _backgroundColor;


    // Selection mode flag (always OFF since this visualization doesn't support interactions)
    static const SelectionMode _selectionMode = OFF;

    IDManager _idManager;



    GLEntity axesObj;



    EntityTreeController * _entities;



    Configuration * _configuration;


    bool _testCoordCaptureRendering;
    float _extraSpaceBehindFocalPoint;


    bool _doNotUpdateGL;

private:

    QString checkGLError();

    void glSetup();

    GLuint prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath);

    void initializeAxes();

    void initializeLights();

public:

    bool isImageLoaded() const;

    GLWidget3DEngineSuperiorVisualization(QWidget * parent = NULL);

    ~GLWidget3DEngineSuperiorVisualization();

    void initializeGL ();

    void resizeGL (int w, int h);

    void resetProjection();

    void paintGL ();

    void paintScene();

    void setImage(cv::Mat loadedImage, float fov);

    void setFOV(float fov);

    void setEntitiesRenderMode(EntitiesRenderMode entitiesRenderMode);

    void setConfiguration(Configuration * configuration);

    void reApplyConfiguration();

    void setEntityTreeController(EntityTreeController * entities);

};

#endif // GLWIDGET3DENGINESUPERIORVISUALIZATION_H
