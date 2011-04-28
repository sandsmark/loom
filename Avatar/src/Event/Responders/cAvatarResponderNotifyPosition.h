#pragma once

#include <Avatar/Event/Responders/IAvatarListenerResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderListenerNotifyPosition : public IAvatarListenerResponder<cAvatarResponderListenerNotifyPosition>
{
public:
	class cParam
	{
	public:
		size_t Id;
		eEffector Effector;
		Ogre::Vector3 Position;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Id = vData->Id;
			Effector = vData->Effector;
			Position = vData->Position;
		}
	};

public:
	cAvatarResponderListenerNotifyPosition() : IAvatarListenerResponder( _T("Avatar::cAvatarResponderNotifyPosition") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarListenerEvent::OnNotifyEffectorPosition, vParam.Id, vParam.Effector, vParam.Position );
	}
};

END_NAMESPACE()