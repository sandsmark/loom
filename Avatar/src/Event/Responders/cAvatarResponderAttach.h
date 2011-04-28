#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderAttach : public IAvatarResponder<cAvatarResponderAttach>
{
public:
	class cParam
	{
	public:
		size_t Id;
		eEffector Effector;
		Ogre::String Name;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *vData = (char*)iParam.GetData();
			Id = *(size_t*)vData; vData += sizeof( size_t );
			Effector = *(eEffector*)vData; vData += sizeof( eEffector );
			char *iName = (char*)vData;
			size_t vLength = strlen( iName ) + 1;
			char *vTemp = new char[ vLength ];
			strcpy_s( vTemp, vLength, iName );
			Name = vTemp;
		}
	};

public:
	cAvatarResponderAttach() : IAvatarResponder( _T("Avatar::cAvatarResponderAttach") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnAttach, vParam.Id, vParam.Name, vParam.Effector );
	}
};

END_NAMESPACE()