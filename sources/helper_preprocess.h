#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#define CHECK_FRAMEBUFFER_STATUS() \
{ \
	GLenum status; \
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); \
	switch(status) { \
		  case GL_FRAMEBUFFER_COMPLETE_EXT: \
		  printf("> frambuffer complet\n"); \
		  break; \
		  case GL_FRAMEBUFFER_UNSUPPORTED_EXT: \
		  printf("combinaison de formats non supportée par l'implémentation\n"); \
		  /* choose different formats */ \
		  break; \
		  default: \
		  printf("programming error; will fail on all hardware\n"); \
		  break; \
	} \
}

#define CHECK_GL_ERRORS(s) \
{ \
	GLenum err_gl = glGetError( ); \
	if (err_gl != GL_NO_ERROR) { \
	printf("<%s> OpenGL error : %s\n", s, gluErrorString(err_gl)); \
	} \
 }

#define ERRORMSG(str)   printf("Error: %s\n",   (char *) str)
#define WARNINGMSG(str) printf("Warning: %s\n", (char *) str)
#define INFOMSG(str)    printf("%s\n",          (char *) str)

// DEFINES
#define LOADDIRSHADER(_qStringDir, _prog) 									\
{														\
	std::cout << "# Load shader, path used : " <<  _qStringDir.toAscii().constData() << std::endl;		\
	_prog.loadDir(_qStringDir);										\
	}													\

#define LOADSHADER(_qStringShader, _prog) 									\
{														\
	std::cout << "# Load shader, path used : " <<  _qStringShader.toAscii().constData() << std::endl;	\
	_prog.load( _qStringShader );										\
	}													\

#endif
