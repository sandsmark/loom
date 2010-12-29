#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderMoveTo : public IOgreResponder<cOgreResponderMoveTo>
{
public:
	class cParam
	{
	public:
		float Speed;
		Ogre::Vector3 Position;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *iData = (char*)iParam.GetData();
			Speed = *((float*)iData); iData += 4;
			Position = *((Ogre::Vector3*)iData); iData += sizeof(Ogre::Vector3);
			char *iName = ((char*)iData);
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderMoveTo() : IOgreResponder( _T("Ogre::cOgreResponderMoveTo") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnMoveTo, vParam.Name, vParam.Position, vParam.Speed );
	}
};

END_NAMESPACE()