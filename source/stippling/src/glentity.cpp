/**
 * @file glentity.cpp
 * @brief GLEntity source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "glentity.h"

QString GLEntity::checkGLError()
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

void GLEntity::printGLError()
{
    out << endl;
    out << "################ GL STATUS = " << checkGLError()  << " ################" << endl;
    out << endl;
}

void GLEntity::delete_vertexMPBuffer()
{
    if(_vertexMPBuffer != 0)
    {
        delete[] _vertexMPBuffer;
        _vertexMPBuffer = 0;
    }
}

void GLEntity::delete_normalMPBuffer()
{
    if(_normalMPBuffer != 0)
    {
        delete[] _normalMPBuffer;
        _normalMPBuffer = 0;
    }
}

void GLEntity::delete_colorMPBuffer()
{
    if(_colorMPBuffer != 0)
    {
        delete[] _colorMPBuffer;
        _colorMPBuffer = 0;
    }
}

GLEntity::GLEntity()
{
    _id = -1;
    _model = glm::mat4(1.0f);
    _bufferSize = 0;

    initialized = false;
    _vertexArrayID = 0;

    _vertexMPBuffer = 0;
    _normalMPBuffer = 0;
    _colorMPBuffer = 0;
}

GLEntity::GLEntity(int id, glm::vec3 selectionColor)
{
    _id = id;
    _selectionColor = glm::vec3(selectionColor);
    _model = glm::mat4(1.0f);
    _bufferSize = 0;

    initialized = false;
    _vertexArrayID = 0;

    _vertexMPBuffer = 0;
    _normalMPBuffer = 0;
    _colorMPBuffer = 0;
}

GLEntity::~GLEntity()
{
    /*
    if(initialized)
    {
        bool isVAO = (glIsVertexArray(_vertexArrayID) == GL_TRUE);
        if(isVAO)
        {
            glDeleteVertexArrays(1, &_vertexArrayID);
            _vertexArrayID = 0;
        }
    }
    */

    /*
    delete_vertexMPBuffer();
    delete_normalMPBuffer();
    delete_colorMPBuffer();
    */
}

int GLEntity::id() const
{
    return _id;
}

glm::vec3 GLEntity::selectionColor() const
{
    return _selectionColor;
}

glm::mat4 GLEntity::model() const
{
    return _model;
}

void GLEntity::translate(const glm::vec3 offset)
{
    _model = glm::translate(_model, offset);
}

void GLEntity::rotate(const GLfloat angle, const glm::vec3 axis)
{
    _model = glm::rotate(_model, angle, axis);
}

void GLEntity::scale(const glm::vec3 scale)
{
    _model = glm::scale(_model, scale);
}

void GLEntity::resetModel()
{
    _model = glm::mat4(1.0f);
}

void GLEntity::initialize(const GLfloat vertexBufferData[], const GLfloat colorBufferData[],
                          const GLfloat normalBufferData[], const GLfloat uvBufferData[],
                          const int bufferSize, GLuint glType, int valuesPerVertex, GLenum usage,
                          GLuint renderShader, GLuint selectionShader)
{
    resetModel();

    _bufferSize = bufferSize;
    _glType = glType;
    _valuesPerVertex = valuesPerVertex;

    glGenVertexArrays(1, &_vertexArrayID);
    glBindVertexArray(_vertexArrayID);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_bufferSize, vertexBufferData, usage);

    glGenBuffers(1, &_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_bufferSize, normalBufferData, usage);

    glGenBuffers(1, &_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_bufferSize, colorBufferData, usage);

    glGenBuffers(1, &_uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_bufferSize, uvBufferData, usage);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(
            0,                      // attribute. Match the layout in the shader.
            _valuesPerVertex,       // size
            GL_FLOAT,               // type
            GL_FALSE,               // normalized?
            0,                      // stride
            (void*)0                // array buffer offset
    );

    // 2nd attribute buffer : normal
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
    glVertexAttribPointer(
            1,                      // attribute. Match the layout in the shader.
            3,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // normalized?
            0,                      // stride
            (void*)0                // array buffer offset
    );

    // 3rd attribute buffer : color
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glVertexAttribPointer(
            2,                      // attribute. Match the layout in the shader.
            3,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // normalized?
            0,                      // stride
            (void*)0                // array buffer offset
    );

    // 4th attribute buffer : uv coordinates
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
    glVertexAttribPointer(
            3,                      // attribute. Match the layout in the shader.
            2,                      // size
            GL_FLOAT,               // type
            GL_FALSE,               // normalized?
            0,                      // stride
            (void*)0                // array buffer offset
    );

    _renderShader = renderShader;
    _selectionShader = selectionShader;


    delete_vertexMPBuffer();
    if(vertexBufferData != 0)
    {
        _vertexMPBuffer =  new GLfloat[_bufferSize];
        for(int i=0; i<_bufferSize; ++i)
        {
            _vertexMPBuffer[i] = vertexBufferData[i];
        }
    }


    delete_normalMPBuffer();
    if(normalBufferData != 0)
    {
        _normalMPBuffer =  new GLfloat[_bufferSize];
        for(int i=0; i<_bufferSize; ++i)
        {
            _normalMPBuffer[i] = normalBufferData[i];
        }
    }


    delete_colorMPBuffer();
    if(colorBufferData != 0)
    {
        _colorMPBuffer =  new GLfloat[_bufferSize];
        for(int i=0; i<_bufferSize; ++i)
        {
            _colorMPBuffer[i] = colorBufferData[i];
        }
    }

    initialized = true;
}

void GLEntity::paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
                     const glm::mat4 * extModel, GLuint texture, GLfloat xLeft, GLfloat xRight,
                     GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar,
                     bool drawStippledLines, bool illuminated) const
{
    glm::mat4 identity = glm::mat4(1.0f);
    if(extModel == 0)
    {
        extModel = &identity;
    }

    if(!illuminated)
    {
        // Draw normally, no illumination

        // ModelViewProjection : multiplication of our 3 matrices; _mvp = ( projection * ( view * ( (extModel * ( model ) ) ) )
        glm::mat4 mvp = (*projection) * (*view) * (*extModel) * model();

        GLuint usingShader;
        if(selectionMode == ON)
        {
            // Use the selection shader
            usingShader = _selectionShader;
        }
        else
        {
            // Use the render shader
            usingShader = _renderShader;
        }

        // Get the handles for the uniforms
        GLuint mvpHandler = glGetUniformLocation(usingShader, "mvp");
        GLuint pickingColorHandler  = glGetUniformLocation(usingShader, "pickingColor");
        GLuint textureSamplerHandler  = glGetUniformLocation(usingShader, "textureSampler");
        GLuint xLeftID = glGetUniformLocation(usingShader, "xLeft");
        GLuint xRightID = glGetUniformLocation(usingShader, "xRight");
        GLuint yBottomID = glGetUniformLocation(usingShader, "yBottom");
        GLuint yTopID = glGetUniformLocation(usingShader, "yTop");
        GLuint zNearID = glGetUniformLocation(usingShader, "zNear");
        GLuint zFarID = glGetUniformLocation(usingShader, "zFar");

        // Use our shader
        glUseProgram(usingShader);

        // Load the uniforms data on the program's uniforms
        glUniformMatrix4fv(mvpHandler, 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(pickingColorHandler, selectionColor().r, selectionColor().g, selectionColor().b);
        if(texture >= 0)
        {
            // Bind the texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Set the "textureSampler" sampler to user Texture Unit 0
            glUniform1i(textureSamplerHandler, 0);
        }
        glUniform1f(xLeftID, xLeft);
        glUniform1f(xRightID, xRight);
        glUniform1f(yBottomID, yBottom);
        glUniform1f(yTopID, yTop);
        glUniform1f(zNearID, zNear);
        glUniform1f(zFarID, zFar);

        int previousWidth;
        glGetIntegerv(GL_LINE_WIDTH, &previousWidth);
        if(_glType == GL_LINES)
        {
            glPushAttrib(GL_ENABLE_BIT);
            glLineWidth(10.0f);
            if(drawStippledLines)
            {
                glLineStipple(1, 0xF00F);
                glEnable(GL_LINE_STIPPLE);
            }
        }

        // Draw using the VAO (Vertex Array Object) data
        glBindVertexArray(_vertexArrayID);
        glDrawArrays(_glType, 0, (int)(_bufferSize/_valuesPerVertex));

        if(_glType == GL_LINES)
        {
            glLineWidth(previousWidth);
            glPopAttrib();
        }
    }
    else
    {
        glUseProgram(0);

        glm::mat4 mv = (*view) * (*extModel) * model();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((const float*)glm::value_ptr(*projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((const float*)glm::value_ptr(mv));

        glShadeModel(GL_SMOOTH);

        /*
        out << "About to paint the following vertex buffer:" << endl;
        for(int i=0; i<(int)(_bufferSize); i+=_valuesPerVertex)
        {
            out << "( " << _vertexMPBuffer[i] << " , " << _vertexMPBuffer[i+1] << " , " << _vertexMPBuffer[i+2] << " )" << endl;
        }
        */

        glBegin(_glType);

        for(int i=0; i<(int)(_bufferSize); i+=_valuesPerVertex)
        {
            glColor3f(_colorMPBuffer[i], _colorMPBuffer[i+1], _colorMPBuffer[i+2]);
            if(_glType != GL_LINES)
            {
                glNormal3f(_normalMPBuffer[i], _normalMPBuffer[i+1], _normalMPBuffer[i+2]);
            }
            glVertex3f(_vertexMPBuffer[i], _vertexMPBuffer[i+1], _vertexMPBuffer[i+2]);
        }

        glEnd();

        /*
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

        //glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, _vertexMPBuffer);

        //glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, _normalMPBuffer);

        //glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, _colorMPBuffer);

        //glBindVertexArray(_vertexArrayID);
        glDrawArrays(_glType, 0, (int)(_bufferSize/_valuesPerVertex));

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopClientAttrib();
        */
    }


}

void GLEntity::setVertexData(const int offset, const int bufferSize, const GLfloat * vertexBufferData)
{
    if(initialized)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER,  offset,  sizeof(GLfloat)*bufferSize,  vertexBufferData);

        for(int i=offset; i<bufferSize; ++i)
        {
            _vertexMPBuffer[i] = vertexBufferData[i];
        }
    }
}

void GLEntity::setColorData(const int offset, const int bufferSize, const GLfloat * colorBufferData)
{
    if(initialized)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferSubData(GL_ARRAY_BUFFER,  offset,  sizeof(GLfloat)*bufferSize,  colorBufferData);

        for(int i=offset; i<bufferSize; ++i)
        {
            _colorMPBuffer[i] = colorBufferData[i];
        }
    }
}

void GLEntity::setUVData(const int offset, const int bufferSize, const GLfloat * uvBufferData)
{
    if(initialized)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
        glBufferSubData(GL_ARRAY_BUFFER,  offset,  sizeof(GLfloat)*bufferSize,  uvBufferData);
    }
}

void GLEntity::setColor(glm::vec3 color)
{
    if(initialized)
    {
        GLfloat * vertexColor = new GLfloat[_bufferSize];

        int index;
        for(index=0; index<_bufferSize; index+=3)
        {
            vertexColor[index] = color.r;
            vertexColor[index+1] = color.g;
            vertexColor[index+2] = color.b;
        }

        setColorData(0, _bufferSize, vertexColor);

        if(vertexColor != 0)
        {
            delete[] vertexColor;
            vertexColor = 0;
        }
    }
}

