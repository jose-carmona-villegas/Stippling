/**
 * @file glentity.h
 * @brief GLEntity header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef GLENTITY_H
#define GLENTITY_H

#include "GL/glew.h"
#include <QtOpenGL/QGLWidget>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>


#include <QVector3D>
#include <QTextStream>
#include "enums.h"
#include "util.h"

/**
 * @brief GLEntity class.
 * Represents a renderable openGL object.
 */
class GLEntity
{
private:
    QString checkGLError();
    void printGLError();

    void delete_vertexMPBuffer();
    void delete_normalMPBuffer();
    void delete_colorMPBuffer();

    GLuint _vertexArrayID;

    GLuint _vertexBuffer;       // Layout 0
    GLuint _normalBuffer;       // Layout 1
    GLuint _colorBuffer;        // Layout 2
    GLuint _uvBuffer;           // Layout 3

    int _bufferSize;
    int _elementType;
    GLuint _glType;
    int _valuesPerVertex;

    GLuint _renderShader;
    GLuint _selectionShader;

    int _id; /**< Selection ID number. */
    glm::vec3 _selectionColor; /**< Selection color (associated to the id number). */

    glm::mat4 _model; /**< Model matrix (contains information about position, rotation and scale). Uses float as value type. */

    bool initialized;

    // Buffer copies on MP to be used with the FFP
    GLfloat * _vertexMPBuffer;
    GLfloat * _normalMPBuffer;
    GLfloat * _colorMPBuffer;

public:
    /**
     * @brief Default constructor.
     * Initializes the model matrix and sets the id to -1.
     * To be usable, the object requires to be initialized.
     * @see GLEntity#initialize()
     */
    GLEntity();

    /**
     * @brief Constructor.
     * Initializes the model matrix and sets the id to the given value.
     * To be usable, the object requires to be initialized.
     * @see GLEntity#initialize()
     * @param id The id value desired for this entity.
     * @param selectionColor The normalized RGB color associated to the id.
     */
    GLEntity(int id, glm::vec3 selectionColor);

    /**
     * @brief Destructor.
     */
    ~GLEntity();

    /**
     * @brief Gets the id of the entity.
     * @return The id (integer value, greater than zero) that identifies the entity.
     */
    int id() const;

    /**
     * @brief Gets the selection color of the identity.
     * @return Normalized RGB color used when painting in selection mode.
     */
    glm::vec3 selectionColor() const;

    /**
     * @brief Returns the model matrix.
     * Right after initialization, it will be the identity (position (0,0,0), no rotations and no scaling).
     * @see GLEntity#translate()
     * @see GLEntity#rotate()
     * @see GLEntity#scale()
     */
    glm::mat4 model() const;

    /**
     * @brief Moves the object (using the amounts specified). It accumulates with former transformations.
     * @param offset Amounts of pixels (in x,y,z coords.) that will be added to the object's current position.
     */
    void translate(const glm::vec3 offset);
    /**
     * @brief Rotates the object (using both the angle and axis specified). It accumulates with former transformations.
     * @param angle Angle (in degrees) to be rotated.
     * @param axis Three dimensional vector that specifies the rotation axis.
     */
    void rotate(const GLfloat angle, const glm::vec3 axis);
    /**
     * @brief Scales the object (using the amounts specified). It accumulates with former transformations.
     * @param scale Three dimensional vector that specifies the scale factors on (x,y,z).
     */
    void scale(const glm::vec3 scale);

    /**
     * @brief Resets the model matrix to an identity matrix (position on (0,0,0), no rotations nor scaling).
     */
    void resetModel();

    /**
     * @brief Initializes the object using the data provided.
     * @param vertexBufferData Each vertex has to provide 3 GLfloat values that represent its cartesian coordinates.
     * @param colorBufferData Each vertex has to provide 3 GLfloat values that represent its RGB color code.
     * @param normalBufferData Each vertex has to provide 3 GLfloat values that represent its normal.
     * @param uvBufferData Each vertex has to provide 2 GLfloat values that represent its UV coordintes (for textured entities).
     * @param bufferSize Number of GLfloat values present in the vertexBufferData array.
     * @param glType Type of primitive to be rendered. The accepted values are:
                    GL_POINTS,
                    GL_LINE_STRIP,
                    GL_LINE_LOOP,
                    GL_LINES,
                    GL_LINE_STRIP_ADJACENCY,
                    GL_LINES_ADJACENCY,
                    GL_TRIANGLE_STRIP,
                    GL_TRIANGLE_FAN,
                    GL_TRIANGLES,
                    GL_TRIANGLE_STRIP_ADJACENCY,
                    GL_TRIANGLES_ADJACENCY,
                    GL_PATCHES.
     * @param pointsPerElement Number of values required to define a single primitive (among the ones specified in glType)
     * @param usage Specifies the expected usage pattern of the data store. The symbolic constant must be
                    GL_STREAM_DRAW,
                    GL_STREAM_READ,
                    GL_STREAM_COPY,
                    GL_STATIC_DRAW,
                    GL_STATIC_READ,
                    GL_STATIC_COPY,
                    GL_DYNAMIC_DRAW,
                    GL_DYNAMIC_READ,
                    GL_DYNAMIC_COPY.
     * @param renderShader
     * @param selectionShader
     * @see OpenGL#glDrawArrays()
     * @see OpenGL#glBufferData()
     */
    void initialize(const GLfloat vertexBufferData[], const GLfloat colorBufferData[],
                    const GLfloat normalBufferData[], const GLfloat uvBufferData[],
                    const int bufferSize, GLuint glType, int pointsPerElement, GLenum usage,
                    GLuint renderShader, GLuint selectionShader);

    /**
     * @brief Renders the object.
     * @pre The method initialize() must have been called beforehand.
     * @param selectionMode
     * @param view
     * @param projection
     * @param extModel
     * @param texture Texture identifier as generated by OpenGL (0 means no texture), @see glGenTextures().
     * @param xLeft
     * @param xRight
     * @param yBottom
     * @param yTop
     * @param zNear
     * @param zFar
     * @param drawStippledLines
     * @see GLEntity#initialize()
     */
    void paint(SelectionMode selectionMode, const glm::mat4 * view, const glm::mat4 * projection,
               const glm::mat4 * extModel, GLuint texture, GLfloat xLeft, GLfloat xRight,
               GLfloat yBottom, GLfloat yTop, GLfloat zNear, GLfloat zFar,
               bool drawStippledLines = false, bool illuminated = false) const;


    void setVertexData(const int offset, const int bufferSize, const GLfloat * vertexBufferData);

    void setColorData(const int offset, const int bufferSize, const GLfloat * colorBufferData);

    void setColor(glm::vec3 color);

    void setUVData(const int offset, const int bufferSize, const GLfloat * uvBufferData);
};

#endif // GLENTITY_H
