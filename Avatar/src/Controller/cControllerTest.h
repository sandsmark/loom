#pragma once

#include <Avatar/Controller/IController.h>
#include <windows.h>	// DWORD

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API cControllerTest : public IController
{
protected:
	float mFrequency;
	float mTime;
	DWORD mLastUpdate;
	cOgreAvatar *mAvatar;

public:
	cControllerTest();
	virtual ~cControllerTest();

	void SetFrequency( const float iFrequency ) { mFrequency = iFrequency; }

	// IController methods
	virtual void Init( cOgreAvatar *iAvatar );
	virtual void Update( void );
};


END_NAMESPACE()
