#pragma once

#include <Avatar/Event/Responders/IAvatarListenerResponder.h>
#include <Ogre/OgreQuaternion.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderListenerNotifyEffectorRotation : public IAvatarListenerResponder<cAvatarResponderListenerNotifyEffectorRotation>
{
public:
	class cParam
	{
	public:
		size_t Id;
		eEffector Effector;
		Ogre::Quaternion Rotation;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Id = vData->Id;
			Effector = vData->Effector;
			Rotation = vData->Rotation;
		}
	};

public:
	cAvatarResponderListenerNotifyEffectorRotation() : IAvatarListenerResponder( _T("Avatar::cAvatarResponderNotifyRotation") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarListenerEvent::OnNotifyEffectorRotation, vParam.Id, vParam.Effector, vParam.Rotation );
	}
};

END_NAMESPACE()