#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderGetEffectorPosition : public IAvatarResponder<cAvatarResponderGetEffectorPosition>
{
public:
	class cParam
	{
	public:
		size_t Id;
		eEffector Effector;
		Ogre::Vector3 *Position;

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
	cAvatarResponderGetEffectorPosition() : IAvatarResponder( _T("Avatar::cAvatarResponderSetPosition") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnSetEffectorPosition, vParam.Id, vParam.Effector, *vParam.Position );
	}
};

END_NAMESPACE()