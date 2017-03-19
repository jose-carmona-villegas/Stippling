#include "framebuffer.hh"
//#define FORCE_NO_FBO


// --------------------------------------------------
S3DFBO::S3DFBO(unsigned int width, unsigned int height, 
	       GLenum format, GLenum flag, GLenum type, GLenum interp, 
	       bool withdepthbuffer, bool withstencilbuffer)
{
   int maxtexsize;

   this->img = 0;

   this->backup = 0;
   this->old_viewport = new GLint[4];
   glGetIntegerv( GL_VIEWPORT, old_viewport);

   this->fbo = 0;
   this->depthbuffer = 0;

   this->w = width;
   this->h = height;

   this->fmt = format;
   this->flg = flag;
   this->tp = type;
   this->intr = interp;

#ifndef FORCE_NO_FBO

   glGenFramebuffers(1, &(this->fbo));
   if (withdepthbuffer == true)
      glGenRenderbuffers(1, &(this->depthbuffer));

   glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

//   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxtexsize);
//   std::cerr << "GL_MAX_TEXTURE_SIZE = " << maxtexsize << "\n";

   if (this->img == 0)
      glGenTextures(1, &(this->img));

   glBindTexture(GL_TEXTURE_2D, this->img);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interp);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interp);

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   glTexImage2D(GL_TEXTURE_2D, 0, format,  this->w, this->h, 
		0, flag, type, 0);

   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			     GL_TEXTURE_2D, this->img, 0);


   if (withdepthbuffer == true)
   {
      glBindTexture(GL_TEXTURE_2D, this->depthbuffer);
      //    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
      //           width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, 
//                      GL_COMPARE_R_TO_TEXTURE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      glBindRenderbuffer(GL_RENDERBUFFER, this->depthbuffer);
      if (withstencilbuffer == false)
         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
				  width, height);
      else
      {
         glRenderbufferStorage(GL_RENDERBUFFER,
                                  GL_DEPTH_STENCIL,
				  width, height);
         glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                      GL_DEPTH_ATTACHMENT,
                                      GL_RENDERBUFFER, this->depthbuffer);

         glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                      GL_STENCIL_ATTACHMENT,
                                      GL_RENDERBUFFER, this->depthbuffer);
      }
   }

   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);


   this->status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

#else

   this->status = GL_FRAMEBUFFER_UNSUPPORTED;

#endif
}


// --------------------------------------------------
S3DFBO::~S3DFBO()
{
   if (this->old_viewport != 0)
      delete [] this->old_viewport;

   if (this->backup != 0)
      delete [] this->backup;

   if (this->img != 0)
   {
      glDeleteTextures(1, &(this->img));
      this->img = 0;
   }


   if (this->depthbuffer != 0)
      glDeleteRenderbuffers(1, &(this->depthbuffer));

   if (this->fbo != 0)
      glDeleteFramebuffers(1, &(this->fbo));
}


// --------------------------------------------------
void S3DFBO::renderFBO(void)
{
   GLint new_viewport[4];


#ifndef FORCE_NO_FBO

//   glPushAttrib(GL_ALL_ATTRIB_BITS);

   glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

   glDrawBuffer(GL_COLOR_ATTACHMENT0);
   glReadBuffer(GL_COLOR_ATTACHMENT0);

   glPushAttrib(GL_VIEWPORT_BIT);

#else

   glGetIntegerv (GL_VIEWPORT, new_viewport);
   if ( (new_viewport[2] != this->old_viewport[2]) || 
	(new_viewport[3] != this->old_viewport[3]) || 
	(this->backup == 0))
   {
      memcpy(this->old_viewport, new_viewport, sizeof(GLint) * 4);
      if (this->backup != 0)
	 delete [] this->backup;

      this->backup = new unsigned char[this->old_viewport[2] * 
				       this->old_viewport[3] * 3];
   }

   glReadPixels(this->old_viewport[0], this->old_viewport[1], 
		this->old_viewport[2], this->old_viewport[3], 
		GL_RGB, GL_UNSIGNED_BYTE, this->backup);   
   
   
//   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushAttrib(GL_VIEWPORT_BIT);

#endif

   glViewport(0, 0, this->w, this->h);
}


// --------------------------------------------------
void S3DFBO::renderFramebuffer(void)
{
   unsigned char *bk;

#ifndef FORCE_NO_FBO

   glPopAttrib();

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);

#else

   bk = new unsigned char[this->w * this->h * this->fmt];
   glReadPixels(0, 0, this->w, this->h, 
		this->flg, this->tp, bk);   
   
   // We can use the depthbuffer texture as background in this case
   if (this->img == 0)
      glGenTextures( 1, &(this->img));

   glBindTexture (GL_TEXTURE_2D, this->img);

   glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
   // the texture wraps over at the edges (repeat)
   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   glTexImage2D(GL_TEXTURE_2D, 0, this->fmt, this->w, this->h,
		0, this->flg, this->tp, bk);
   
   delete [] bk;


   glPopAttrib();

   
//   glViewport(this->old_viewport[0], this->old_viewport[1], 
//	      this->old_viewport[2], this->old_viewport[3]);
   if (this->backup != 0)
   {
//      glRasterPos2i(this->old_viewport[0], this->old_viewport[1]);
      glDrawPixels(this->old_viewport[2], this->old_viewport[3], 
		   GL_RGB, GL_UNSIGNED_BYTE, this->backup);
   }


#endif

}


// --------------------------------------------------
bool S3DFBO::isValid (void)
{
   bool isOK = false;

   this->status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

   switch(this->status) {                                          
      case GL_FRAMEBUFFER_COMPLETE: { // Everything's OK
	 isOK = true;
      } break;
	 
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n";
      } break;
	 
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n";
      } break;
	 
        /*
      case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n";
      } break;
	 
      case GL_FRAMEBUFFER_INCOMPLETE_FORMATS: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n";
      } break;
        */

      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
      } break;
	 
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
      } break;
	 
      case GL_FRAMEBUFFER_UNSUPPORTED: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
                   << "GL_FRAMEBUFFER_UNSUPPORTED\n";
      } break;
	 
      default: {
	 std::cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
		   << "Unknown ERROR\n";
      }
   }

   return isOK;
}


// --------------------------------------------------
GLuint S3DFBO::getTexture (void)
{
   return this->img;
}
