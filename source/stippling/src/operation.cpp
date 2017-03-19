#include "operation.h"

Operation::Operation(IDManager * idManager, const GLuint entityRenderShader,
                     const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color) :
    Entity3D(idManager)
{
    _type = OPERATION;

    _entityRenderShader = entityRenderShader;
    _controlPointsRenderShader = controlPointsRenderShader;
    _selectionShader = selectionShader;

    _color = color;

    int id;

    id = _idManager->getNewID();
    _position = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_TRANSLATION_X | CONSTRAINT_TRANSLATION_Y | CONSTRAINT_TRANSLATION_Z,
                             glm::vec3(0.0f,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _rotateY = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_ROTATION_Y,
                             glm::vec3(200.0f,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    add(&_position);
    add(&_rotateY);

    _parent = 0;

    _leftOperator = 0;
    _rightOperator = 0;

    id = _idManager->getNewID();
    _wireframe = GLEntity(id, _idManager->encodeID(id));
    id = _idManager->getNewID();
    _solid = GLEntity(id, _idManager->encodeID(id));

    setName("Operation " + QString::number(_solid.id()));
    _isOperation = true;
    _operation = BoolMesh::OP_UNION;

    recalculateGeometry();

    GLfloat * vertexColor = new GLfloat[lines_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<lines_NumberOfValues; i+=3)
    {
        vertexColor[i] = color.r;
        vertexColor[i+1] = color.g;
        vertexColor[i+2] = color.b;
    }

    const GLfloat * normalBufferData = 0;

    const GLfloat * uvBufferData = 0;

    _wireframe.initialize(lines, vertexColor, normalBufferData, uvBufferData,
                          lines_NumberOfValues, GL_LINES, 3, GL_DYNAMIC_DRAW,
                          entityRenderShader, selectionShader);

    deleteLines();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }

    vertexColor = new GLfloat[triangles_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<triangles_NumberOfValues; i+=3)
    {
        vertexColor[i] = color.r;
        vertexColor[i+1] = color.g;
        vertexColor[i+2] = color.b;
    }

    uvBufferData = 0;

    _solid.initialize(triangles, vertexColor, normals, uvBufferData,
                      triangles_NumberOfValues, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                      entityRenderShader, selectionShader);

    deleteTriangles();
    deleteNormals();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }
}

Operation::Operation(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
                     const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color) :
    Entity3D(idManager)
{
    _entityRenderShader = entityRenderShader;
    _controlPointsRenderShader = controlPointsRenderShader;
    _selectionShader = selectionShader;

    EntityType type = EntityType(node.firstChildElement("type").text().toInt());
    QString name = node.firstChildElement("name").text();
    bool visible = bool(node.firstChildElement("visible").text().toInt());
    bool isOperation = bool(node.firstChildElement("isOperation").text().toInt());
    unsigned int operation = node.firstChildElement("operation").text().toUInt();

    // Model matrix
    QDomNode xmlModel = node.firstChildElement("model");
    _model[0][0] = xmlModel.firstChildElement("elem0").text().toFloat();
    _model[0][1] = xmlModel.firstChildElement("elem1").text().toFloat();
    _model[0][2] = xmlModel.firstChildElement("elem2").text().toFloat();
    _model[0][3] = xmlModel.firstChildElement("elem3").text().toFloat();

    _model[1][0] = xmlModel.firstChildElement("elem4").text().toFloat();
    _model[1][1] = xmlModel.firstChildElement("elem5").text().toFloat();
    _model[1][2] = xmlModel.firstChildElement("elem6").text().toFloat();
    _model[1][3] = xmlModel.firstChildElement("elem7").text().toFloat();

    _model[2][0] = xmlModel.firstChildElement("elem8").text().toFloat();
    _model[2][1] = xmlModel.firstChildElement("elem9").text().toFloat();
    _model[2][2] = xmlModel.firstChildElement("elem10").text().toFloat();
    _model[2][3] = xmlModel.firstChildElement("elem11").text().toFloat();

    _model[3][0] = xmlModel.firstChildElement("elem12").text().toFloat();
    _model[3][1] = xmlModel.firstChildElement("elem13").text().toFloat();
    _model[3][2] = xmlModel.firstChildElement("elem14").text().toFloat();
    _model[3][3] = xmlModel.firstChildElement("elem15").text().toFloat();

    // Control points
    QDomNode xmlCPs = node.firstChildElement("controlPoints");

    QDomNode xmlPositionCP = xmlCPs.firstChildElement("controlPoint");
    QDomNode xmlRotateYCP = xmlPositionCP.nextSiblingElement("controlPoint");

    _type = type;
    _color = color;
    _name = name;
    _visible = visible;
    _isOperation = isOperation;
    _operation = operation;

    int id;
    id = _idManager->getNewID();
    _position = ControlPoint(id, idManager->encodeID(id), xmlPositionCP,
                             25, controlPointsRenderShader, selectionShader);

    id = _idManager->getNewID();
    _rotateY = ControlPoint(id, idManager->encodeID(id), xmlRotateYCP,
                            25, controlPointsRenderShader, selectionShader);

    add(&_position);
    add(&_rotateY);

    _parent = 0;

    _leftOperator = 0;
    _rightOperator = 0;

    id = _idManager->getNewID();
    _wireframe = GLEntity(id, _idManager->encodeID(id));
    id = _idManager->getNewID();
    _solid = GLEntity(id, _idManager->encodeID(id));

    recalculateGeometry();

    GLfloat * vertexColor = new GLfloat[lines_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<lines_NumberOfValues; i+=3)
    {
        vertexColor[i] = color.r;
        vertexColor[i+1] = color.g;
        vertexColor[i+2] = color.b;
    }

    const GLfloat * normalBufferData = 0;

    const GLfloat * uvBufferData = 0;

    _wireframe.initialize(lines, vertexColor, normalBufferData, uvBufferData,
                          lines_NumberOfValues, GL_LINES, 3, GL_DYNAMIC_DRAW,
                          entityRenderShader, selectionShader);

    deleteLines();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }

    vertexColor = new GLfloat[triangles_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<triangles_NumberOfValues; i+=3)
    {
        vertexColor[i] = color.r;
        vertexColor[i+1] = color.g;
        vertexColor[i+2] = color.b;
    }

    uvBufferData = 0;

    _solid.initialize(triangles, vertexColor, normals, uvBufferData,
                      triangles_NumberOfValues, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                      entityRenderShader, selectionShader);

    deleteTriangles();
    deleteNormals();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }
}

Operation::~Operation()
{

}

void Operation::recalculateGeometry()
{
    if(_leftOperator != 0 && _rightOperator != 0)
    {
        BoolMesh left;
        BoolMesh right;
        // Copied when assigned
        left = _leftOperator->mesh();
        right = _rightOperator->mesh();

        // A relative (to the left entity) vectorial space will be used so that the final
        // vertex positions of the calculated operation will not have the common transformations
        // applied (basically the left operand is taken as the new origin of coordinates and the
        // right operand's model will be the only transformation passed).
        glm::mat4 leftModel = glm::mat4(1.0f);
        glm::mat4 rightModel = glm::inverse(_leftOperator->model()) * _rightOperator->model();

        /*
        out << "Left model matrix:" << endl;
        Util::printMatrix(leftModel);
        out << "Right model matrix:" << endl;
        Util::printMatrix(rightModel);
        */

        // Apply the transformations
        left.multMatrix(leftModel);
        right.multMatrix(rightModel);

        // Calculate the resulting mesh
        _mesh = left.boolean(right, _operation);

        //_mesh.saveOFFMesh( QString("debug " + name() + ".off").toStdString() );

        unsigned int nVertices;
        float * vertices = _mesh.verticesCache(nVertices);
        unsigned int nFaces;
        std::vector<unsigned int> nVerticesPerFace;
        unsigned int ** faces = _mesh.facesCache(nFaces, nVerticesPerFace);

        /*
        out << endl;
        out << "==============================" << endl;
        out << "    Debugging CGAL mesh output" << endl;
        out << "nVertices = " << nVertices << endl;
        out << "vertices = " << endl << "{" << endl;
        for(int i=0; i<3*nVertices; i=i+3)
        {
            out << "( " << vertices[i] << ", " << vertices[i+1] << ", " << vertices[i+2] << " )";
            if(i<(3*nVertices)-3)
            {
                out << "," << endl;
            }
        }
        out << endl << "}" << endl;
        out << "nFaces = " << nFaces << endl;
        out << "nVerticesPerFace = {";
        for(int i=0; i<nFaces; ++i)
        {
            out << " " << nVerticesPerFace.at(i);
            if(i<nFaces-1)
            {
                out << ",";
            }
        }
        out << " }" << endl;
        out << "faces = " << endl << "{" << endl;
        for(int i=0; i<nFaces; ++i)
        {
            for(int j=0; j<nVerticesPerFace.at(i); ++j)
            {
                out << " " << faces[i][j];
                if(j<nVerticesPerFace.at(i)-1)
                {
                    out << ",";
                }
            }
            if(i<nFaces-1)
            {
                out << ";" << endl;
            }
        }
        out << ";" << endl << " }" << endl;
        out << "==============================" << endl;
        out << endl;
        */

        generateLines(nVertices, vertices, nFaces, &nVerticesPerFace, faces);
        generateTriangles(nVertices, vertices, nFaces, &nVerticesPerFace, faces);
        generateNormals(nVertices, vertices, nFaces, &nVerticesPerFace, faces);

        if(vertices != 0)
        {
            delete[] vertices;
            vertices = 0;
        }
        for(int f=0; f<int(nFaces); ++f)
        {
            if(faces[f] != 0)
            {
                delete[] faces[f];
                faces[f] = 0;
            }
        }
        if(faces != 0)
        {
            delete[] faces;
            faces = 0;
        }
    }
    else
    {
        lines_NumberOfValues = 0;
        triangles_NumberOfTriangles = 0;
    }
}

void Operation::updateGeometry()
{
    recalculateGeometry();

    int id = _wireframe.id();
    _wireframe = GLEntity(id, _idManager->encodeID(id));

    GLfloat * vertexColor = new GLfloat[lines_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<lines_NumberOfValues; i+=3)
    {
        vertexColor[i] = _color.r;
        vertexColor[i+1] = _color.g;
        vertexColor[i+2] = _color.b;
    }

    const GLfloat * normalBufferData = 0;

    const GLfloat * uvBufferData = 0;

    _wireframe.initialize(lines, vertexColor, normalBufferData, uvBufferData,
                          lines_NumberOfValues, GL_LINES, 3, GL_DYNAMIC_DRAW,
                          _entityRenderShader, _selectionShader);

    deleteLines();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }

    id = _solid.id();
    _solid = GLEntity(id, _idManager->encodeID(id));

    vertexColor = new GLfloat[triangles_NumberOfValues];
    // Setting the color of each vertex
    for(int i=0; i<triangles_NumberOfValues; i+=3)
    {
        vertexColor[i] = _color.r;
        vertexColor[i+1] = _color.g;
        vertexColor[i+2] = _color.b;
    }

    uvBufferData = 0;

    _solid.initialize(triangles, vertexColor, normals, uvBufferData,
                      triangles_NumberOfValues, GL_TRIANGLES, 3, GL_DYNAMIC_DRAW,
                      _entityRenderShader, _selectionShader);

    deleteTriangles();
    deleteNormals();
    if(vertexColor != 0)
    {
        delete[] vertexColor;
        vertexColor = 0;
    }

    if(_parent != 0 && _parent->isOperation())
    {
        _parent->updateGeometry();
    }
}

void Operation::interactSpecifically(ControlPoint * cp)
{

}

Entity3D * Operation::leftOperator() const
{
    return _leftOperator;
}

Entity3D * Operation::rightOperator() const
{
    return _rightOperator;
}

ControlPoint * Operation::getControlPointRotateY()
{
    return &_rotateY;
}

void Operation::setLeftOperator(Entity3D * leftOperator)
{
    if(_leftOperator != 0)
    {
        _leftOperator->_parent = 0;
    }

    _leftOperator = leftOperator;

    if(_leftOperator != 0)
    {
        _leftOperator->_parent = this;
    }

    updateGeometry();
}

void Operation::setRightOperator(Entity3D * rightOperator)
{
    if(_rightOperator != 0)
    {
        _rightOperator->_parent = 0;
    }

    _rightOperator = rightOperator;

    if(_rightOperator != 0)
    {
        _rightOperator->_parent = this;
    }

    updateGeometry();
}

