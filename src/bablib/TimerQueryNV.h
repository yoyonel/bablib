#ifndef __CLASS_TIMER_QUERY_H__
#define __CLASS_TIMER_QUERY_H__

#include <GL/glew.h>
#include <GL/gl.h>

class CTimerQueryNV {
	public:
	CTimerQueryNV();
	~CTimerQueryNV();
	
	void init();
	void destroy();

	void wait();
	void update( bool _bForceWait = false );
	inline void 		begin() 			{ bValid = false; glBeginQuery(GL_TIME_ELAPSED_EXT, uiIdQuery); }
	inline void 		end() 				{ glEndQuery(GL_TIME_ELAPSED_EXT); bValid = true; }
	// resultat en nanoseconde (nano, micro, milli, etc ...) (10e-9)
	inline GLuint64EXT 	getTimeElapsed() 	const 	{ return ui64TimeElapsed; }
	inline GLuint 		getId() 		const 	{ return uiIdQuery; }

	private:
	GLuint uiIdQuery;
	GLint iAvailabe;
	GLuint64EXT ui64TimeElapsed;

	bool bInit;
	bool bValid;
};

#endif
