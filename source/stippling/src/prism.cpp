/**
 * @file prism.cpp
 * @brief Prism class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 11/November/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "prism.h"

Prism::Prism(IDManager * idManager, const float size, const GLuint entityRenderShader,
             const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color) :
    Entity3D(idManager)
{   
    _type = PRISM;

    _color = color;
    float halfSize = size/2.0f;

    int id;

    id = _idManager->getNewID();
    _position = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_TRANSLATION_X | CONSTRAINT_TRANSLATION_Y | CONSTRAINT_TRANSLATION_Z,
                             glm::vec3(0.0f,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _resizeHeight = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_RESIZE_Y,
                             glm::vec3(0.0f,size,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _resizeX = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_RESIZE_X | CONSTRAINT_RESIZE_Z,
                             glm::vec3(-size,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _resizeZ = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_RESIZE_X | CONSTRAINT_RESIZE_Z,
                             glm::vec3(0.0f,0.0f,-size),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _rotateY = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_ROTATION_Y,
                             glm::vec3(halfSize,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    add(&_position);
    add(&_resizeHeight);
    add(&_resizeX);
    add(&_resizeZ);
    add(&_rotateY);

    _parent = 0;

    id = _idManager->getNewID();
    _wireframe = GLEntity(id, _idManager->encodeID(id));
    id = _idManager->getNewID();
    _solid = GLEntity(id, _idManager->encodeID(id));

    setName("Prism " + QString::number(_solid.id()));
    _isOperation = false;

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

Prism::Prism(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
      const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color) :
    Entity3D(idManager)
{
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
    QDomNode xmlResizeXCP = xmlRotateYCP.nextSiblingElement("controlPoint");
    QDomNode xmlResizeZCP = xmlResizeXCP.nextSiblingElement("controlPoint");
    QDomNode xmlResizeHeightCP = xmlResizeZCP.nextSiblingElement("controlPoint");

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

    id = _idManager->getNewID();
    _resizeX = ControlPoint(id, idManager->encodeID(id), xmlResizeXCP,
                            25, controlPointsRenderShader, selectionShader);

    id = _idManager->getNewID();
    _resizeZ = ControlPoint(id, idManager->encodeID(id), xmlResizeZCP,
                            25, controlPointsRenderShader, selectionShader);

    id = _idManager->getNewID();
    _resizeHeight = ControlPoint(id, idManager->encodeID(id), xmlResizeHeightCP,
                            25, controlPointsRenderShader, selectionShader);

    add(&_position);
    add(&_resizeHeight);
    add(&_resizeX);
    add(&_resizeZ);
    add(&_rotateY);

    _parent = 0;

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

Prism::~Prism()
{

}

ControlPoint * Prism::getControlPointRotateY()
{
    return &_rotateY;
}

ControlPoint * Prism::getControlPointResizeX()
{
    return &_resizeX;
}

ControlPoint * Prism::getControlPointResizeHeight()
{
    return &_resizeHeight;
}

ControlPoint * Prism::getControlPointResizeZ()
{
    return &_resizeZ;
}

void Prism::recalculateGeometry()
{
    QVector<glm::vec3> baseBottom = QVector<glm::vec3>();
    QVector<glm::vec3> baseTop = QVector<glm::vec3>();

    // Bottom base calculation
    glm::vec3 corner;
    corner = _position.localPos();
    baseBottom.append(corner);
    corner = _position.localPos() + _resizeZ.localPos();
    baseBottom.append(corner);
    corner = _position.localPos() + _resizeX.localPos() + _resizeZ.localPos();
    baseBottom.append(corner);
    corner = _position.localPos() + _resizeX.localPos();
    baseBottom.append(corner);

    // Top base calcualtion
    foreach(glm::vec3 cornerBottom, baseBottom)
    {
        glm::vec3 cornerTop = cornerBottom + _resizeHeight.localPos();
        baseTop.append(cornerTop);
    }

    Surface surface;

    // Adding the vertices to the surface
    //int vertexIndex = 0;
    //out << "Prism's vertices:" << endl;
    foreach(glm::vec3 cornerBottom, baseBottom)
    {
        //out << "vertex #" << vertexIndex << " = " << endl;
        //Util::printVector(cornerBottom);
        //++vertexIndex;
        surface.addVertex(cornerBottom);
    }
    foreach(glm::vec3 cornerTop, baseTop)
    {
        //out << "vertex #" << vertexIndex << " = " << endl;
        //Util::printVector(cornerTop);
        //++vertexIndex;
        surface.addVertex(cornerTop);
    }

    QVector<unsigned int> tempFaces;

    // Adding the bottom face to the surface
    for(int i=0; i<baseBottom.size()-2; ++i)
    {
       tempFaces.clear();

       tempFaces.push_back(0);
       tempFaces.push_back(Util::mod(-1-i,baseBottom.size()));
       tempFaces.push_back(Util::mod(-2-i,baseBottom.size()));

       surface.addFace(tempFaces);
    }

    // Adding the top face to the surface
    for(int i=0; i<baseTop.size()-2; ++i)
    {
       tempFaces.clear();

       tempFaces.push_back(baseBottom.size());
       tempFaces.push_back(baseBottom.size() + Util::mod(1+i,baseTop.size()));
       tempFaces.push_back(baseBottom.size() + Util::mod(2+i,baseTop.size()));

       surface.addFace(tempFaces);
    }

    // Adding the lateral faces to the surface
    for(int i=0; i<baseBottom.size(); ++i)
    {
        tempFaces.clear();

        tempFaces.push_back(i);
        tempFaces.push_back(Util::mod(i+1,baseBottom.size()));
        tempFaces.push_back(baseBottom.size() + Util::mod(i+1,baseTop.size()));

        surface.addFace(tempFaces);

        tempFaces.clear();

        tempFaces.push_back(i);
        tempFaces.push_back(baseBottom.size() + Util::mod(i+1,baseTop.size()));
        tempFaces.push_back(baseBottom.size() + i);

        surface.addFace(tempFaces);
    }

    // create polyhedron from surface
    _mesh.clearCache();
    _mesh.addSurface(surface);
    _mesh.flushCache();

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

void Prism::interactSpecifically(ControlPoint * cp)
{
    if(cp == &_resizeX)
    {
        if(_resizeZ.localPos().x != 0.0f)
        {
            float maxZ = _resizeX.localPos().x * (_resizeZ.localPos().z / _resizeZ.localPos().x);
            if( (_resizeX.localPos().z < maxZ) )
            {
                glm::vec3 temp = _resizeX.localPos();
                temp.z = maxZ + 1.0f;
                _resizeX.setLocalPos(temp);
            }
        }
    }
    else if(cp == &_resizeZ)
    {
        if(_resizeX.localPos().x != 0.0f)
        {
            float minZ = _resizeZ.localPos().x * (_resizeX.localPos().z / _resizeX.localPos().x);
            if(_resizeZ.localPos().z > minZ )
            {
                glm::vec3 temp = _resizeZ.localPos();
                temp.z = minZ - 1.0f;
                _resizeZ.setLocalPos(temp);
            }
        }
    }
}
