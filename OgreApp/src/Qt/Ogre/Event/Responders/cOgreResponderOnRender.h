#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreListenerResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderOnRender : public IOgreListenerResponder<cOgreResponderOnRender>
{
public:
	cOgreResponderOnRender() : IOgreListenerResponder( _T("Ogre::cOgreResponderOnRender") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		Dispatch( &IOgreListenerEvent::OnRender );
	}
};

END_NAMESPACE()