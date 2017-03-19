#include "cylinder.h"

Cylinder::Cylinder(IDManager * idManager, const float size, const GLuint entityRenderShader,
             const GLuint controlPointsRenderShader, const GLuint selectionShader, glm::vec3 color) :
    Entity3D(idManager)
{
    _type = CYLINDER;

    _color = color;

    float radius = size / 2.0f;

    int id;

    id = _idManager->getNewID();
    _position = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_TRANSLATION_X | CONSTRAINT_TRANSLATION_Y | CONSTRAINT_TRANSLATION_Z,
                             glm::vec3(0.0f,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    id = _idManager->getNewID();
    _resizeRadiusBottom = ControlPoint(id, _idManager->encodeID(id),
                                 CONSTRAINT_RESIZE_X,
                                 glm::vec3(-radius,0.0f,0.0f),
                                 25,
                                 controlPointsRenderShader,
                                 selectionShader);

    id = _idManager->getNewID();
    _resizeRadiusTop = ControlPoint(id, _idManager->encodeID(id),
                                 CONSTRAINT_RESIZE_X,
                                 glm::vec3(-radius,size,0.0f),
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
    _rotateY = ControlPoint(id, _idManager->encodeID(id),
                             CONSTRAINT_ROTATION_Y,
                             glm::vec3(size,0.0f,0.0f),
                             25,
                             controlPointsRenderShader,
                             selectionShader);

    add(&_position);
    add(&_resizeRadiusBottom);
    add(&_resizeRadiusTop);
    add(&_resizeHeight);
    add(&_rotateY);

    _parent = 0;

    id = _idManager->getNewID();
    _wireframe = GLEntity(id, _idManager->encodeID(id));
    id = _idManager->getNewID();
    _solid = GLEntity(id, _idManager->encodeID(id));

    setName("Cylinder " + QString::number(_solid.id()));
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

Cylinder::Cylinder(IDManager * idManager, QDomNode node, const GLuint entityRenderShader,
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
    QDomNode xmlResizeRadiusBottomCP = xmlRotateYCP.nextSiblingElement("controlPoint");
    QDomNode xmlResizeRadiusTopCP = xmlResizeRadiusBottomCP.nextSiblingElement("controlPoint");
    QDomNode xmlResizeHeightCP = xmlResizeRadiusTopCP.nextSiblingElement("controlPoint");

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
    _resizeRadiusBottom = ControlPoint(id, idManager->encodeID(id), xmlResizeRadiusBottomCP,
                            25, controlPointsRenderShader, selectionShader);

    id = _idManager->getNewID();
    _resizeRadiusTop = ControlPoint(id, idManager->encodeID(id), xmlResizeRadiusTopCP,
                            25, controlPointsRenderShader, selectionShader);

    id = _idManager->getNewID();
    _resizeHeight = ControlPoint(id, idManager->encodeID(id), xmlResizeHeightCP,
                            25, controlPointsRenderShader, selectionShader);

    add(&_position);
    add(&_resizeRadiusBottom);
    add(&_resizeRadiusTop);
    add(&_resizeHeight);
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

Cylinder::~Cylinder()
{

}

ControlPoint * Cylinder::getControlPointRotateY()
{
    return &_rotateY;
}

ControlPoint * Cylinder::getControlPointResizeRadiusBottom()
{
    return &_resizeRadiusBottom;
}

ControlPoint * Cylinder::getControlPointResizeRadiusTop()
{
    return &_resizeRadiusTop;
}

ControlPoint * Cylinder::getControlPointResizeHeight()
{
    return &_resizeHeight;
}


void Cylinder::recalculateGeometry()
{
    float radiusBottom = _resizeRadiusBottom.localPos().x - _position.localPos().x;
    float radiusTop = _resizeRadiusTop.localPos().x - _position.localPos().x;
    float height = _resizeHeight.localPos().y - _position.localPos().y;

    /*
    out << "=============  Recalculating Cylinder's geometry..." << endl;
    out << "Position: " << endl;
    Util::printVector(_position.localPos());
    out << "RadiusBottom: " << endl;
    Util::printVector(_resizeRadiusBottom.localPos());
    out << "RadiusTop: " << endl;
    Util::printVector(_resizeRadiusTop.localPos());
    out << "Height: " << endl;
    Util::printVector(_resizeHeight.localPos());

    out << "bottom radius size: " << radiusBottom << endl;
    out << "top radius size: " << radiusTop << endl;
    out << "height size: " << height << endl;
    */

    QVector<glm::vec3> baseBottom = QVector<glm::vec3>();
    QVector<glm::vec3> baseTop = QVector<glm::vec3>();

    // Bottom base calculation
    //out << "Bottom base formed by: " << endl;
    for(int i=0; i<_numberOfSidesPerCircle; ++i)
    {
        glm::vec3 circumferencePoint = _position.localPos() +
                glm::vec3(radiusBottom * cos((2*PI / _numberOfSidesPerCircle) * i),
                          0.0f,
                          radiusBottom * sin((2*PI / _numberOfSidesPerCircle) * i));
        baseBottom.append(circumferencePoint);
        //Util::printVector(circumferencePoint);
    }

    // Top base calcualtion
    //out << "Top base formed by: " << endl;
    for(int i=0; i<_numberOfSidesPerCircle; ++i)
    {
        glm::vec3 circumferencePoint = _position.localPos() +
                glm::vec3(radiusTop * cos((2*PI / _numberOfSidesPerCircle) * i),
                          height,
                          radiusTop * sin((2*PI / _numberOfSidesPerCircle) * i));
        baseTop.append(circumferencePoint);
        //Util::printVector(circumferencePoint);
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

    //out << "vertex #" << vertexIndex << " = " << endl;
    //Util::printVector(_position.localPos());
    //++vertexIndex;
    surface.addVertex(_position.localPos());

    //out << "vertex #" << vertexIndex << " = " << endl;
    //Util::printVector(_resizeHeight.localPos());
    //++vertexIndex;
    surface.addVertex(_resizeHeight.localPos());

    QVector<unsigned int> tempFaces;

    // Adding the bottom face to the surface
    for(int i=0; i<baseBottom.size(); ++i)
    {
       tempFaces.clear();

       tempFaces.push_back(baseBottom.size() + baseTop.size());
       tempFaces.push_back(Util::mod(-i,baseBottom.size()));
       tempFaces.push_back(Util::mod(-1-i,baseBottom.size()));

       surface.addFace(tempFaces);
    }

    // Adding the top face to the surface
    for(int i=0; i<baseBottom.size(); ++i)
    {
       tempFaces.clear();

       tempFaces.push_back(baseBottom.size() + baseTop.size() + 1);
       tempFaces.push_back(baseBottom.size() + Util::mod(i,baseTop.size()));
       tempFaces.push_back(baseBottom.size() + Util::mod(1+i,baseTop.size()));

       surface.addFace(tempFaces);
    }

    // Adding the lateral faces to the surface
    for(int i=0; i<baseBottom.size(); ++i)
    {
        tempFaces.clear();
        tempFaces.push_back(i);
        tempFaces.push_back(Util::mod(i+1,baseBottom.size()));
        tempFaces.push_back(baseBottom.size() + Util::mod(i+1,baseBottom.size()));
        surface.addFace(tempFaces);

        tempFaces.clear();
        tempFaces.push_back(i);
        tempFaces.push_back(baseBottom.size() + Util::mod(i+1,baseBottom.size()));
        tempFaces.push_back(baseBottom.size() + i);
        surface.addFace(tempFaces);
    }

    //out << "Vertices and faces added to the surface..." << endl;
    // create polyhedron from surface
    _mesh.clearCache();
    //out << "Cleared mesh cache..." << endl;
    _mesh.addSurface(surface);
    //out << "Surface added to the mesh..." << endl;
    _mesh.flushCache();
    //out << "Flushed mesch cache..." << endl;

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

void Cylinder::interactSpecifically(ControlPoint * cp)
{
    if(cp == &_resizeHeight)
    {
        glm::vec3 temp = _resizeRadiusTop.localPos();
        temp.y = _resizeHeight.localPos().y;

        _resizeRadiusTop.setLocalPos(temp);
    }
}
