#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreQuaternion.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderSetEffectorRotation : public IAvatarResponder<cAvatarResponderSetEffectorRotation>
{
public:
	class cParam
	{
	public:
		eEffector Effector;
		Ogre::Quaternion Rotation;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Effector = vData->Effector;
			Rotation = vData->Rotation;
		}
	};

public:
	cAvatarResponderSetEffectorRotation() : IAvatarResponder( _T("Avatar::cAvatarResponderSetRotation") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnSetEffectorRotation, vParam.Effector, vParam.Rotation );
	}
};

END_NAMESPACE()