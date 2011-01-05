#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>
#include <Core/Datatype/cString.h>

using Loom::Core::cString;

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderClearLines : public IOgreResponder<cOgreResponderClearLines>
{
public:
	cOgreResponderClearLines() : IOgreResponder( _T("Ogre::cOgreResponderClearLines") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		Dispatch( &IOgreEvent::OnClearLines );
	}
};

END_NAMESPACE()