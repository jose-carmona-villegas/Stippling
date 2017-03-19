/**
 * @file glwidget3denginesuperiorvisualization.cpp
 * @brief GLWidget3DEngineSuperiorVisualization source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 17/June/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "glwidget3denginesuperiorvisualization.h"

QString GLWidget3DEngineSuperiorVisualization::checkGLError()
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

void GLWidget3DEngineSuperiorVisualization::glSetup()
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
    out << "===   3D-Engine superior view viewport   ===" << endl;
    out << "===   OpenGL initialized   ===" << endl;
    out << "Vendor: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) << endl;
    out << "Renderer: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) << endl;
    out << "Version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << endl;
    out << "GLSL version: " << QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << endl;
    out << "Status: Using GLEW " << QLatin1String(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))) << endl;
    out << "==============================" << endl;
    out << endl;
}


GLuint GLWidget3DEngineSuperiorVisualization::prepareShaderProgram(const QString& vertexShaderPath, const QString& fragmentShaderPath)
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

void GLWidget3DEngineSuperiorVisualization::initializeAxes()
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

void GLWidget3DEngineSuperiorVisualization::initializeLights()
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

bool GLWidget3DEngineSuperiorVisualization::isImageLoaded() const
{
    return _isImageLoaded;
}

GLWidget3DEngineSuperiorVisualization::GLWidget3DEngineSuperiorVisualization(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(false);
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    viewportWidth = 1;
    viewportHeight = 1;

    _isImageLoaded = false;


    _backgroundColor = glm::vec4(0.2f,0.2f,0.2f,1.0f);
    _idManager = IDManager(_backgroundColor);

    int id;
    id = _idManager.getNewID();
    axesObj = GLEntity(id, _idManager.encodeID(id));

    _entities = 0;

    _testCoordCaptureRendering = false;
    _extraSpaceBehindFocalPoint = 5000.0f;

    _doNotUpdateGL = false;
}

GLWidget3DEngineSuperiorVisualization::~GLWidget3DEngineSuperiorVisualization()
{

}

void GLWidget3DEngineSuperiorVisualization::initializeGL()
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

    reApplyConfiguration();

    initializeAxes();

    initializeLights();
}

void GLWidget3DEngineSuperiorVisualization::resizeGL(int w, int h)
{
    viewportWidth = w;
    viewportHeight = h;
    glViewport(0, 0, w, h);
}

void GLWidget3DEngineSuperiorVisualization::resetProjection()
{
    _cameraDistance = (imageRows / 2.0f) / tan( (_cameraFOV/2.0f) * PI / 180.0f );



    // ORTHOGRAPHIC
    _zNear = 0.1f;
    _zFar = 100000.0f;
    float size = qMax(float(imageRows), float(imageColumns));
    projection = glm::ortho(-size, size,
                            -size, size,
                            _zNear, _zFar);

    _camera.reset();
    _camera.setPosition(glm::vec3(0.1f, 10000.0f, -100.0f));
    _camera.setPitch(-90.0f);
    view = _camera.view();

    /*
    out << "imageRows = " << imageRows << endl;
    out << "imageColumns = " << imageColumns << endl;

    out << "projection matrix:" << endl;
    Util::printMatrix(projection);

    out << "view matrix:" << endl;
    Util::printMatrix(view);
    */
}

void GLWidget3DEngineSuperiorVisualization::paintGL()
{
    if(!_doNotUpdateGL)
    {
        makeCurrent();
        paintScene();
        doneCurrent();
    }
}

void GLWidget3DEngineSuperiorVisualization::paintScene()
{
    // Background color
    glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    // Draw ORTHOGRAPHIC
    resetProjection();

    // Draw all models.
    if(_isImageLoaded)
    {
        //axesObj.paint(_selectionMode, &view, &projection, 0, 0, 0, 0, 0, 0, 0, 0, false);

        // Draw all the entities added
        QVector<Entity3D*> * visibleEntities = _entities->getVisibleEntities();
        foreach(Entity3D * entity, *visibleEntities)
        {
            entity->paintOnDifferentGLContext(&view, &projection);
        }

        visibleEntities->clear();
        if(visibleEntities != 0)
        {
            delete visibleEntities;
            visibleEntities = 0;
        }
    }
}

void GLWidget3DEngineSuperiorVisualization::setImage(cv::Mat loadedImage, float fov)
{
    imageRows = loadedImage.rows;
    imageColumns = loadedImage.cols;

    _cameraFOV = fov;
    _cameraDistance = (imageRows / 2.0f) / tan( (_cameraFOV/2.0f) * PI / 180.0f );

    _isImageLoaded = true;

    updateGL();
}

void GLWidget3DEngineSuperiorVisualization::setFOV(float fov)
{
     _cameraFOV = fov;

     updateGL();
}

void GLWidget3DEngineSuperiorVisualization::setConfiguration(Configuration * configuration)
{
    _configuration = configuration;
}

void GLWidget3DEngineSuperiorVisualization::reApplyConfiguration()
{
    // Draw all the entities added
    _entities->applyConfigurationSetColor(_configuration->entityWires());
    _entities->applyConfigurationSetColorsCPs(_configuration->positionCP(), _configuration->resizeCP(), _configuration->rotateCP(), _configuration->scaleCP());

    // Repaint
    repaint();
}

void GLWidget3DEngineSuperiorVisualization::setEntityTreeController(EntityTreeController * entities)
{
    _entities = entities;
}




