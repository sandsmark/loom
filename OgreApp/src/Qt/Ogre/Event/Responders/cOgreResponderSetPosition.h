#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderSetPosition : public IOgreResponder<cOgreResponderSetPosition>
{
public:
	class cParam
	{
	public:
		Ogre::Vector3 Position;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			Position = *((Ogre::Vector3*)iParam.GetData());
			char *iName = ((char*)iParam.GetData())+sizeof(Ogre::Vector3);
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderSetPosition() : IOgreResponder( _T("Ogre::cOgreResponderSetPosition") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnSetPosition, vParam.Name, vParam.Position );
	}
};

END_NAMESPACE()