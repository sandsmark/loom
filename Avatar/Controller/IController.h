#pragma once

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API IController
{
protected:
	bool mActive;

public:
	virtual ~IController() {}

	virtual void Init( cOgreAvatar *iAvatar ) = 0;
	virtual void Update() = 0;

	virtual bool IsActive( void ) const { return mActive; }
	virtual void SetActive( bool iActive ) { mActive = iActive; }
};


END_NAMESPACE()
