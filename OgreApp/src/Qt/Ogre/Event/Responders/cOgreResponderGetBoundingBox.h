#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderGetBoundingBox : public IOgreResponder<cOgreResponderGetBoundingBox>
{
public:
	class cParam
	{
	public:
		Ogre::AxisAlignedBox *Bounds;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			Bounds = ((Ogre::AxisAlignedBox*)iParam.GetData());
			char *iName = ((char*)iParam.GetData())+sizeof(Ogre::Vector3);
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cOgreResponderGetBoundingBox() : IOgreResponder( _T("Ogre::cOgreResponderGetBoundingBox") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnGetBoundingBox, vParam.Name, *vParam.Bounds );
	}
};

END_NAMESPACE()