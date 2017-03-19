/**
 * @file glwidget3dengine.cpp
 * @brief GLWidget3DEngine source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "glwidget3dengine.h"

cv::Mat GLWidget3DEngine::fboTexturetoImage(S3DFBO * fbo, int height, int width)
{
    fbo->renderFBO();

    cv::Mat img;

    img.create(height, width, CV_8UC3);




    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

    //set length of one complete row in destination data (doesn't need to equal img.cols)
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);

    //cv::Mat flipped;
    //cv::flip(img, flipped, 0);






    fbo->renderFramebuffer();


    return img;
}

QString GLWidget3DEngine::checkGLError()
{
    QString str = "";
    GLenum error = glGetError();

    while(error != GL_NO_ERROR)
    {
        switch(error)
        {
        case GL_INVALID_OPERATION:
            str += "INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            str += "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            str += "INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            str += "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            str += "INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_NO_ERROR:
            str += "NO_ERROR";
            break;
        default:
            str += "UNKNOWN_ERROR";
            break;
        }
        str += " ";
        error = glGetError();
    }

    if(str == "")
    {
        return "NO_ERROR";
    }
    else
    {
        return str;
    }
}

void GLWidget3DEngine::printGLError()
{
    out << endl;
    out << "################ GL STATUS = " << checkGLError()  << " ################" << endl;
    out << endl;
}

void GLWidget3DEngine::glSetup()
{
    glewExperimental = GL_TRUE;
    GLint GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK)
    {
        const GLubyte* errorStr = glewGetErrorString(GlewInitResult);
        int size = strlen(reinterpret_cast<const char*>(errorStr));
        qDebug() <<"Glew error "<<QString::fromUtf8(reinterpret_cast<const char*>(errorStr), size);

    }

    out << endl;
    out << "===   3D-Engine viewport   ===" << endl;
    out << "===   OpenGL initialized   ===" << endl;
    out << "Vendor: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) << endl;
    out << "Renderer: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) << endl;
    out << "Version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << endl;
    out << "GLSL version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << endl;
    out << "Status: Using GLEW " << QLatin1String(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))) << endl;
    out << "==============================" << endl;
    out << endl;
}


GLuint GLWidget3DEngine::prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath)
{
    struct Shader
    {
        const QString& filename;
        GLenum type;
        GLchar* source;
    }
    shaders[2] =
    {
        { vertexShaderPath, GL_VERTEX_SHADER, NULL },
        { fragmentShaderPath, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();

    for ( int i = 0; i < 2; ++i )
    {
        Shader& s = shaders[i];
        QFile file( s.filename );
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Cannot open file " << s.filename;
            exit( EXIT_FAILURE );
        }
        QByteArray data = file.readAll();
        file.close();
        s.source = data.data();

        if ( shaders[i].source == NULL )
        {
            qWarning() << "Failed to read " << s.filename;
            exit( EXIT_FAILURE );
        }
        GLuint shader = glCreateShader( s.type );
        glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
        glCompileShader( shader );

        GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled )
        {
            qWarning() << s.filename << " failed to compile:" ;
            GLint logSize;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            char* logMsg = new char[logSize];
            glGetShaderInfoLog( shader, logSize, NULL, logMsg );
            qWarning() << logMsg;
            delete [] logMsg;

            exit( EXIT_FAILURE );
        }

        glAttachShader( program, shader );
    }

    /* Link output */
    glBindFragDataLocation(program, 0, "fragColor");

    /* link and error check */
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked )
    {
        qWarning() << "Shader program failed to link";
        GLint logSize;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog( program, logSize, NULL, logMsg );
        qWarning() << logMsg ;
        delete [] logMsg;

        exit( EXIT_FAILURE );
    }

    /* use program object */
    glUseProgram(program);

    return program;
}

void GLWidget3DEngine::initializeImage()
{
    const GLfloat vertexBufferData[] =
    {
        0.0f, 0.0f, 0.0f,
        image.cols, 0.0f, 0.0f,
        image.cols, image.rows, 0.0f,

        image.cols, image.rows, 0.0f,
        0.0f , image.rows, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    // One color for each vertex.
    const GLfloat * colorBufferData = 0;

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    // One uv coord for each vertex.
    const GLfloat uvBufferData[] =
    {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    };

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferData)/sizeof(GLfloat);

    imageObj.initialize(vertexBufferData, colorBufferData,
                        normalBufferData, uvBufferData,
                        numberOfValuesInStaticBuffers, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                        textureShaderID, pickingShaderID);
    //imageObj.translate(glm::vec3(0.0f, 0.0f, 0.0f));
}

void GLWidget3DEngine::initializeAxes()
{
    float max = 10000.0f;

    const GLfloat vertexBufferData[] =
    {
        // x axis
        -max, 0.0f, 0.0f,
        max, 0.0f, 0.0f,
        // y axis
        0.0f, -max, 0.0f,
        0.0f, max, 0.0f,
        // z axis
        0.0f, 0.0f, -max,
        0.0f, 0.0f, max,
    };

    // One color for each vertex.
    const GLfloat colorBufferData[] =
    {
        // x axis
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        // y axis
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        // z axis
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
    };

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    // One uv coord for each vertex.
    const GLfloat * uvBufferData = 0;

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferData)/sizeof(GLfloat);

    axesObj.initialize(vertexBufferData, colorBufferData,
                       normalBufferData, uvBufferData,
                       numberOfValuesInStaticBuffers, GL_LINES, 3, GL_STATIC_DRAW,
                       colorShaderID, pickingShaderID);
}

void GLWidget3DEngine::initializeLights()
{
    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularReflectivity[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // Directional light (light coming from infinity following that vector)
    // This simulates sunlight.
    GLfloat lightPos[] = { -1.0f, 1.0f, 0.0f, 0.0f };

    glEnable(GL_DEPTH_TEST); // Hidden surface removal

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectivity);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    //glDisable(GL_LIGHTING);
}

bool GLWidget3DEngine::isImageLoaded() const
{
    return _isImageLoaded;
}

GLWidget3DEngine::GLWidget3DEngine(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    resetViewingTransformations();

    viewportWidth = 1;
    viewportHeight = 1;

    projectionMode = ORTHOGRAPHIC;
    _isImageLoaded = false;
    _isImageTextureBeenLoaded = false;

    isLeftButtonPressed = false;
    isMiddleButtonPressed = false;
    isRightButtonPressed = false;

    isControlPressed = false;
    isDeletePressed = false;

    _backgroundColor = glm::vec4(0.2f,0.2f,0.2f,1.0f);
    _idManager = IDManager(_backgroundColor);
    _selectionMode = OFF;
    _selectedID = _idManager.NONE();

    int id;

    id = _idManager.getNewID();
    imageObj = GLEntity(id, _idManager.encodeID(id));

    id = _idManager.getNewID();
    axesObj = GLEntity(id, _idManager.encodeID(id));

    virtualSceneObj = GLEntity(_idManager.NONE(),_idManager.encodeID(_idManager.NONE()));

    fbo = 0;

    _interactionState = NO_INTERACTION;

    _entitiesRenderMode = WIREFRAME;
    _floorRenderMode = GRID_SOLID;

    tempEntity = 0;

    _entities = 0;

    _testCoordCaptureRendering = false;
    _extraSpaceBehindFocalPoint = 5000.0f;

    _doNotUpdateGL = false;

    _cameraDistanceModifier = 0.0f;
}

GLWidget3DEngine::~GLWidget3DEngine()
{
    if(fbo != 0)
    {
        delete fbo;
        fbo = 0;
    }
}

void GLWidget3DEngine::initializeGL()
{
    glSetup();

    makeCurrent();

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create and compile our GLSL program from the shaders
    colorShaderID = prepareShaderProgram(":shaders/colorShader.vert", ":shaders/colorShader.frag");
    textureShaderID = prepareShaderProgram(":shaders/textureShader.vert", ":shaders/textureShader.frag");
    zColorShaderID = prepareShaderProgram(":shaders/zColorShader.vert", ":shaders/zColorShader.frag");
    pickingShaderID = prepareShaderProgram(":shaders/pickingShader.vert",":shaders/pickingShader.frag");
    stippleShaderID = prepareShaderProgram(":shaders/stippleShader.vert",":shaders/stippleShader.frag");
    xyColorShaderID = prepareShaderProgram(":shaders/xyColorShader.vert", ":shaders/xyColorShader.frag");
    xzColorShaderID = prepareShaderProgram(":shaders/xzColorShader.vert", ":shaders/xzColorShader.frag");

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    int id;

     id = _idManager.getNewID();
     _xyPlane = InfinitePlaneZ0(id, _idManager.encodeID(id), xyColorShaderID, pickingShaderID);
     id = _idManager.getNewID();
    _xzPlane = InfinitePlaneY0(id, _idManager.encodeID(id), xzColorShaderID, pickingShaderID);

    floorPlane = FloorPlane(&_idManager, zColorShaderID, colorShaderID, pickingShaderID, colorShaderID, _configuration->floorWires());

    initializeAxes();

    reApplyConfiguration();

    initializeLights();
}

void GLWidget3DEngine::resizeGL(int w, int h)
{
    viewportWidth = w;
    viewportHeight = h;
    glViewport(0, 0, w, h);
}

void GLWidget3DEngine::resetProjection_VisualizationScene()
{
    zNearVisualization = 0.1f;
    zFarVisualization = 100.0f + zNearVisualization;
    projection = glm::ortho(-(image.cols/2.0f), (image.cols/2.0f), -image.rows/2.0f, image.rows/2.0f, zNearVisualization, zFarVisualization);
}

void GLWidget3DEngine::resetProjection_VirtualScene()
{
    float aspectRatio = viewportWidth / (float)viewportHeight;

    _cameraDistance = (image.rows / 2.0f) / tan( (_cameraFOV/2.0f) * PI / 180.0f );
    _cameraDistance += _cameraDistanceModifier;

    if(projectionMode == PERSPECTIVE)
    {
        zNearVirtual = 100.0f;
        zFarVirtual = _cameraDistance + _extraSpaceBehindFocalPoint;
        projection = glm::perspective(_cameraFOV, aspectRatio, zNearVirtual, zFarVirtual);
    }
    else // projectionMode == ORTHOGRAPHIC
    {
        zNearVirtual = 0.001f;
        zFarVirtual = 10.0f + zNearVirtual;
        projection = glm::ortho(-(image.cols/2.0f), (image.cols/2.0f), -image.rows/2.0f, image.rows/2.0f, zNearVirtual, zFarVirtual);
    }
}

void GLWidget3DEngine::paintGL()
{ 
    if(!_doNotUpdateGL)
    {
        //paintVisualizationScene(false);
        paintVisualizationScene(_testCoordCaptureRendering); // Use this line to see the coord capture plane
    }
}

void GLWidget3DEngine::paintVisualizationScene(bool isCoordCaptureModeON)
{
    // Render the virtual scene on the fbo
    if(_isImageLoaded)
    {
        fbo->renderFBO();
            paintVirtualScene(isCoordCaptureModeON);
        fbo->renderFramebuffer();

        setVirtualSceneFBO(fbo->getTexture());
    }

    // Draw ORTHOGRAPHIC
    projectionMode = ORTHOGRAPHIC;

    // Background color
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    resetProjection_VisualizationScene();
    view = _visualizationSceneOrthographicCamera.view();

    // Dynamic viewing transformations (from mouse interactions)
    view = glm::translate(view, _pos);
    view = glm::scale(view, _scale);

    // Render the visualization scene (paint fbo's texture which holds the result of rendering the virtual scene)
    if(_isImageLoaded)
    {
        // It will paint itself normally, independently of the selection mode.
        virtualSceneObj.paint(OFF, &view, &projection, 0, virtualSceneFBOTextureID, 0, 0, 0, 0, 0, 0, false);
    }
}

void GLWidget3DEngine::paintVirtualScene(bool isCoordCaptureModeON, bool renderImageAsBackground, bool renderHeightIndicators)
{
    // Background color
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    if(isCoordCaptureModeON)
    {
        // Draw PERSPECTIVE
        projectionMode = PERSPECTIVE;
        resetProjection_VirtualScene();
        view = _virtualScenePerspectiveCamera.view();

        _coordCapturePlane->paint(_selectionMode, &view, &projection,
                                  _coordCapture_xLeft, _coordCapture_xRight,
                                  _coordCapture_yBottom, _coordCapture_yTop,
                                  _coordCapture_zNear, _coordCapture_zFar);
    }
    else
    {
        // Draw ORTHOGRAPHIC
        projectionMode = ORTHOGRAPHIC;
        resetProjection_VirtualScene();
        view = _virtualSceneOrthographicCamera.view();

        if(_isImageLoaded && _selectionMode == OFF)
        {
            if(renderImageAsBackground)
            {
                imageObj.paint(_selectionMode, &view, &projection, 0, imageTextureID, 0, 0, 0, 0, 0, 0, false);
            }
        }

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Draw PERSPECTIVE
        projectionMode = PERSPECTIVE;
        resetProjection_VirtualScene();
        view = _virtualScenePerspectiveCamera.view();

        // Draw all models.
        if(_isImageLoaded)
        {
            // Axes are a visual only element.
            if(_selectionMode == OFF)
            {
                //axesObj.paint(_selectionMode, &view, &projection, 0, 0, 0, 0, 0, 0, 0, 0, false);
            }

            // The floor is always rendered (the rendering mode specifies if it will be painted solid, grid or both).
            floorPlane.paint(_selectionMode, &view, &projection, 0, 0, 0, 0, zNearVirtual, zFarVirtual, _floorRenderMode);

            // Paint the temporal entity (whose is position associated to the mouse position) if it's being added.
            if(_interactionState == ADDING_PRISM
                    || _interactionState == ADDING_CYLINDER
                    || _interactionState == ADDING_OPERATION)
            {
                tempEntity->paint(_selectionMode, &view, &projection, tempEntity, _entitiesRenderMode);
            }

            // Draw all the entities added
            QVector<Entity3D*> * visibleEntities = _entities->getVisibleEntities();
            foreach(Entity3D * entity, *visibleEntities)
            {
                entity->paint(_selectionMode, &view, &projection, _entities->selected(), _entitiesRenderMode);

                if(renderHeightIndicators)
                {
                    if(floorPlane.getY() != entity->position().y)
                    {
                        HeightIndicator indicator = HeightIndicator(colorShaderID, pickingShaderID, _configuration->entityWires(), floorPlane.getY(), entity->position().y, image.cols/10.0f);
                        indicator.translate(entity->position());
                        indicator.paint(_selectionMode, &view, &projection);
                    }
                }
            }
            visibleEntities->clear();
            if(visibleEntities != 0)
            {
                delete visibleEntities;
                visibleEntities = 0;
            }
        }
    }
}

void GLWidget3DEngine::selectGL(int x, int y)
{

    _selectionMode = ON;


    EntitiesRenderMode formerRenderMode = _entitiesRenderMode;
    _entitiesRenderMode = CPS_ONLY;

    paintGL();

    // Wait until all the pending drawing commands are really done.
    glFlush();
    glFinish();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Read the pixel at (x,y).
    unsigned char data[3];
    glReadPixels(x, viewportHeight-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

    int decodedID = _idManager.decodeID(glm::vec3(data[0],data[1],data[2]));

    /*
    out << "BackgroundID = " << _idManager.BackgroundID() << endl;
    out << "NoneID = " << _idManager.NONE() << endl;
    out << "decodedID (First sweep) = " << decodedID << endl;
    */

    // ============   Existing Entity Interaction   ============
    if(_interactionState == NO_INTERACTION
            && _entities->selected() != 0
            && _entities->selected()->ownsControlPoint(decodedID))
    {
        // Selected an entity's control point. Change the state and associate the mouse movement to said point.
        _interactionState = CONTROL_POINT_INTERACTION;
    }
    else
    {
        _entitiesRenderMode = DARK_SOLID;

        paintGL();

        // Wait until all the pending drawing commands are really done.
        glFlush();
        glFinish();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Read the pixel at (x,y).
        unsigned char data[3];
        glReadPixels(x, viewportHeight-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

        decodedID = _idManager.decodeID(glm::vec3(data[0],data[1],data[2]));
        //out << "decodedID (Second sweep) = " << decodedID << endl;

        if(_interactionState == NO_INTERACTION)
        {
            if(_entities->selected() != 0)
            {
                if(decodedID != _idManager.BackgroundID() && decodedID != _idManager.NONE())
                {
                    if(_entities->selected()->id() != decodedID)
                    {
                        _entities->select(decodedID);
                    }
                }
                else
                {
                    _entities->deselect();
                }
            }
            else
            {
                if(decodedID != _idManager.BackgroundID() && decodedID != _idManager.NONE())
                {
                    _entities->select(decodedID);
                }
            }
        }
    }

    if(decodedID == _idManager.BackgroundID() || decodedID == _idManager.NONE())
    {
        _selectedID = _idManager.NONE();
    }
    else
    {
        _selectedID = decodedID;
    }

    _entitiesRenderMode = formerRenderMode;

    _selectionMode = OFF;
    updateGL();


}

glm::vec4 GLWidget3DEngine::offscreenCoordColorCapture(int x, int y, InfinitePlane * p, float xLeft, float xRight,
                                               float yBottom, float yTop, float zNear, float zFar)
{
    if(_isImageLoaded)
    {
        _coordCapturePlane = p;

        _coordCapture_xLeft = xLeft;
        _coordCapture_xRight = xRight;
        _coordCapture_yBottom = yBottom;
        _coordCapture_yTop = yTop;
        _coordCapture_zNear = zNear;
        _coordCapture_zFar = zFar;

        paintVisualizationScene(true);

        // Wait until all the pending drawing commands are really done.
        // There is usually a long time between glDrawElements() and
        // the moment that all the fragments are completely rasterized.
        glFlush();
        glFinish();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Read the pixel at (x,y).
        unsigned char data[4];
        glReadPixels(x, viewportHeight-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        //out << "RGBA = (" << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << ")" << endl;

        // The RGB values must be normalized, to fit the range that the shader supplies.
        float normalizedR = data[0]/255.0f;
        float normalizedG = data[1]/255.0f;
        float normalizedB = data[2]/255.0f;
        float normalizedA = data[3]==255 ? 1.0f : 0.0f;

        //out << "Normalized RGBA = (" << normalizedR << "," << normalizedG << "," << normalizedB << "," << normalizedA << ")" << endl;

        // Finally, repaint normally, so the user does not see any of this.
        paintGL();

        glm::vec4 result = glm::vec4(normalizedR, normalizedG, normalizedB, normalizedA);

        if(result == _backgroundColor)
        {
            return glm::vec4(0.0f,0.0f,0.0f,0.0f);
        }
        else
        {
            return glm::vec4(normalizedR, normalizedG, normalizedB, normalizedA);
        }
    }
    else
    {
        return glm::vec4(0.0f,0.0f,0.0f,0.0f);
    }
}

glm::vec4 GLWidget3DEngine::xzOffscreenCoordSelection(int x, int y, float yCoord)
{
    _xzPlane.resetModel();
    _xzPlane.translate(glm::vec3(0.0f, yCoord, 0.0f));

    float xLeft = -image.cols;
    float xRight = image.cols;
    float yBottom = -image.rows;
    float yTop = image.rows;
    float zNear = -_extraSpaceBehindFocalPoint;
    float zFar = zFarVirtual;

    /*
    out << endl << " === xzOffscreenCoordSelection() === " << endl;
    out << "x: " << x << endl;
    out << "y: " << y << endl;
    out << "yCoord: " << yCoord << endl;
    out << "xLeft: " << xLeft << endl;
    out << "xRight: " << xRight << endl;
    out << "yBottom: " << yBottom << endl;
    out << "yTop: " << yTop << endl;
    out << "zNear: " << zNear << endl;
    out << "zFar: " << zFar << endl;
    */

    glm::vec4 coordColor = offscreenCoordColorCapture(x, y, &_xzPlane, xLeft, xRight, yBottom, yTop, zNear, zFar);

    // Transform the normalized color (varying in [0,1]) back into 3D space positions
    // R channel holds X coordinate, normalized between xLeft and xRight.
    // B channel holds Z coordinate, normalized between zNear and zFar.

    // From the shader: xColor = (gl_Position.x - xLeft) / (xRight - xLeft);
    // Operate to get the x position: gl_Position.x = ((xRight - xLeft)*xColor) + xLeft
    float xPosition = ((xRight - xLeft) * coordColor.r) + xLeft;

    float yPosition = yCoord;

    // From the shader: zColor = (gl_Position.z - zNear) / (zFar - zNear);
    // Operate to get the z position: gl_Position.z = ((zFar - zNear)*zColor) + zNear;
    float zPosition = ((zFar - zNear) * coordColor.b) + zNear;

    //out << "coords = (" << xPosition << "," << yPosition << "," << zPosition << "," << coordColor.a << ")" << endl;

    return glm::vec4(xPosition, yPosition, zPosition, coordColor.a);
}

glm::vec4 GLWidget3DEngine::xyOffscreenCoordSelection(int x, int y, float zCoord)
{
    _xyPlane.resetModel();
    _xyPlane.translate(glm::vec3(0.0f, -image.rows/2.0f, zCoord));

    float xLeft = -2*image.cols;
    float xRight = 2*image.cols;
    float yBottom = -3*image.rows;
    float yTop = 3*image.rows;
    float zNear = zNearVirtual;
    float zFar = zFarVirtual;

    glm::vec4 coordColor = offscreenCoordColorCapture(x, y, &_xyPlane, xLeft, xRight, yBottom, yTop, zNear, zFar);

    // Transform the normalized color (varying in [0,1]) back into 3D space positions
    // R channel holds X coordinate, normalized between xLeft and xRight.
    // G channel holds Y coordinate, normalized between yBottom and yTop.

    // From the shader: xColor = (gl_Position.x - xLeft) / (xRight - xLeft);
    // Operate to get the x position: gl_Position.x = ((xRight - xLeft)*xColor) + xLeft
    float xPosition = ((xRight - xLeft) * coordColor.r) + xLeft;

    // From the shader: yColor = (gl_Position.y - yBottom) / (yTop - yBottom);
    // Operate to get the y position: gl_Position.y = ((yTop - yBottom)*yColor) + yBottom;
    float yPosition = ((yTop - yBottom) * coordColor.g) + yBottom;
    // Compensation due to the different coordinate systems.
    yPosition -= image.rows/2.0f;

    float zPosition = zCoord;

    //out << "screenCoords = (" << x << "," << y << ")" << endl;
    //out << "coords = (" << xPosition << "," << yPosition << "," << zPosition << "," << coordColor.a << ")" << endl;

    return glm::vec4(xPosition, yPosition, zPosition, coordColor.a);
}

void GLWidget3DEngine::setVirtualSceneFBO(GLuint textureID)
{
    virtualSceneFBOTextureID = textureID;
    glBindTexture( GL_TEXTURE_2D, virtualSceneFBOTextureID );

    const GLfloat vertexBufferData[] =
    {
        0.0f, 0.0f, 0.0f,
        image.cols, 0.0f, 0.0f,
        image.cols, image.rows, 0.0f,

        image.cols, image.rows, 0.0f,
        0.0f , image.rows, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    // One color for each vertex.
    const GLfloat * colorBufferData = 0;

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    // One uv coord for each vertex.
    const GLfloat uvBufferData[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        /* inverted uv
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        */
    };

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferData)/sizeof(GLfloat);

    virtualSceneObj.initialize(vertexBufferData, colorBufferData,
                               normalBufferData, uvBufferData,
                               numberOfValuesInStaticBuffers, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                               textureShaderID, pickingShaderID);
}

void GLWidget3DEngine::setImage(cv::Mat loadedImage, float fov)
{
    makeCurrent();

    // Remove former models (if any).
    _entities->clear();

    // Set the image and update the cameras.
    this->image = cv::Mat(loadedImage);

    GLint internalFormat;
    GLenum format;
    if(image.channels() == 1)
    {
        internalFormat = GL_LUMINANCE;
        format = GL_LUMINANCE;
    }
    else if(image.channels() == 3)
    {
        internalFormat = GL_RGB;
        format = GL_BGR;
    }
    else if(image.channels() == 4)
    {
        internalFormat = GL_RGBA;
        format = GL_BGRA;
    }

    _visualizationSceneOrthographicCamera.reset();
    _virtualSceneOrthographicCamera.reset();
    _virtualScenePerspectiveCamera.reset();

    _cameraDistanceModifier = 0.0f;

    _cameraFOV = fov;
    _cameraDistance = (image.rows / 2.0f) / tan( (_cameraFOV/2.0f) * PI / 180.0f );
    _cameraDistance += _cameraDistanceModifier;


    _visualizationSceneOrthographicCamera.setPosition(glm::vec3((image.cols/2.0f),(image.rows/2),50.0f));

    _virtualSceneOrthographicCamera.setPosition(glm::vec3((image.cols/2.0f),(image.rows/2),10.0f));
    _virtualScenePerspectiveCamera.setDistance(_cameraDistance);

    // Release resources used previously (if any) and load the texture.
    if(_isImageTextureBeenLoaded)
    {
        // Delete the former image texture
        glDeleteTextures(1, &imageTextureID);
        _isImageTextureBeenLoaded = false;
    }

    glGenTextures( 1, &imageTextureID );
    glBindTexture( GL_TEXTURE_2D, imageTextureID );


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when the image is bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when the image is smaller than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, image.ptr());

    _isImageTextureBeenLoaded = true;

    initializeImage();

    resetViewingTransformations();

    if(fbo != 0)
    {
        delete fbo;
        fbo = 0;
    }
    makeCurrent();
    fbo = new S3DFBO(image.cols,image.rows,
                     GL_RGB,GL_RGB,GL_FLOAT,GL_NEAREST,
                     true,true);

    _isImageLoaded = true;


    floorPlane.resetModel();
    floorPlane.translate(glm::vec3(0.0f, -image.rows/4.0f, 0.0f));

    updateGL();
}

void GLWidget3DEngine::setFOV(float fov)
{
     _cameraFOV = fov;

     // Repaint so the scene parameters are recalculated.
     updateGL();
}

void GLWidget3DEngine::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        selectGL(event->pos().x(), event->pos().y());

        isLeftButtonPressed = true;
    }
    if(event->button() == Qt::MidButton)
    {
        printGLError();

        //_testCoordCaptureRendering = true;
        //updateGL();

        isMiddleButtonPressed = true;
    }
    if(event->button() == Qt::RightButton)
    {
        if(!isLeftButtonPressed)
        {
            if(_interactionState == NO_INTERACTION)
            {
                lastMousePos = event->pos();
            }

            isRightButtonPressed = true;
        }
    }
}

void GLWidget3DEngine::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::NoButton)
    {
        float yFloor = floorPlane.getY();
        glm::vec4 coords = xzOffscreenCoordSelection(event->x(), event->y(), yFloor);

        //out << endl;
        //out << "Mouse position = ( " << event->x() << " , " << event->y() << " )" << endl;
        //out << "Coords = ( " << coords.x << " , " << coords.y << " , " << coords.z << " )" << endl;

        if(coords.w == 1.0f)
        {
            axesObj.resetModel();
            axesObj.translate(glm::vec3(coords));

            // ============   New Entity Placement   ============
            if(_interactionState == ADDING_PRISM
                    || _interactionState == ADDING_CYLINDER
                    || _interactionState == ADDING_OPERATION)
            {
                tempEntity->resetModel();
                tempEntity->translate(glm::vec3(coords));
            }
        }
    }
    if(isLeftButtonPressed)
    {
        if(_interactionState == CONTROL_POINT_INTERACTION)
        {
            interactionControlPointExistingEntity(event->pos());
        }
    }
    if(isRightButtonPressed)
    {
        if(_interactionState == NO_INTERACTION)
        {
            interactionVisualizationSceneMovement(event->pos());
        }
    }

    updateGL();
}

void GLWidget3DEngine::mouseReleaseEvent( QMouseEvent* event )
{
    if(event->button() == Qt::LeftButton)
    {
        if(_interactionState == ADDING_PRISM
                || _interactionState == ADDING_CYLINDER
                || _interactionState == ADDING_OPERATION)
        {
            // ============   New Entity Placement   ============
            glm::vec4 coords = xzOffscreenCoordSelection(event->x(), event->y(), floorPlane.getY());

            if(coords.w == 1.0f)
            {
                tempEntity->resetModel();
                tempEntity->translate(glm::vec3(coords));
                _interactionState = NO_INTERACTION;

                _entities->addPrimitive(tempEntity);

                tempEntity = 0;
            }
        }
        else if(isLeftButtonPressed)
        {
            if(_interactionState == CONTROL_POINT_INTERACTION)
            {
                interactionControlPointExistingEntity(event->pos());
                _entities->selected()->requestParentUpdate();
                _interactionState = NO_INTERACTION;
            }
        }

        isLeftButtonPressed = false;
    }
    if(event->button() == Qt::MidButton)
    {
        //_testCoordCaptureRendering = false;
        //updateGL();


        isMiddleButtonPressed = false;
    }
    if(event->button() == Qt::RightButton)
    {
        if(isRightButtonPressed)
        {
            if(_interactionState == NO_INTERACTION)
            {
                interactionVisualizationSceneMovement(event->pos());
            }
        }

        isRightButtonPressed = false;
    }

    // Set the _selectedID back to none.
    if(_selectedID != _idManager.NONE())
    {
        _selectedID = _idManager.NONE();
    }

    updateGL();
}

void GLWidget3DEngine::wheelEvent(QWheelEvent *event)
{
    // Correction due to different mouse wheel's precision (degrees per step)
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    float zoomFactor = 25.0;

    _scale += glm::vec3(numSteps/zoomFactor);

    if(_scale.x <= 0 || _scale.y <= 0 || _scale.z <= 0)
    {
        _scale = glm::vec3(0.0f);
    }

    updateGL();

    event->accept();
}


void GLWidget3DEngine::keyPressEvent(QKeyEvent* event)
{
    //out << "Pressed " << event->key() << endl;
    if(event->key() == Qt::Key_Control)
    {
        //out << "Pressed Control" << endl;
        isControlPressed = true;
    }

    if(event->key() == Qt::Key_Delete)
    {
        //out << "Pressed Delete" << endl;
        isDeletePressed = true;
    }
}

void GLWidget3DEngine::keyReleaseEvent(QKeyEvent* event)
{
    //out << "Released " << event->key() << endl;
    if(event->key() == Qt::Key_Control)
    {
        //out << "Released Control" << endl;
        isControlPressed = false;
    }

    if(event->key() == Qt::Key_Delete)
    {
        //out << "Released Delete" << endl;
        isDeletePressed = false;

        _entities->deleteSelected();
    }
}

void GLWidget3DEngine::resetViewingTransformations()
{
    _pos = glm::vec3(0.0f);
    _scale = glm::vec3(1.0f);

    _mouseIncr.setX(0);
    _mouseIncr.setY(0);
}

void GLWidget3DEngine::interactionControlPointExistingEntity(QPoint mousePosition)
{
    // ============   Existing Entity Interaction   ============
    bool xySelection =
            (
                ( isControlPressed && _entities->selected()->getControlPoint(_selectedID)->isConstraintedAs(CONSTRAINT_TRANSLATION_Y) )
                ||
                ( _entities->selected()->getControlPoint(_selectedID)->isConstraintedAs(CONSTRAINT_RESIZE_X) && !_entities->selected()->getControlPoint(_selectedID)->isConstraintedAs(CONSTRAINT_RESIZE_Z) )
                ||
                ( _entities->selected()->getControlPoint(_selectedID)->isConstraintedAs(CONSTRAINT_RESIZE_Y) )
            );

    if(!xySelection)
    {
        float y = _entities->selected()->position().y;
        glm::vec4 coords = xzOffscreenCoordSelection(mousePosition.x(), mousePosition.y(), y);

        //out << "coords = (" << coords.x << "," << coords.y << "," << coords.z << "," << coords.w << ")" << endl;

        if(coords.w == 1.0f)
        {
            _entities->selected()->interact(_selectedID, glm::vec3(coords));
            _entities->emitModifiedInformation();
        }
    }
    else
    {
        float z = _entities->selected()->position().z;
        glm::vec4 coords = xyOffscreenCoordSelection(mousePosition.x(), mousePosition.y(), z);

        //out << "coords = (" << coords.x << "," << coords.y << "," << coords.z << "," << coords.w << ")" << endl;

        if(coords.w == 1.0f)
        {
            _entities->selected()->interact(_selectedID, glm::vec3(coords));
            _entities->emitModifiedInformation();
        }
    }
}

void GLWidget3DEngine::interactionVisualizationSceneMovement(QPoint mousePosition)
{
    // ============   Visualization Scene Movement   ============
    _pos -= glm::vec3((mousePosition.x() - lastMousePos.x()) * (image.cols/(float)viewportWidth),
                      (-(mousePosition.y() - lastMousePos.y())) * (image.rows/(float)viewportHeight),
                      0.0f);

    _mouseIncr.setX(_mouseIncr.x() + mousePosition.x() - lastMousePos.x());
    _mouseIncr.setY(_mouseIncr.y() + mousePosition.y() - lastMousePos.y());

    // update the last position so we can use it to find the distance the mouse has travelled.
    lastMousePos = mousePosition;
}

void GLWidget3DEngine::cleanInteraction()
{
    if(_interactionState == ADDING_PRISM
            || _interactionState == ADDING_CYLINDER
            || _interactionState == ADDING_OPERATION)
    {
        if(tempEntity != 0)
        {
            delete tempEntity;
            tempEntity = 0;
        }
        _interactionState = NO_INTERACTION;
    }
}

void GLWidget3DEngine::interactionSelectionTool()
{
    cleanInteraction();

    _entities->deselect();
}

void GLWidget3DEngine::interactionAddPrism()
{
    cleanInteraction();

    float size = image.cols / 10.0f;

    makeCurrent();
    tempEntity = new Prism(&_idManager, size, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
    tempEntity->setColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());

    _interactionState = ADDING_PRISM;
}

void GLWidget3DEngine::interactionAddCylinder()
{
    cleanInteraction();

    float size = image.cols / 10.0f;

    makeCurrent();
    tempEntity = new Cylinder(&_idManager, size, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
    tempEntity->setColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());

    _interactionState = ADDING_CYLINDER;
}

void GLWidget3DEngine::interactionAddOperation()
{
    cleanInteraction();

    makeCurrent();
    tempEntity = new Operation(&_idManager, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
    tempEntity->setColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());

    _interactionState = ADDING_OPERATION;
}

void GLWidget3DEngine::setEntitiesRenderMode(EntitiesRenderMode entitiesRenderMode)
{
    _entitiesRenderMode = entitiesRenderMode;
    updateGL();
}

void GLWidget3DEngine::setFloorRenderMode(FloorRenderMode floorRenderMode)
{
    _floorRenderMode = floorRenderMode;
    updateGL();
}

void GLWidget3DEngine::setConfiguration(Configuration * configuration)
{
    _configuration = configuration;
}

void GLWidget3DEngine::reApplyConfiguration()
{
    makeCurrent();

    floorPlane.setGridColor(_configuration->floorWires());

    // Paint the temporal entity (whose is position associated to the mouse position) if it's being added.
    if(tempEntity != 0)
    {
        tempEntity->setColor(_configuration->entityWires());
        tempEntity->setColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());
    }

    // Apply the configuration to all the entities added
    _entities->applyConfigurationSetColor(_configuration->entityWires());
    _entities->applyConfigurationSetColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());

    // Repaint
    repaint();
}

void GLWidget3DEngine::setCameraHeight(int height)
{
    //out << "Camera height: " << height << endl;
    _virtualScenePerspectiveCamera.setHeight(height);
    repaint();
}

void GLWidget3DEngine::setCameraPitch(float angle)
{
    //out << "Pitch angle: " << angle << endl;
    _virtualScenePerspectiveCamera.setPitch(angle);
    repaint();
}

void GLWidget3DEngine::setCameraDistance(float distance)
{
    //out << "Camera distance: " << distance << endl;
    _cameraDistanceModifier = distance;

    _cameraDistance = (image.rows / 2.0f) / tan( (_cameraFOV/2.0f) * PI / 180.0f );
    _cameraDistance += _cameraDistanceModifier;

    _virtualScenePerspectiveCamera.setDistance(_cameraDistance);

    repaint();
}

void GLWidget3DEngine::setEntityTreeController(EntityTreeController * entities)
{
    _entities = entities;
}

EntityTreeNode * GLWidget3DEngine::createNodesFromXML(QDomNode xmlNode)
{
    Entity3D * entity;
    EntityTreeNode * node;

    EntityType type = EntityType(xmlNode.firstChildElement("type").text().toInt());

    makeCurrent();
    switch(type)
    {
    case ROOT:
        entity = new RootEntity();
        break;
    case PRISM:
        entity = new Prism(&_idManager, xmlNode, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
        break;
    case CYLINDER:
        entity = new Cylinder(&_idManager, xmlNode, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
        break;
    case OPERATION:
        entity = new Operation(&_idManager, xmlNode, colorShaderID, colorShaderID, pickingShaderID, _configuration->entityWires());
        break;
    }

    node = new EntityTreeNode(entity);


    // Children.
    QDomNode xmlChildren = xmlNode.firstChildElement("children");
    QDomNode xmlChild = xmlChildren.firstChildElement("node");
    while(!xmlChild.isNull())
    {
        EntityTreeNode * child = createNodesFromXML(xmlChild);
        node->addChild(child);

        xmlChild = xmlChild.nextSiblingElement("node");
    }

    return node;
}

void GLWidget3DEngine::importEntitiesFromXML(QString xml)
{
    QDomDocument document;
    QDomElement csgtreeElement;

    document.setContent(xml);
    csgtreeElement = document.documentElement();

    EntityTreeNode * importedTree = createNodesFromXML(csgtreeElement.firstChildElement("node"));

    if(importedTree->children.size() > 0)
    {
        foreach(EntityTreeNode * child, importedTree->children)
        {
            // Since the controller already has a root, all that's needed is to
            // graft the branches from the imported tree into the controller's.
            _entities->pruneAndGraftAsPrimitive(child);
            //out << "added the child:" << endl;
            //child->entity->printToConsole();
            //printGLError();
        }
    }

    reApplyConfiguration();

    delete importedTree;
    importedTree = 0;

    updateGL();
}

cv::Mat GLWidget3DEngine::renderSceneToImageAsSolidAllIlluminated()
{
    makeCurrent();
    S3DFBO * offscreenFBO = new S3DFBO(image.cols, image.rows,
                                    GL_RGB,GL_RGB,GL_FLOAT,GL_LINEAR,
                                    true,true);

    _doNotUpdateGL = true;

    // Prepare the rendering configurations.
    _entities->deselect();
    resetViewingTransformations();

    Configuration tempConfig = Configuration();
    tempConfig.setEntityWires(QColor(0, 255, 0, 255));
    Configuration * savedConfig = _configuration;
    setConfiguration(&tempConfig);
    reApplyConfiguration();



    glm::vec4 savedBGColor = _backgroundColor;
    _backgroundColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red BG, so it's easily noticed.

    EntitiesRenderMode savedEntitiesRenderMode = _entitiesRenderMode;
    _entitiesRenderMode = ILLUMINATED_SOLID;
    FloorRenderMode savedFloorRenderMode = _floorRenderMode;
    _floorRenderMode = NO_FLOOR;

    // Render offscreen and store the result on an image.
    offscreenFBO->renderFBO();
    {
        paintVirtualScene(false, false, false);
    }
    offscreenFBO->renderFramebuffer();

    cv::Mat result = fboTexturetoImage(offscreenFBO, image.rows, image.cols);


    cv::Mat corrected = cv::Mat(image.rows, image.cols, CV_8UC3);
    corrected.setTo(cv::Scalar(0,0,255)); // This one is BGR

    int offset = 15;
    cv::Mat cropped = result(cv::Rect(offset, 0, image.cols-offset, image.rows));
    cropped.copyTo(corrected);

    cv::imwrite("debugSolidRendering.png", corrected);



    // Restore everything back to how it was before rendering.
    setConfiguration(savedConfig);
    reApplyConfiguration();

    _backgroundColor = savedBGColor;

    _entitiesRenderMode = savedEntitiesRenderMode;
    _floorRenderMode = savedFloorRenderMode;

    delete offscreenFBO;
    offscreenFBO = 0;

    _doNotUpdateGL = false;

    updateGL();

    return corrected;
}

cv::Mat GLWidget3DEngine::renderSceneToImageAsSolidOneEntityIlluminated(Entity3D * illuminated)
{
    makeCurrent();
    S3DFBO * offscreenFBO = new S3DFBO(image.cols, image.rows,
                                    GL_RGB,GL_RGB,GL_FLOAT,GL_LINEAR,
                                    true,true);

    _doNotUpdateGL = true;

    // Prepare the rendering configurations.
    _entities->deselect();
    resetViewingTransformations();

    Configuration tempConfig = Configuration();
    tempConfig.setEntityWires(QColor(0, 255, 0, 255));
    Configuration * savedConfig = _configuration;
    setConfiguration(&tempConfig);
    reApplyConfiguration();



    glm::vec4 savedBGColor = _backgroundColor;
    _backgroundColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red BG, so it's easily noticed.


    // Render offscreen and store the result on an image.
    offscreenFBO->renderFBO();
    {
        // Background color
        glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Draw PERSPECTIVE
        projectionMode = PERSPECTIVE;
        resetProjection_VirtualScene();
        view = _virtualScenePerspectiveCamera.view();

        // Draw all models.
        if(_isImageLoaded)
        {
            // Draw all the entities added
            QVector<Entity3D *> * allEntities = _entities->getNonConflictingEntities(illuminated);
            foreach(Entity3D * entity, *allEntities)
            {
                if(entity == illuminated)
                {
                    tempConfig.setEntityWires(QColor(0, 255, 0, 255));
                    setConfiguration(&tempConfig);
                    reApplyConfiguration();
                    // Uses the Fixed-Function Pipe
                    entity->paint(_selectionMode, &view, &projection, _entities->selected(), ILLUMINATED_SOLID);
                }
                else
                {
                    tempConfig.setEntityWires(QColor(255, 0, 0, 255));
                    setConfiguration(&tempConfig);
                    reApplyConfiguration();
                    // Uses the defined shaders
                    entity->paint(_selectionMode, &view, &projection, _entities->selected(), DARK_SOLID);
                }
            }
            allEntities->clear();
            if(allEntities != 0)
            {
                delete allEntities;
                allEntities = 0;
            }
        }
    }
    offscreenFBO->renderFramebuffer();

    cv::Mat result = fboTexturetoImage(offscreenFBO, image.rows, image.cols);

    cv::Mat corrected = cv::Mat(image.rows, image.cols, CV_8UC3);
    corrected.setTo(cv::Scalar(0,0,255)); // This one is BGR

    int offset = 15;
    cv::Mat cropped = result(cv::Rect(offset, 0, image.cols-offset, image.rows));
    cropped.copyTo(corrected);

    cv::imwrite(QString("debugSolidRendering " + illuminated->name() + ".png").toStdString(), corrected);



    // Restore everything back to how it was before rendering.
    setConfiguration(savedConfig);
    reApplyConfiguration();

    _backgroundColor = savedBGColor;


    delete offscreenFBO;
    offscreenFBO = 0;

    _doNotUpdateGL = false;

    updateGL();

    return corrected;
}


void GLWidget3DEngine::generateSolidRenderings()
{
    _entities->correctEntityPositions();

    QVector<EntityTreeNode *> * nodes = _entities->traverseBreadthFirst();

    foreach(EntityTreeNode * node, *nodes)
    {
        cv::Mat solidRendering;
        if(node->hasParent())
        {
            solidRendering = renderSceneToImageAsSolidOneEntityIlluminated(node->getEntity());
        }
        else
        {
            solidRendering = cv::Mat(image.rows, image.cols, CV_8UC3);
            solidRendering.setTo(cv::Scalar(0,0,255)); // This one is BGR
            cv::imwrite(QString("debugSolidRendering " + node->getEntity()->name() + ".png").toStdString(), solidRendering);
        }
        node->setSolidRendering(solidRendering);
    }
}


