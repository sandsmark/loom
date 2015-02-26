#pragma once

#include <Avatar/Controller/IController.h>
#include <TrackStar/TrackStar.h>
#include <TrackStar/Module/cModuleTrackStar.h>
#include <TrackStar/Module/TrackStarInterface.h>
#include <Core/Module/cModuleManager.h>
#include <windows.h>	// DWORD

using Loom::TrackStar::TrackStarInterface;

BEGIN_NAMESPACE( Avatar )

class cOgreAvatar;

class AVATAR_API cControllerTrack : public IController
{
protected:
	float mFrequency;
	float mTime;
	DWORD mLastUpdate;
	cOgreAvatar *mAvatar;
	TrackStarInterface *mInterface;

public:
	cControllerTrack();
	virtual ~cControllerTrack();

	void SetFrequency( const float iFrequency ) { mFrequency = iFrequency; }

	// IController methods
	virtual void Init( cOgreAvatar *iAvatar );
	virtual void Update( void );
};


END_NAMESPACE()
