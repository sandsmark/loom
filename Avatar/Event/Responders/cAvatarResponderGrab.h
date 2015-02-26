#pragma once

#include <Avatar/Event/Responders/IAvatarResponder.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( Avatar )

class cAvatarResponderGrab : public IAvatarResponder<cAvatarResponderGrab>
{
public:
	class cParam
	{
	public:
		size_t Id;
		eEffector Effector;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			cParam *vData = (cParam*)iParam.GetData();
			Id = vData->Id;
			Effector = vData->Effector;
		}
	};

public:
	cAvatarResponderGrab() : IAvatarResponder( _T("Avatar::cAvatarResponderGrab") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IAvatarEvent::OnGrab, vParam.Id, vParam.Effector );
	}
};

END_NAMESPACE()