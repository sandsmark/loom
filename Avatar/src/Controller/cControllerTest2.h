#pragma once

#include <Avatar/Controller/IController.h>
#include <windows.h>	// DWORD
#include <Ogre/OgreVector3.h>
#include <Core/Spline/cMotion.h>

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API cControllerTest2 : public IController
{
protected:
	enum eStage
	{
		STAGE_INIT = 0,
		STAGE_ONE,		// Reach
		STAGE_TWO,		// Grab
		STAGE_THREE,	// Up
		STAGE_FOUR,		// Down
		STAGE_FIVE,		// Release
		STAGE_SIX,		// Rest
		STAGE_SEVEN,	// Reach again
		STAGE_EIGHT,	// Grab
		STAGE_NINE,		// Up
		STAGE_TEN,		// Down
		STAGE_ELEVEN,	// Release
		STAGE_TWELVE,	// Rest
		STAGE_DONE,

		STAGE_MAX,
	};
	eStage mStage;

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

	void UpdateStageOne( void );
	void UpdateStageTwo( void );
	void UpdateStageThree( void );
	void UpdateStageFour( void );
	void UpdateStageFive( void );
	void UpdateStageSix( void );
	void UpdateStageSeven( void );
	void UpdateStageEight( void );
	void UpdateStageNine( void );
	void UpdateStageTen( void );
	void UpdateStageEleven( void );
	void UpdateStageTwelve( void );

public:
	cControllerTest2();
	virtual ~cControllerTest2();

	void SetFrequency( const float iFrequency ) { mFrequency = iFrequency; }
	void NextStage( void )
	{
		if ( mStage < STAGE_MAX-1 )
		{
			mStage = (eStage)(((int)(mStage)) + 1);
		}
	}

	// IController methods
	virtual void Init( cOgreAvatar *iAvatar );
	virtual void Update( void );
};


END_NAMESPACE()
