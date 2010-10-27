#pragma once
#include <OgreApp/Qt/Ogre/cQOgre.h>

BEGIN_NAMESPACE( OgreApp )

class OGREAPP_API cQScene : public cQOgre
{
protected:

	virtual bool InitResources( void );
	virtual bool InitPost( void );
public:
	cQScene( QWidget *iParent = NULL );

	virtual void RenderFrame( void );
};

END_NAMESPACE()