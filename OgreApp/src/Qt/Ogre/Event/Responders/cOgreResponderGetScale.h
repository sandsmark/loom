#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderGetScale : public IOgreResponder<cOgreResponderGetScale>
{
public:
	class cParam
	{
	public:
		Ogre::Vector3 *Scale;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			Scale = ((Ogre::Vector3*)iParam.GetData());
			char *iName = ((char*)iParam.GetData())+sizeof(Ogre::Vector3);
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderGetScale() : IOgreResponder( _T("Ogre::cOgreResponderGetScale") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnGetScale, vParam.Name, *vParam.Scale );
	}
};

END_NAMESPACE()