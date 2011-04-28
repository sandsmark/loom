#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderDetach : public IAvatarResponder<cAvatarResponderDetach>
{
public:
	class cParam
	{
	public:
		size_t Id;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *vData = (char*)iParam.GetData();
			Id = *(size_t*)vData; vData += sizeof( size_t );
			char *iName = (char*)vData;
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
		}
	};

public:
	cAvatarResponderDetach() : IAvatarResponder( _T("Avatar::cAvatarResponderDetach") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnDetach, vParam.Id, vParam.Name );
	}
};

END_NAMESPACE()