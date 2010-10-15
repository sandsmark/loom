#pragma once

#include <Weaver/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Weaver )

class cOgreResponderCreateBox : public IOgreResponder<cOgreResponderCreateBox>
{
public:
	class cParam
	{
	public:
		Ogre::Vector3 Position;
		Ogre::Vector3 Size;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *vData = (char*)iParam.GetData();
			Position = *((Ogre::Vector3*)vData); vData += sizeof( Ogre::Vector3 );
			Size     = *((Ogre::Vector3*)vData); vData += sizeof( Ogre::Vector3 );
			char *iName = vData;
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderCreateBox() : IOgreResponder( _T("Ogre::CreateBox") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnCreateBox, vParam.Name, vParam.Position, vParam.Size );
	}
};

END_NAMESPACE()