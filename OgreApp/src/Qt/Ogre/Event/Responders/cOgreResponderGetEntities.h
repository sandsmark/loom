#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderGetEntities : public IOgreResponder<cOgreResponderGetEntities>
{
public:
	class cParam
	{
	public:
		cArray<Ogre::String> *Names;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			Names = ((cArray<Ogre::String>*)iParam.GetData());
		}
	};

public:
	cOgreResponderGetEntities() : IOgreResponder( _T("Ogre::cOgreResponderGetEntities") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnGetEntities, *vParam.Names );
	}
};

END_NAMESPACE()