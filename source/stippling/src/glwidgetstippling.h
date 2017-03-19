/**
 * @file glwidgetstippling.h
 * @brief GLWidgetStippling header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/April/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */


#ifndef _GLWIDGETSTIPPLING_H
#define _GLWIDGETSTIPPLING_H

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
#include <sstream>

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
#include <QThread>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "util.h"
#include "enums.h"
#include "idmanager.h"
#include "infiniteplane.h"
#include "infiniteplanez0.h"
#include "glcamera.h"
#include "stippledot.h"
#include "framebuffer.hh"
#include "quadtree.h"
#include "dotgenerationworker.h"
#include "configuration.h"
#include "entitytreecontroller.h"



class GLWidgetStippling : public QGLWidget
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

    // Screen camera
    float zNear;
    float zFar;
    float cameraDistance;
    float cameraPosX;
    float cameraPosY;
    float cameraPosOffsetX;
    float cameraPosOffsetY;
    float cameraZoom;
    GLCamera _sceneCamera;

    // Tiling camera
    float tiling_zNear;
    float tiling_zFar;
    float tiling_cameraDistance;
    float tiling_cameraPosX;
    float tiling_cameraPosY;
    float tiling_cameraPosOffsetX;
    float tiling_cameraPosOffsetY;
    float tiling_cameraZoom;
    GLCamera tiling_sceneCamera;

    // Mouse
    QPoint lastMousePos;
    bool isLeftButtonPressed;
    bool isMiddleButtonPressed;
    bool isRightButtonPressed;
    // Keyboard
    bool isDeletePressed;




    // Set only when the GLWidget is constructed.
    // Not meant to be changed after instance construction (so the id manager stays consistent).
    glm::vec4 _backgroundColor;


    // Selection mode flag
    SelectionMode _selectionMode;
    // Selected Object id
    int _selectedID;
    IDManager _idManager;


    GLEntity axesObj;




    InfinitePlaneZ0 _xyPlane;
    InfinitePlane * _coordCapturePlane;
    float _coordCapture_xLeft;
    float _coordCapture_xRight;
    float _coordCapture_yBottom;
    float _coordCapture_yTop;
    float _coordCapture_zNear;
    float _coordCapture_zFar;





    QVector<GLEntity> sprites;

    glm::vec2 spriteSize;
    glm::vec2 spritesMatrixSize;

    cv::Mat spritesImg;
    GLuint spritesTextureID;

    QuadTree * _stipplingDots;
    bool stipplingMode;
    int stippledImageRows;
    int stippledImageCols;


    QThread * _dotGenerationWorkerThread;
    DotGenerationWorker * _dotGenerationWorker;

    Configuration * _configuration;



    bool _stipplingPerformedAtLeastOnce;

    S3DFBO * _fboStipplingToTexture;
    GLuint _stipplingTextureID;
    GLEntity _stipplingTextureHolder;
    int _stipplingTextureHolderRows;
    int _stipplingTextureHolderCols;
    bool _isStipplingTextureReady;

    bool _renderUsingMultipleTiles;

    QVector<S3DFBO *> _tilesFBOs;
    QVector<glm::vec3> _tilesPos;
    bool _areStipplingTexturesReady;


    EntityTreeController * _entities;

private:

    cv::Mat fboTexturetoImage(S3DFBO * fbo, int height, int width);

    QString checkGLError();

    void glSetup();

    GLuint prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath);

    void initializeAxes();

    void initializeSprite(GLEntity & dot, glm::vec2 spriteSize,
                          glm::vec2 spritesMatrixSize,
                          glm::vec2 spritesMatrixPosition);

    void initializeSprites();

    void initializeStipplingTextureHolder(int rows, int cols);

public:

    bool isImageLoaded() const;

    GLWidgetStippling(QWidget * parent = NULL);

    ~GLWidgetStippling();

    void initializeGL ();

    void resizeGL(int w, int h);

    void resetProjection(bool isTileRenderingModeON);

    void paintGL();

    void renderUsingSingleTile();

    void renderUsingMultipleTiles();

    void paintScene(bool isCoordCaptureModeON, bool isTileRenderingModeON);

    void singleTileRenderCurrentScene();

    void tileRenderCurrentScene();

    void stippleImage(cv::Mat toStipple, DotGenerationWorker::DitheringMethod ditheringMethod,
                      cv::Mat solid3DModel);

public slots:
    void stippleImageEnded();




public:
    void selectGL (int x, int y);




    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void interactionMoveCamera(QPoint mousePosition);



    void resetViewingTransformations();




    void applyStippleDotDispersion();


    void saveStippledImageToDisk(QString fileName);



    void setConfiguration(Configuration * configuration);

    void reApplyConfiguration();


    void setEntityTreeController(EntityTreeController * entities);
};



#endif  /* _GLWIDGETSTIPPLING_H */
