#pragma once

#include <Avatar/Event/Responders/IAvatarListenerResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderListenerNotifyBones : public IAvatarListenerResponder<cAvatarResponderListenerNotifyBones>
{
public:
	class cParam
	{
	public:
		size_t Id;
		size_t NumBones;
		const IAvatarListenerEvent::sBone *Bones;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Id = vData->Id;
			NumBones = vData->NumBones;
			Bones = new IAvatarListenerEvent::sBone[ NumBones ];
			memcpy( (void*)Bones, vData->Bones, sizeof( IAvatarListenerEvent::sBone ) * NumBones );
		}
	};

public:
	cAvatarResponderListenerNotifyBones() : IAvatarListenerResponder( _T("Avatar::cAvatarResponderNotifyBones") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarListenerEvent::OnNotifyBones, vParam.Id, vParam.Bones, vParam.NumBones );
	}
};

END_NAMESPACE()