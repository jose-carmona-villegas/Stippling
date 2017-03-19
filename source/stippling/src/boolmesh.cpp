#include "boolmesh.h"

// -------------------------------------------------------------------------
BoolMesh::BoolMesh () 
{
   this->_cache.clear();
   return;
}


// -------------------------------------------------------------------------
BoolMesh::BoolMesh (BoolMesh &org) 
{
   this->_cache.clear();
   this->CGALpoly = Nef_polyhedron(org.CGALpoly);
   this->updateCache();
   return;
}


// -------------------------------------------------------------------------
BoolMesh::~BoolMesh () 
{
   return;
}


// -------------------------------------------------------------------------
int BoolMesh::loadOFFMesh (const char *filename) 
{
   std::string fname(filename);
   return this->loadOFFMesh(fname);
}


// -------------------------------------------------------------------------
int BoolMesh::loadOFFMesh (std::string filename) 
{
   std::ifstream offInputFile;

   offInputFile.open(filename.c_str()); // i.e. exported with blender
   if (!offInputFile.is_open())
      return this->ERROR_READ;
   offInputFile >> this->_cache;
   offInputFile.close();

   Nef_polyhedron N(this->_cache);
   this->CGALpoly = N;

   return this->OK; 
}


// -------------------------------------------------------------------------
int BoolMesh::saveOFFMesh (const char *filename) 
{
   std::string fname(filename);
   return this->saveOFFMesh(fname);
}


// -------------------------------------------------------------------------
int BoolMesh::saveOFFMesh (std::string filename) 
{
   std::ofstream offOutputFile;

   offOutputFile.open (filename.c_str()); // 3d file that can be viewed by geomview (meshlab doesn't load it properly)
   if (!offOutputFile.is_open())
      return this->ERROR_WRITE;
   offOutputFile << this->_cache;
   offOutputFile.close();

   return this->OK; 
}


// -------------------------------------------------------------------------
BoolMesh BoolMesh::boolean(BoolMesh model, unsigned int operation)
{
   BoolMesh NewMesh;

   if (operation == this->OP_DIFFERENCE)
      NewMesh.CGALpoly = this->CGALpoly.difference(model.CGALpoly);
   else if (operation == this->OP_INTERSECTION)
      NewMesh.CGALpoly = this->CGALpoly.intersection(model.CGALpoly);
   else if (operation == this->OP_UNION)
      NewMesh.CGALpoly = this->CGALpoly.join(model.CGALpoly);
   else
      std::cerr << "Warning: boolean operation [" << operation << "] unknown."
		<< " Result is set to EMPTY.\n";

   // convert polyhedron for the result of the boolean operation
   NewMesh.updateCache();
   
   return NewMesh;
}


// -------------------------------------------------------------------------
void BoolMesh::clearCache(void)
{
   this->_cache.clear();
}


// -------------------------------------------------------------------------
void BoolMesh::flushCache(void)
{
   if (!this->_cache.is_valid())
      std::cerr << "Warning: the cache is not a valid surface.\n";
   //std::cerr  << "flushCache(), before creation of N \n";
   Nef_polyhedron N(this->_cache);
   //std::cerr  << "flushCache(), after creation of N \n";
   this->CGALpoly = N;
}


// -------------------------------------------------------------------------
void BoolMesh::makeTriangleCache(const float *v1, const float *v2,
				 const float *v3)
{
   Point p(v1[0], v1[1], v1[2]);
   Point q(v2[0], v2[1], v2[2]);
   Point r(v3[0], v3[1], v3[2]);
   this->_cache.make_triangle(p, q, r);
   return;
}

// -------------------------------------------------------------------------
void BoolMesh::makeTetrahedronCache(const float *v1, const float *v2,
				    const float *v3, const float *v4)
{
   Point p(v1[0], v1[1], v1[2]);
   Point q(v2[0], v2[1], v2[2]);
   Point r(v3[0], v3[1], v3[2]);
   Point s(v4[0], v4[1], v4[2]);
   this->_cache.make_tetrahedron(p, q, r, s);
   return;
}

// -------------------------------------------------------------------------
void BoolMesh::addSurface(Surface f)
{
   this->_cache.delegate(f);
   return;
}

// -------------------------------------------------------------------------
void BoolMesh::updateCache(void)
{
   if(!this->CGALpoly.is_simple())
      std::cerr << "Warning: the CGAL polygon is not a valid surface.\n";

   this->CGALpoly.convert_to_polyhedron(this->_cache);

   if (!this->_cache.is_valid())
      std::cerr << "Warning: the cache is not a valid surface.\n";
}

// -------------------------------------------------------------------------
float * BoolMesh::verticesCache(unsigned int &nvertices)
{
   Point p;
   Vertex_iterator v;
   float *array;
   unsigned int index = 0;

   nvertices = this->_cache.size_of_vertices();
   array = new float[3*nvertices];

   for (v = this->_cache.vertices_begin(); 
	v != this->_cache.vertices_end(); ++v)
   {
      p = v->point();
      array[index] = (float) (CGAL::to_double(p.x()));
      array[index+1] = (float) (CGAL::to_double(p.y()));
      array[index+2] = (float) (CGAL::to_double(p.z()));
      index += 3;
   }
   
   return array;
}


// -------------------------------------------------------------------------
unsigned int **BoolMesh::facesCache(unsigned int &nfaces, 
				    std::vector<unsigned int> &nindex)
{
   Facet_iterator f;
   Halfedge_facet_circulator hf;
   unsigned int **array;
   unsigned int nvertex;
   unsigned int i, j, indexv;

   nindex.clear();
   nfaces = this->_cache.size_of_facets();
   array = new unsigned int *[nfaces];

   i = 0;
   for (f = this->_cache.facets_begin(); f != this->_cache.facets_end(); ++f) 
   {
      hf = f->facet_begin();
      nvertex = CGAL::circulator_size(hf);

      //  Facets in polyhedral surfaces are at least triangles
      CGAL_assertion(nvertex >= 3);

      nindex.push_back(nvertex);
      array[i] = new unsigned int[nvertex];

      j = 0;
      do {
	 indexv = std::distance(this->_cache.vertices_begin(), hf->vertex());
	 array[i][j] = indexv;
	 j++;
      } while (++hf != f->facet_begin());

      i++;
   }

   return array;
}


// -------------------------------------------------------------------------
void BoolMesh::translatef(float x, float y, float z)
{
   Aff_transformation transl(CGAL::TRANSLATION, Vector_3(x, y, z));
   this->CGALpoly.transform(transl);
} 

// -------------------------------------------------------------------------
void BoolMesh::scalef(float s)
{
   Aff_transformation transl(CGAL::SCALING, s);
   this->CGALpoly.transform(transl);
   this->updateCache();
} 

// -------------------------------------------------------------------------
void BoolMesh::rotatef(float degrees, float nx, float ny, float nz)
{
   double r = degrees * PI / 180.0;
   Aff_transformation transf(cos(r)+nx*nx*(1-cos(r)), 
			     nx*ny*(1-cos(r))-nz*sin(r),
			     nx*nz*(1-cos(r))+ny*sin(r),
			     0,

			     ny*nx*(1-cos(r))+nz*sin(r), 
			     cos(r)+ny*ny*(1-cos(r)),
			     ny*nz*(1-cos(r))-nx*sin(r),
			     0,
			     
			     nz*nx*(1-cos(r))-ny*sin(r),
			     nz*ny*(1-cos(r))+nx*sin(r),
			     cos(r)+nz*nz*(1-cos(r)),
			     0);

   this->CGALpoly.transform(transf);
   this->updateCache();
   return;
} 


// -------------------------------------------------------------------------
void BoolMesh::multMatrix(const float *matrix)
{
   Aff_transformation transf(matrix[0], matrix[1], matrix[2], matrix[3],
			     matrix[4], matrix[5], matrix[6], matrix[7], 
			     matrix[8], matrix[9], matrix[10], matrix[11],

			     1);
   this->CGALpoly.transform(transf);
   this->updateCache();
} 

// -------------------------------------------------------------------------
void BoolMesh::multMatrix (const glm::mat4 matrix)
{
    // OpenGL stores it's model matrices column major, but CGAL expects them to be row major,
    // thus they have to be transposed.
    glm::mat4 transposed = glm::transpose(matrix);

    const float *matrix2 = (const float*)glm::value_ptr(transposed);

    /*
    out << endl << "Model matrix passed to multMatrix():" << endl;
    Util::printMatrix(matrix);
    out << endl << "Transposed model matrix passed to multMatrix():" << endl;
    Util::printMatrix(glm::transpose(matrix));
    */

    /*
    out << "Model vector passed to multMatrix():" << endl;
    for(int i=0; i<16; ++i)
    {
        out << matrix2[i] << " ";
    }
    out << endl << endl;
    */

    // Dynamic memory so that CGAL doesn't produce a Segmentation fault later.
    float * passedMatrix = new float [16];
    for(int i=0; i<16; ++i)
    {
        passedMatrix[i] = matrix2[i];
    }

    /*
    out << "FINAL Model vector passed to multMatrix():" << endl;
    for(int i=0; i<16; ++i)
    {
        out << passedMatrix[i] << " ";
    }
    out << endl << endl;
    */

    multMatrix(passedMatrix);

    delete[] passedMatrix;
}


// -------------------------------------------------------------------------
BoolMesh& BoolMesh::operator=(const BoolMesh& b) 
{
   this->_cache.clear();
   this->CGALpoly = Nef_polyhedron(b.CGALpoly);
   this->updateCache();
   return *this;
}
