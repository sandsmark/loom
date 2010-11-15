#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderSetEffectorPosition : public IAvatarResponder<cAvatarResponderSetEffectorPosition>
{
public:
	class cParam
	{
	public:
		eEffector Effector;
		Ogre::Vector3 Position;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Effector = vData->Effector;
			Position = vData->Position;
		}
	};

public:
	cAvatarResponderSetEffectorPosition() : IAvatarResponder( _T("Avatar::cAvatarResponderSetPosition") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnSetEffectorPosition, vParam.Effector, vParam.Position );
	}
};

END_NAMESPACE()