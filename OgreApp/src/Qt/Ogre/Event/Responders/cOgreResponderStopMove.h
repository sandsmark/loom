#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderStopMove : public IOgreResponder<cOgreResponderStopMove>
{
public:
	class cParam
	{
	public:
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *iData = (char*)iParam.GetData();
			char *iName = ((char*)iData);
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderStopMove() : IOgreResponder( _T("Ogre::cOgreResponderStopMove") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnStopMove, vParam.Name );
	}
};

END_NAMESPACE()