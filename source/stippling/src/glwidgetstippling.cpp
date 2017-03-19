/**
 * @file glwidgetstippling.cpp
 * @brief GLWidgetStippling source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/April/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "glwidgetstippling.h"

cv::Mat GLWidgetStippling::fboTexturetoImage(S3DFBO * fbo, int height, int width)
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

QString GLWidgetStippling::checkGLError()
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

void GLWidgetStippling::glSetup()
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
    out << "===   Stippling viewport   ===" << endl;
    out << "===   OpenGL initialized   ===" << endl;
    out << "Vendor: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) << endl;
    out << "Renderer: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) << endl;
    out << "Version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << endl;
    out << "GLSL version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << endl;
    out << "Status: Using GLEW " << QLatin1String(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))) << endl;
    out << "==============================" << endl;
    out << endl;
}

GLuint GLWidgetStippling::prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath)
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

void GLWidgetStippling::initializeAxes()
{
    float max = 50000.0f;

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

void GLWidgetStippling::initializeSprite(GLEntity & dot, glm::vec2 spriteSize,
                                         glm::vec2 spritesMatrixSize,
                                         glm::vec2 spritesMatrixPosition)
{
    const GLfloat vertexBufferData[] =
    {
        0.0f, 0.0f, 0.0f,
        spriteSize.x, 0.0f, 0.0f,
        spriteSize.x, spriteSize.y, 0.0f,

        spriteSize.x, spriteSize.y, 0.0f,
        0.0f , spriteSize.y, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    // One color for each vertex.
    const GLfloat * colorBufferData = 0;

    // One normal for each vertex.
    const GLfloat * normalBufferData = 0;

    float unitX = 1.0f / float(spritesMatrixSize.x);
    float unitY = 1.0f / float(spritesMatrixSize.y);

    float minX = spritesMatrixPosition.x * unitX;
    float maxX = (spritesMatrixPosition.x + 1) * unitX;
    float minY = spritesMatrixPosition.y * unitY;
    float maxY = (spritesMatrixPosition.y + 1) * unitY;

    // One uv coord for each vertex.
    const GLfloat uvBufferData[] =
    {
        minX, maxY,
        maxX, maxY,
        maxX, minY,

        maxX, minY,
        minX, minY,
        minX, maxY,
    };

    int numberOfValuesInStaticBuffers = sizeof(vertexBufferData)/sizeof(GLfloat);

    dot.initialize(vertexBufferData, colorBufferData, normalBufferData, uvBufferData,
                   numberOfValuesInStaticBuffers, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                   textureShaderID, pickingShaderID);
}

void GLWidgetStippling::initializeSprites()
{
    spritesImg = cv::imread("./resources/precdots10alpha.png", CV_LOAD_IMAGE_UNCHANGED);

    if(! spritesImg.data ) // Check for invalid input
    {
        QMessageBox::information(this, tr("PROJECT"), tr("Could not open or find the image \"/resources/sprites.png\"."));
        return;
    }

    glGenTextures( 1, &spritesTextureID );
    glBindTexture( GL_TEXTURE_2D, spritesTextureID );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spritesImg.cols, spritesImg.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, spritesImg.ptr());

    // TODO REFACTOR Create a SpriteMatrix class to take care of this responsibility
    spriteSize = glm::vec2(10, 10);
    spritesMatrixSize = glm::vec2(29,29);

    sprites.clear();
    sprites.reserve(spritesMatrixSize.x * spritesMatrixSize.y);
    for(int i=0; i<spritesMatrixSize.x * spritesMatrixSize.y; ++i)
    {
        int id = _idManager.getNewID();
        GLEntity dot = GLEntity(id, _idManager.encodeID(id));

        int chosenRow = int(i / spritesMatrixSize.x);
        int chosenColumn = i % int(spritesMatrixSize.x);

        initializeSprite(dot, spriteSize, spritesMatrixSize, glm::vec2(chosenColumn, chosenRow));

        sprites.push_back(dot);
    }
    // Now sprites holds all the dots from the sprite matrix.
    // To represent a StippleDot all that's necessary is to acess the sprites array
    // using the chosen dot integer value as index and invoke its paint method.
}

void GLWidgetStippling::initializeStipplingTextureHolder(int rows, int cols)
{
    const GLfloat vertexBufferData[] =
    {
        0.0f, 0.0f, 0.0f,
        cols, 0.0f, 0.0f,
        cols, rows, 0.0f,

        cols, rows, 0.0f,
        0.0f , rows, 0.0f,
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

    _stipplingTextureHolder.initialize(vertexBufferData, colorBufferData,
                               normalBufferData, uvBufferData,
                               numberOfValuesInStaticBuffers, GL_TRIANGLES, 3, GL_STATIC_DRAW,
                               textureShaderID, pickingShaderID);

    _stipplingTextureHolderRows = rows;
    _stipplingTextureHolderCols = cols;
}

GLWidgetStippling::GLWidgetStippling(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

	isLeftButtonPressed = false;
    isMiddleButtonPressed = false;
    isRightButtonPressed = false;

    viewportWidth = 1;
    viewportHeight = 1;

    cameraDistance = 0.0f;
    cameraZoom = 1.0f;
    cameraPosOffsetX = 0.0f;
    cameraPosOffsetY = 0.0f;

    _backgroundColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    _idManager = IDManager(_backgroundColor);
    _selectionMode = OFF;
    _selectedID = _idManager.NONE();

    int id;

    id = _idManager.getNewID();
    axesObj = GLEntity(id, _idManager.encodeID(id));

    _fboStipplingToTexture = 0;
    _stipplingTextureHolder = GLEntity(_idManager.NONE(),_idManager.encodeID(_idManager.NONE()));
    _isStipplingTextureReady = false;
    _stipplingTextureID = 0;

    spritesTextureID = 0;

    _stipplingDots = 0;
    _dotGenerationWorker = 0;
    _dotGenerationWorkerThread = 0;

    isDeletePressed = false;

    _areStipplingTexturesReady = false;

    _renderUsingMultipleTiles = false;

    _stipplingPerformedAtLeastOnce = false;

    _entities = 0;
}

GLWidgetStippling::~GLWidgetStippling()
{
    if(_fboStipplingToTexture != 0)
    {
        delete _fboStipplingToTexture;
        _fboStipplingToTexture = 0;
    }

    if(_stipplingDots != 0)
    {
        delete _stipplingDots;
        _stipplingDots = 0;
    }

    if(_dotGenerationWorkerThread != 0)
    {
        _dotGenerationWorkerThread->quit();
        _dotGenerationWorkerThread->deleteLater();
        _dotGenerationWorkerThread = 0;
    }
}

void GLWidgetStippling::initializeGL()
{
    glSetup();

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int id;

     id = _idManager.getNewID();
     _xyPlane = InfinitePlaneZ0(id, _idManager.encodeID(id), xyColorShaderID, pickingShaderID);

    initializeAxes();

    initializeSprites();
}

void GLWidgetStippling::resizeGL(int w, int h)
{
    viewportWidth = w;
    viewportHeight = h;
    glViewport(0, 0, w, h);
}

void GLWidgetStippling::resetProjection(bool isTileRenderingModeON)
{
    if(!isTileRenderingModeON)
    {
        cameraDistance = 0.0f;

        zNear = cameraDistance - 100.0f;
        zFar = cameraDistance + 100.0f;

        cameraPosX = cameraZoom * stippledImageCols/2.0f;
        cameraPosY = cameraZoom * stippledImageRows/2.0f;

        projection = glm::ortho(-cameraPosX, cameraPosX,
                                -cameraPosY, cameraPosY,
                                zNear, zFar);

        /*
        out << "Camera position = ( " << cameraPosX << ", " << cameraPosY << " )" << endl;
        out << "Camera position offset = ( " << cameraPosOffsetX << ", " << cameraPosOffsetY << " )" << endl;
        out << "Camera zoom = " << cameraZoom << endl;
        */
    }
    else
    {
        projection = glm::ortho(-tiling_cameraPosX, tiling_cameraPosX,
                                -tiling_cameraPosY, tiling_cameraPosY,
                                tiling_zNear, tiling_zFar);
    }
}

void GLWidgetStippling::paintGL()
{
    if(_renderUsingMultipleTiles)
    {
        renderUsingMultipleTiles();
    }
    else
    {
        renderUsingSingleTile();
    }
}

void GLWidgetStippling::renderUsingSingleTile()
{
    if(_isStipplingTextureReady)
    {
        float visualizationCameraPosX = _stipplingTextureHolderCols/2.0f;
        float visualizationCameraPosY = _stipplingTextureHolderRows/2.0f;

        glm::mat4 visualizationProjection;
        glm::mat4 visualizationView;
        GLCamera visualizationCamera;

        visualizationProjection = glm::ortho(-visualizationCameraPosX, visualizationCameraPosX,
                                             -visualizationCameraPosY, visualizationCameraPosY,
                                             -10.0f, 10.0f);

        visualizationCamera.reset();
        visualizationCamera.translate(glm::vec3(visualizationCameraPosX,
                                         visualizationCameraPosY,
                                         0.0f));
        visualizationView = visualizationCamera.view();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Background color
        glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

        glDisable(GL_DEPTH_TEST);

        glBindTexture( GL_TEXTURE_2D, _stipplingTextureID );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

        // It will paint itself normally, independently of the selection mode.
        _stipplingTextureHolder.paint(OFF, &visualizationView, &visualizationProjection, 0, _stipplingTextureID, 0, 0, 0, 0, 0, 0, false);
    }
}

void GLWidgetStippling::renderUsingMultipleTiles()
{
    // Background color
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    if(_areStipplingTexturesReady)
    {
        float visualizationCameraPosX = cameraZoom * stippledImageCols/2.0f;
        float visualizationCameraPosY = cameraZoom * stippledImageRows/2.0f;

        glm::mat4 visualizationProjection;
        glm::mat4 visualizationView;
        GLCamera visualizationCamera;

        visualizationProjection = glm::ortho(-visualizationCameraPosX, visualizationCameraPosX,
                                             -visualizationCameraPosY, visualizationCameraPosY,
                                             -10.0f, 10.0f);

        visualizationCamera.reset();
        visualizationCamera.translate(glm::vec3(visualizationCameraPosX,
                                         visualizationCameraPosY,
                                         0.0f));
        visualizationView = visualizationCamera.view();

        for(int i=0; i<_tilesFBOs.size(); ++i)
        {
            glBindTexture( GL_TEXTURE_2D, _tilesFBOs.at(i)->getTexture() );
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

            _stipplingTextureHolder.resetModel();
            _stipplingTextureHolder.translate(_tilesPos.at(i));
            //out << "Rendering tile " << i << " whose texture id is " << _tilesFBOs.at(i)->getTexture() << " at:" << endl;
            //Util::printVector(_tilesPos.at(i));

            // It will paint itself normally, independently of the selection mode.
            _stipplingTextureHolder.paint(OFF, &visualizationView, &visualizationProjection, 0, _tilesFBOs.at(i)->getTexture(), 0, 0, 0, 0, 0, 0, false);
        }
    }
}

void GLWidgetStippling::paintScene(bool isCoordCaptureModeON, bool isTileRenderingModeON)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Background color
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

    glDisable(GL_DEPTH_TEST);

    resetProjection(isTileRenderingModeON);
    if(!isTileRenderingModeON)
    {
        _sceneCamera.reset();
        _sceneCamera.translate(glm::vec3(cameraPosX + cameraPosOffsetX,
                                         cameraPosY + cameraPosOffsetY,
                                         cameraDistance));
        view = _sceneCamera.view();
    }
    else
    {
        tiling_sceneCamera.reset();
        tiling_sceneCamera.translate(glm::vec3(tiling_cameraPosX + tiling_cameraPosOffsetX,
                                         tiling_cameraPosY + tiling_cameraPosOffsetY,
                                         tiling_cameraDistance));
        view = tiling_sceneCamera.view();
    }



    if(isCoordCaptureModeON)
    {

    }
    else
    {
        //axesObj.paint(_selectionMode, &view, &projection, 0, 0, 0, 0, 0, 0, 0, 0, false);

        int numDotsRendered = 0;

        int startX = cameraPosOffsetX;
        int endX = 2*cameraPosX + cameraPosOffsetX;
        int startY = cameraPosOffsetY;
        int endY = 2*cameraPosY + cameraPosOffsetY;

        if(isTileRenderingModeON)
        {
            startX = tiling_cameraPosOffsetX;
            endX = 2*tiling_cameraPosX + tiling_cameraPosOffsetX;
            startY = tiling_cameraPosOffsetY;
            endY = 2*tiling_cameraPosY + tiling_cameraPosOffsetY;
        }

        glm::vec4 renderArea = glm::vec4(startX, startY, endX, endY);
        int padding = qMax(spriteSize.x, spriteSize.y);



        /*
        out << "Render area:" << endl;
        Util::printVector(renderArea);
        out << "Padding: " << padding << endl;;
        */
        if(_stipplingDots != 0)
        {
            glBindTexture( GL_TEXTURE_2D, spritesTextureID );

            foreach(QVector<StippleDot *> * subvector, _stipplingDots->getDotsInPaddedArea(renderArea, padding))
            {
                foreach(StippleDot * dot, *subvector)
                {
                    glm::mat4 model = dot->model();
                    sprites.at(dot->chosenDot()).paint(_selectionMode, &view, &projection, &model, spritesTextureID, 0, 0, 0, 0, 0, 0);

                    ++numDotsRendered;
                }
            }
        }

        //out << numDotsRendered << " dots were rendered" << endl;
    }
}

void GLWidgetStippling::singleTileRenderCurrentScene()
{
    //out << "Rendering the Stippling Scene to texture..." << endl;

    int tileWidth = viewportWidth;
    int tileHeight = viewportHeight;

    if(_fboStipplingToTexture == 0)
    {
        //out << "Creating an fbo to dump the rendering as texture with resolution of: " << tileWidth << " x " << tileHeight << endl;
        _fboStipplingToTexture = new S3DFBO(tileWidth, tileHeight,
                         GL_RGB,GL_RGB,GL_FLOAT,GL_NEAREST,
                         true,true);

        _stipplingTextureID = _fboStipplingToTexture->getTexture();
    }

    initializeStipplingTextureHolder(tileHeight, tileWidth);

    resetProjection(false);

    _fboStipplingToTexture->renderFBO();
        paintScene(false, false);
    _fboStipplingToTexture->renderFramebuffer();

    //cv::Mat img = fboTexturetoImage(_fboStipplingToTexture, _stipplingTextureHolderRows, _stipplingTextureHolderCols);
    //cv::imwrite("stipplingTextureRendered.png", img);

    _isStipplingTextureReady = true;

    //out << "Rendered the Stippling Scene to texture." << endl;
}

void GLWidgetStippling::tileRenderCurrentScene()
{
    _areStipplingTexturesReady = false;

    foreach(S3DFBO * fbo, _tilesFBOs)
    {
        if(fbo != 0)
        {
            delete fbo;
            fbo = 0;
        }
    }
    _tilesFBOs.clear();
    _tilesPos.clear();



    int tileWidth = _configuration->tileWidth();
    int tileHeight = _configuration->tileHeight();

    initializeStipplingTextureHolder(tileHeight, tileWidth);

    int renderAreaRows = cameraZoom * stippledImageRows;
    int renderAreaCols = cameraZoom * stippledImageCols;

    float iMaxFloat = renderAreaRows / float(tileHeight);
    float jMaxFloat = renderAreaCols / float(tileWidth);
    int iMax = renderAreaRows / tileHeight;
    int jMax = renderAreaCols / tileWidth;
    if(iMaxFloat - iMax > 0.0f)
    {
        ++iMax;
    }
    if(jMaxFloat - jMax > 0.0f)
    {
        ++jMax;
    }

    /*
    out << "stippledImageRows: " << stippledImageRows << endl;
    out << "stippledImageCols: " << stippledImageCols << endl;
    out << "cameraZoom: " << cameraZoom << endl;
    out << "renderAreaRows: " << renderAreaRows << endl;
    out << "renderAreaCols: " << renderAreaCols << endl;
    out << "tileHeight: " << tileHeight << endl;
    out << "tileWidth: " << tileWidth << endl;
    out << "iMax: " << iMax << endl;
    out << "jMax: " << jMax << endl;
    */

    for(int i=0; i<iMax; ++i) // Tile matrix Rows
    {
        for(int j=0; j<jMax; ++j) // Tile matrix Columns
        {
            //out << "i: " << i << endl;
            //out << "j: " << j << endl;

            // Set the projection/view so the scene rendered is the appropiate tile.
            tiling_cameraDistance = 0.0f;

            tiling_zNear = tiling_cameraDistance - 100.0f;
            tiling_zFar = tiling_cameraDistance + 100.0f;

            tiling_cameraPosX = tileWidth/2.0f;
            tiling_cameraPosY = tileHeight/2.0f;

            tiling_cameraPosOffsetX = (j * tileWidth) + cameraPosOffsetX;
            tiling_cameraPosOffsetY = (i * tileHeight) + cameraPosOffsetY;


            S3DFBO * tileFBO = new S3DFBO(tileWidth,tileHeight,
                                          GL_RGB,GL_RGB,GL_FLOAT,GL_NEAREST,
                                          true,true);

            // Activate the tiling fbo so all rendering occurs off screen.
            tileFBO->renderFBO();

            // Render the tile.
            paintScene(false, true);

            // Deactivate the tiling fbo so all rendering occurs on screen.
            tileFBO->renderFramebuffer();

            _tilesFBOs.push_back(tileFBO);
            _tilesPos.push_back(glm::vec3((j * tileWidth), (i * tileHeight), 0));
        }
    }

    _areStipplingTexturesReady = true;
}

void GLWidgetStippling::stippleImage(cv::Mat toStipple, DotGenerationWorker::DitheringMethod ditheringMethod,
                                     cv::Mat solid3DModel)
{
    _isStipplingTextureReady = false;

    resetViewingTransformations();

    if(_stipplingDots != 0)
    {
        delete _stipplingDots;
        _stipplingDots = 0;
    }

    if(_fboStipplingToTexture != 0)
    {
        delete _fboStipplingToTexture;
        _fboStipplingToTexture = 0;
    }

    _dotGenerationWorkerThread = new QThread();

    _dotGenerationWorker = new DotGenerationWorker(toStipple, ditheringMethod, spriteSize, spritesMatrixSize,
                                                  solid3DModel,
                                                  _configuration,
                                                  _entities);

    _dotGenerationWorker->moveToThread(_dotGenerationWorkerThread);

    connect(_dotGenerationWorkerThread, SIGNAL(started()), _dotGenerationWorker, SLOT(process()));
    connect(_dotGenerationWorker, SIGNAL(finished()), this, SLOT(stippleImageEnded()));

    _dotGenerationWorkerThread->start();
}

void GLWidgetStippling::stippleImageEnded()
{
    _stipplingDots = _dotGenerationWorker->getStipplingDots();

    //out << "Got Stippling dots, deleting the worker thread..." << endl;

    if(_dotGenerationWorkerThread != 0)
    {
        _dotGenerationWorkerThread->quit();
        _dotGenerationWorkerThread->deleteLater();
        _dotGenerationWorkerThread = 0;
    }

    //out << "Worker thread deleted." << endl;

    glm::vec4 quadTreeArea = _stipplingDots->getRootArea();

    stippledImageRows = quadTreeArea.w;
    stippledImageCols = quadTreeArea.z;

    //out << "Stippled image size: " << stippledImageRows << " x " << stippledImageCols << " px" << endl;

    out << "First texture render starting..." << endl;
    // Single texture rendering mode first rendering
    tileRenderCurrentScene();
    tileRenderCurrentScene();
    // Multiple textures rendering mode first rendering
    singleTileRenderCurrentScene();
    singleTileRenderCurrentScene();
    out << "First texture render finished." << endl;

    out << "First onscreen rendering starting..." << endl;
    updateGL();
    out << "First onscreen rendering finished." << endl;

    _stipplingPerformedAtLeastOnce = true;
}








void GLWidgetStippling::selectGL (int x, int y)
{
    _selectionMode = ON;

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
    out << "decodedID = " << decodedID << endl;
    */

    /*
    foreach(StippleDot dot, _stipplingDots)
    {
        if(decodedID == dot.id())
        {
            out << "Selected the StippleDot whose id is " << decodedID << "." << endl;
        }
    }
    */

    _selectionMode = OFF;
    updateGL();
}






void GLWidgetStippling::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        selectGL(event->pos().x(), event->pos().y());

        isLeftButtonPressed = true;
    }
    if(event->button() == Qt::MidButton)
    {
        out << endl;
        out << "################ GL STATUS = " << checkGLError()  << " ################" << endl;
        out << endl;

        isMiddleButtonPressed = true;
    }
    if(event->button() == Qt::RightButton)
    {
        lastMousePos = event->pos();

        isRightButtonPressed = true;
    }
}

void GLWidgetStippling::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::NoButton)
    {

    }
    if(isLeftButtonPressed)
    {

    }
    if(isRightButtonPressed)
    {
        interactionMoveCamera(event->pos());

        if(_renderUsingMultipleTiles)
        {
            tileRenderCurrentScene();
        }
        else
        {
            singleTileRenderCurrentScene();
        }
    }

    updateGL();
}

void GLWidgetStippling::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {

        isLeftButtonPressed = false;
    }
    if(event->button() == Qt::MidButton)
    {

        isMiddleButtonPressed = false;
    }
    if(event->button() == Qt::RightButton)
    {
        interactionMoveCamera(event->pos());

        if(_renderUsingMultipleTiles)
        {
            tileRenderCurrentScene();
        }
        else
        {
            singleTileRenderCurrentScene();
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

void GLWidgetStippling::wheelEvent(QWheelEvent *event)
{
    // Correction due to different mouse wheel's precision (degrees per step)
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    float zoomFactor = 25.0;

    cameraZoom -= numSteps/zoomFactor;

    if(cameraZoom <= 0.0f)
    {
        cameraZoom = 1.0f / zoomFactor;
    }
    if(cameraZoom >= 1.0f)
    {
        cameraZoom = 1.0f;
    }

    if(_renderUsingMultipleTiles)
    {
        tileRenderCurrentScene();
    }
    else
    {
        singleTileRenderCurrentScene();
    }

    updateGL();

    event->accept();
}

void GLWidgetStippling::keyPressEvent(QKeyEvent *event)
{
    //out << "Pressed " << event->key() << endl;
    if(event->key() == Qt::Key_Delete)
    {
        //out << "Pressed Control" << endl;
        isDeletePressed = true;
    }
}

void GLWidgetStippling::keyReleaseEvent(QKeyEvent *event)
{
    //out << "Released " << event->key() << endl;
    if(event->key() == Qt::Key_Delete)
    {
        //out << "Released Control" << endl;
        isDeletePressed = false;
    }
}

void GLWidgetStippling::interactionMoveCamera(QPoint mousePosition)
{
    cameraPosOffsetX += cameraZoom * ( -(mousePosition.x() - lastMousePos.x()) * (stippledImageCols/(float)viewportWidth) );
    cameraPosOffsetY += cameraZoom * ( (mousePosition.y() - lastMousePos.y()) * (stippledImageRows/(float)viewportHeight) );

    // update the last position so we can use it to find the distance the mouse has travelled.
    lastMousePos = mousePosition;
}









void GLWidgetStippling::resetViewingTransformations()
{
    cameraZoom = 1.0f;
    cameraPosOffsetX = 0.0f;
    cameraPosOffsetY = 0.0f;

    if(_fboStipplingToTexture != 0)
    {
        if(_renderUsingMultipleTiles)
        {
            tileRenderCurrentScene();
        }
        else
        {
            singleTileRenderCurrentScene();
        }
    }
    updateGL();
}





void GLWidgetStippling::applyStippleDotDispersion()
{
    unsigned int dispersion = _configuration->stippleDotDispersion();
    dispersion /= 10; // Factor down the dispersion, so it can be better tuned.

    if(_stipplingDots != 0)
    {
        srand(_configuration->rngSeed());

        foreach(QVector<StippleDot *> * subvector, _stipplingDots->getDotsInFullArea())
        {
            foreach(StippleDot * dot, *subvector)
            {
                float alpha = rand() % 360;
                float length = rand() % (dispersion + 1);

                int offsetX = int(length * cos(alpha));
                int offsetY = int(length * sin(alpha));

                if(dispersion == 0)
                {
                    offsetX = 0;
                    offsetY = 0;
                }

                glm::vec2 offset = glm::vec2(offsetX, offsetY);
                dot->setOffset(offset);
            }
        }
    }

    if(_renderUsingMultipleTiles)
    {
        tileRenderCurrentScene();
    }
    else
    {
        singleTileRenderCurrentScene();
    }

    updateGL();
}



void GLWidgetStippling::saveStippledImageToDisk(QString fileName)
{
    // THIS ALGORITHM IS OPTIMIZED TO REDUCE MEMORY CONSUMPTION WHEN WRITING
    // THE FULL STIPPLED IMAGE TO DISK.

    out << "Beginning tile rendering process..." << endl;

    S3DFBO *fboTiling;

    // Maximum possible fbo size (both dimensions are equal).
    GLint dims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);

    int tileWidth = dims[0];
    int tileHeight = dims[0];

    tileWidth = 500;
    tileHeight = 500;

    fboTiling = new S3DFBO(tileWidth, tileHeight,
                     GL_RGB,GL_RGB,GL_FLOAT,GL_NEAREST,
                     true,true);

    float iMaxFloat = stippledImageRows / float(tileHeight);
    float jMaxFloat = stippledImageCols / float(tileWidth);
    int iMax = stippledImageRows / tileHeight;
    int jMax = stippledImageCols / tileWidth;
    if(iMaxFloat - iMax > 0.0f)
    {
        ++iMax;
    }
    if(jMaxFloat - jMax > 0.0f)
    {
        ++jMax;
    }

    cv::Mat fullResolutionScene;
    fullResolutionScene.create(tileHeight * iMax, tileWidth * jMax, CV_8UC3);

    cv::Mat tile;
    tile.create(tileHeight, tileWidth, CV_8UC3);


    /*
    out << "stippledImageRows: " << stippledImageRows << endl;
    out << "stippledImageCols: " << stippledImageCols << endl;
    out << "tileHeight: " << tileHeight << endl;
    out << "tileWidth: " << tileWidth << endl;
    out << "iMax: " << iMax << endl;
    out << "jMax: " << jMax << endl;
    */

    for(int i=0; i<iMax; ++i) // Tile matrix Rows
    {
        for(int j=0; j<jMax; ++j) // Tile matrix Columns
        {
            //out << "i: " << i << endl;
            //out << "j: " << j << endl;

            // Set the projection/view so the scene rendered is the appropiate tile.
            tiling_cameraDistance = 0.0f;

            tiling_zNear = tiling_cameraDistance - 100.0f;
            tiling_zFar = tiling_cameraDistance + 100.0f;

            tiling_cameraPosX = tileWidth/2.0f;
            tiling_cameraPosY = tileHeight/2.0f;

            tiling_cameraPosOffsetX = j * tileWidth;
            tiling_cameraPosOffsetY = i * tileHeight;

            //out << "tiling_cameraPosOffsetX: " << tiling_cameraPosOffsetX << endl;
            //out << "tiling_cameraPosOffsetY: " << tiling_cameraPosOffsetY << endl;

            // Activate the tiling fbo so all rendering occurs off screen.
            fboTiling->renderFBO();

            // Render the tile.
            paintScene(false, true);

            // Read the pixels and store them in the tile image.
            glFlush();
            glFinish();
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glPixelStorei(GL_PACK_ROW_LENGTH, tile.step/tile.elemSize());
            glReadPixels(0, 0, tile.cols, tile.rows, GL_BGR, GL_UNSIGNED_BYTE, tile.data);

            // Deactivate the tiling fbo so all rendering occurs on screen.
            fboTiling->renderFramebuffer();


            cv::Mat flipped;
            cv::flip(tile, flipped, 0);

            cv::Mat mirrored;
            cv::flip(flipped, mirrored, 1);

            /*
            std::stringstream tileName;
            tileName << "tile" << ((i*jMax)+j) << ".png";
            out << "Writing " << QString::fromStdString(tileName.str()) << " to disc." << endl;
            imwrite(tileName.str(), mirrored);
            */

            //out << "tile.cols: " << tile.cols << endl;
            //out << "tile.rows: " << tile.rows << endl;

            // Copy the data from the tile in the right area of the full resolution scene image.
            cv::Mat cropped = fullResolutionScene(cv::Rect(tiling_cameraPosOffsetX, tiling_cameraPosOffsetY, mirrored.cols, mirrored.rows));
            mirrored.copyTo(cropped);

            //out << "Completed tile " << (((i*jMax)+j)+1) << " out of " << (iMax * jMax) << " tiles." << endl;
        }
    }

    delete fboTiling;
    fboTiling = 0;

    // Crop the unrelevant parts (rendered over a larger area for simplicity's sake)
    cv::Mat cropped = fullResolutionScene(cv::Rect(0, 0, stippledImageCols, stippledImageRows));

    // Flip the image to correct the different coordinate systems that OpenGL and OpenCV use.
    cv::Mat stippledImage;
    cv::flip(cropped, stippledImage, 0);

    imwrite(fileName.toStdString(), stippledImage);

    out << "Tile rendering process ended." << endl;

    imshow("Generated image", stippledImage);
}



void GLWidgetStippling::setConfiguration(Configuration * configuration)
{
    _configuration = configuration;
}

void GLWidgetStippling::reApplyConfiguration()
{
    // Here go the actions that require to be performed right after
    // the configuration is changed and accepted.

    _renderUsingMultipleTiles = _configuration->useTileRendering();
    if(_stipplingPerformedAtLeastOnce)
    {
        if(_renderUsingMultipleTiles)
        {
            tileRenderCurrentScene();
        }
        else
        {
            singleTileRenderCurrentScene();
        }
        updateGL();
    }
}




void GLWidgetStippling::setEntityTreeController(EntityTreeController * entities)
{
    _entities = entities;
}
