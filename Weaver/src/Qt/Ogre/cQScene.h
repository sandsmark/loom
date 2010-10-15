#pragma once
#include <Weaver/Qt/Ogre/cQOgre.h>

BEGIN_NAMESPACE( Weaver )

class cQScene : public cQOgre
{
protected:

	virtual bool InitResources( void );
	virtual bool InitPost( void );
public:
	cQScene( QWidget *iParent = NULL );

	virtual void RenderFrame( void );
};

END_NAMESPACE()