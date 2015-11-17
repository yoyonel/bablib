#include "TimerQueryNV.h"
#include <Message.h>

CTimerQueryNV::CTimerQueryNV()
{
	//init();
	bInit = false;
}

CTimerQueryNV::~CTimerQueryNV()
{
	destroy();
}

void CTimerQueryNV::init()
{
	if (!bInit) {
		glGenQueries( 1, &uiIdQuery );
		bInit = true;
	}
}

void CTimerQueryNV::destroy()
{
	if (bInit) {
		glDeleteQueries( 1, &uiIdQuery );
	}
	bInit = false;
}
	
void CTimerQueryNV::wait()
{
	if ( bInit && bValid ) {
		// il faut etre sure qu'il y ait un end() avant d'attendre ! (sinon boucle infinie !)
		// Wait for all results to become available
		do {
		    glGetQueryObjectiv( uiIdQuery, GL_QUERY_RESULT_AVAILABLE, &iAvailabe);
		} while (!iAvailabe);
	}
}

void CTimerQueryNV::update( bool _bForceWait ) 
{
	if (bInit) {
		if ( _bForceWait) wait();
	
		// See how much time the rendering of object i took in nanoseconds.
		glGetQueryObjectui64vEXT( uiIdQuery, GL_QUERY_RESULT, &ui64TimeElapsed );
	}
}
