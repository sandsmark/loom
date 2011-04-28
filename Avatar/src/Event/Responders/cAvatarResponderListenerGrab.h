#pragma once

#include <Avatar/Event/Responders/IAvatarListenerResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderListenerGrab : public IAvatarListenerResponder<cAvatarResponderListenerGrab>
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
			Id = *((size_t*)iParam.GetData());
			Effector = *(eEffector*)(((char*)iParam.GetData())+sizeof(cOgreAvatar*));
			char *iName = ((char*)iParam.GetData())+sizeof(cOgreAvatar*)+sizeof(eEffector);
			size_t vLength = strlen( iName ) + 1;
			char *vTemp = new char[ vLength ];
			strcpy_s( vTemp, vLength, iName );
			Name = vTemp;
		}
	};

public:
	cAvatarResponderListenerGrab() : IAvatarListenerResponder( _T("Avatar::cAvatarResponderListenerGrab") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarListenerEvent::OnNotifyGrab, vParam.Id, vParam.Effector, vParam.Name );
	}
};

END_NAMESPACE()