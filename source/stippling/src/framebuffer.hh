#ifndef __FBOBJ_OPENGL__
#define __FBOBJ_OPENGL__

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/glew.h>


/** @class   S3DFBO framebuffer.hh
 *  @author  Germán Arroyo
 *  @date    2007
 *  @brief   This class manages a framebuffer object of OpenGL
 *
 *  @bug     No bugs detected yet
 */

class S3DFBO {
   public:

      /** 
       * @param[in] width The width of the fbo
       * @param[in] height The width of the fbo
       * @param[in] format One of this: GL_RGBA8, GL_RGB, etc.
       * @param[in] flag One of this: GL_RGBA, GL_LUMINANCE, etc.
       * @param[in] type One of this: GL_UNSIGNED_BYTE, GL_FLOAT, etc.
       * @param[in] interp One of this: GL_NEAREST, GL_LINEAR, etc.
       * @param[in] withdepthbuffer If it is true, the frame buffer will have
       *            attached a depthbuffer
       * @post Constructor. Attach 1 texture to 1 FBO
       */
      S3DFBO (unsigned int width, unsigned int height,
	      GLenum format, GLenum flag, GLenum type, GLenum interp, 
	      bool withdepthbuffer = true, bool withstencilbuffer = true);

      /** 
       * @post Destructor, free the memory
       */
      ~S3DFBO ();

      /**
       * @post Switch to the FBO mode, so everything from this point is drawed
       *       in the FBO
       * @note This method must be called always before the renderFramebuffer
       * @warning If you use this method you must use the renderFramebuffer 
       *          after it
       */
      void renderFBO(void);

      /**
       * @post Switch to the framebuffer, from this point everything is drawed
       *      in framebuffer object
       * @warning You can use this method only if you have called previously to
       *          renderFBO
       */
      void renderFramebuffer(void);
      
      /**
       * @post Manage the errors and tell you the problem in case of error, 
       *       true in other case
       */
      bool isValid (void);

      /**
       * @post The texture of the FBO, this method is really fast
       */
      GLuint getTexture (void);     



   private:
      GLuint img; /// The texture of the FBO
      GLuint fbo; /// Framebuffer object id
      GLuint depthbuffer; /** Depth buffer id, 
			   *  the stencil buffer is also stored here
			   */
      unsigned int w, h; /// Size of the FBO
      GLenum status; /// Status of the FBO
      GLint *old_viewport; /// The old viewport
      unsigned char *backup; /// Backup if the FBO is unabled
      GLenum fmt, flg, tp, intr; /// Flags of the definition of the textures
};

#endif

