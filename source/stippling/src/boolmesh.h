#ifndef BOOLEAN_MESH_
#define BOOLEAN_MESH_

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_VRML_2_ostream.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>


////////////////////// Necessary for GLM interface support ////////////////
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
// quaternion
#include <glm/gtx/quaternion.hpp>
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include <QVector>

#include <util.h>

#ifndef PI
//#define PI 3.14159265
#endif 

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Point_3<Kernel> Point;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;
typedef Polyhedron::HalfedgeDS HalfedgeDS;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Nef_polyhedron::Aff_transformation_3  Aff_transformation;
typedef Nef_polyhedron::Vector_3  Vector_3;

// A modifier_baseifier creating a triangle with the incremental builder.
template <class HDS>
class SurfaceM : public CGAL::Modifier_base<HDS>
{
  private:
   std::vector<Point> _vertices;
   std::vector< std::vector<unsigned int> > _faces;
  public:
   SurfaceM() {}

   // TODO:  Finish transform matrix
   // http://www.ics.uci.edu/~dock/manuals/cgal_manual/Nef_3/Chapter_main.html
   // 15.5.3   Transformation


   unsigned int addVertex(float x, float y, float z)
   {
      this->_vertices.push_back(Point(x, y, z));
      return this->_vertices.size() - 1;
   }

   unsigned int addVertex(glm::vec3 vertex)
   {
       return addVertex((float)(vertex.x), (float)(vertex.y), (float)(vertex.z));
   }

   void addFace(std::vector<unsigned int> idf)
   {
      std::vector <unsigned int> f(idf);
      this->_faces.push_back(f);
   }

   void addFace(QVector<unsigned int> idf)
   {
        std::vector <unsigned int> f(idf.toStdVector());
        this->_faces.push_back(f);
   }
   
   void operator()( HDS& hds)
   {
      unsigned int nvertices = this->_vertices.size();
      unsigned int nfaces = this->_faces.size();
      unsigned int i,j;

      // Postcondition: hds is a valid polyhedral surface.
      CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

      B.begin_surface(nvertices, nfaces);
      typedef typename HDS::Vertex   Vertex;
      typedef typename Vertex::Point Point;
      for (i=0; i < nvertices; i ++) // add vertices
	 B.add_vertex(this->_vertices[i]);
      for (i=0; i < nfaces; i ++) // add faces indices
      {
	 B.begin_facet();
	 for (j=0; j < this->_faces[i].size(); j ++)
	    B.add_vertex_to_facet(this->_faces[i][j]);
	 B.end_facet();
      }
      B.end_surface();
   }
};

typedef SurfaceM<HalfedgeDS> Surface;

class BoolMesh {
  private:
   Polyhedron _cache;
  public:

   /* **********  BOOLEAN OPERATIONS  ******** */
   static const unsigned int OP_DIFFERENCE = 0x0;
   static const unsigned int OP_INTERSECTION = 0x1;
   static const unsigned int OP_UNION = 0x2;
   static const unsigned int OP_JOIN = 0x2; // equivalent to UNION
   /* **************************************** */

   /* **********  CODES  ******** */
   static const int OK = 0x0;
   static const int ERROR_WRITE = -1;
   static const int ERROR_READ = -2;
   static const int ERROR_NOT_SIMPLE = -3; 
   // ERROR_NOT_SIMPLE = model is not simple (cannot be converted)
   /* **************************************** */

   // NEF POLYHEDRON
   Nef_polyhedron CGALpoly; // http://doc.cgal.org/latest/Nef_3/classCGAL_1_1Nef__polyhedron__3.html

   // Constructor
   BoolMesh(); 

   // Constructor
   BoolMesh(BoolMesh &org); 

   // Assignement operator
   BoolMesh& operator=(const BoolMesh&);

   // Load a mesh from an OFF file, return a code
   int loadOFFMesh(const char *filename);
   int loadOFFMesh(std::string filename);

   // Save mesh as an OFF file, return a code
   int saveOFFMesh(const char *filename);
   int saveOFFMesh(std::string filename);

   // Boolean operation
   BoolMesh boolean(BoolMesh model, unsigned int operation);

   // Make a triangle in the cache
   void makeTriangleCache(const float *v1, const float *v2, const float *v3);

   // Make a tetrahedron in the cache
   void makeTetrahedronCache(const float *v1, const float *v2,
			     const float *v3, const float *v4);

   // Clear the cache
   void clearCache(void);

   // Flush the cache
   void flushCache(void);

   // Add Surface to the cache
   void addSurface (Surface f);

   // Update the cache with the real model
   void updateCache(void);

   // Return an array of vertices for the cache, you need to free the memory
   // by using delete []
   float *verticesCache(unsigned int &nvertices);

   // Return an array of arrays of vertex indices, nvertices is a vector
   // containing the size of the sub-arrays of vertex indices,
   // you need to free all the memory by using a loop of delete [] 
   // and a final delete []
   unsigned int **facesCache(unsigned int &nfaces,
			     std::vector<unsigned int> &nvertices);

   // Translate the model (it doesn't affect the cached model)
   void translatef (float x, float y, float z);

   // Scale the model (it doesn't affect the cached model)
   void scalef (float s);

   // Rotate the model around a known axis (it doesn't affect the cached model)
   void rotatef (float degrees, float nx, float ny, float nz);

   // Multiply a matrix (4x4) to the model (it doesn't affect the cached model)
   // order is 00, 01, 02, 03, 10, 11, 12, 13, ...
   // See: http://www.ics.uci.edu/~dock/manuals/cgal_manual/Kernel_23_ref/fig/arrthree.gif
   void multMatrix (const float *matrix);

   void multMatrix (const glm::mat4 matrix);
   

   // Destructor
   ~BoolMesh(); 

};

#endif
