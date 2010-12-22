#pragma once

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API IController
{
public:
	virtual ~IController() {}

	virtual void Init( cOgreAvatar *iAvatar ) = 0;
	virtual void Update() = 0;
};


END_NAMESPACE()
