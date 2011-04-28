#pragma once

#include <Avatar/Controller/IController.h>
#include <windows.h>	// DWORD
#include <Ogre/OgreVector3.h>
#include <Core/Spline/cMotion.h>

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API cControllerMouse : public IController
{
protected:
protected:
	float mFrequency;
	float mTime;
	DWORD mLastUpdate;
	cOgreAvatar *mAvatar;

	Ogre::Vector3 mLeftHand;
	Ogre::Vector3 mRightHand;
	float         mHead;

	Ogre::Vector3 mPrevPos;
	float         mPrevGrab;

	bool mGrab;

	bool mPoint;
	Loom::Core::cMotion<float> mPointAlpha;

	void UpdateGrab( void );
	void UpdatePoint( void );

public:
	cControllerMouse();
	virtual ~cControllerMouse();

	void SetFrequency( const float iFrequency ) { mFrequency = iFrequency; }

	// IController methods
	virtual void Init( cOgreAvatar *iAvatar );
	virtual void Update( void );

	virtual void SetActive( bool iActive );
};


END_NAMESPACE()
