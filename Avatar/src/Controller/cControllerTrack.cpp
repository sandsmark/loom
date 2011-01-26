#include <Avatar/Controller/cControllerTrack.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Ogre/OgreVector3.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::Avatar;
using Loom::Core::cModuleManager;
using Loom::TrackStar::cModuleTrackStar;

/************************************************************************/
cControllerTrack::cControllerTrack()
: mFrequency( 1 ), mTime( 0 ), mAvatar( NULL )
/************************************************************************/
{
	mLastUpdate = GetTickCount();
	mInterface = NULL;
}

/************************************************************************/
cControllerTrack::~cControllerTrack()
/************************************************************************/
{
}

/************************************************************************/
void cControllerTrack::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;
}

/************************************************************************/
void cControllerTrack::Update()
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	if ( mAvatar == NULL ) return;

	mTime += vEllapsed;

	if ( mAvatar->mCalibrationState != mAvatar->CALIBRATION_DONE ) return;

	if (!mInterface) {
		cModuleTrackStar *vTrackStar = (cModuleTrackStar*)cModuleManager::Get().GetModule( cModuleTrackStar::GetName() );
		if (!vTrackStar) return;
		mInterface = vTrackStar->GetInterface();
		if (!mInterface) return;
	}

	int vCount = 0;
	int vDone = 0;
	Ogre::Vector3 vLeft, vRight, vHead;
	while ( vDone != 7 )
	{
		int vSensorId;
		double vX, vY, vZ, vA, vE, vR;
		mInterface->getNextEntry( vSensorId, vX, vY, vZ, vA, vE, vR, 0 );
		switch ( vSensorId )
		{
		case EFFECTOR_LEFT_HAND:
			vLeft = Ogre::Vector3( vX, vY, vZ );
			vDone = vDone | 1;
			break;
		case EFFECTOR_RIGHT_HAND:
			vRight = Ogre::Vector3( vX, vY, vZ );
			vDone = vDone | 2;
			break;
		case EFFECTOR_HEAD:
			vHead = Ogre::Vector3( vX, vY, vZ );
			vDone = vDone | 4;
			break;
		}
		if (vCount++ > 10)
			return;
	}

	mAvatar->ResetBones();

	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND , vLeft );
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, vRight );
//	mAvatar->SetEffectorPosition( EFFECTOR_HEAD, vHead );

}
