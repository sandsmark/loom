#include <Avatar/Controller/cControllerInternal.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Ogre/OgreVector3.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::Avatar;

/************************************************************************/
cControllerInternal::cControllerInternal()
: mFrequency( 1 ), mTime( 0 ), mAvatar( NULL )
/************************************************************************/
{
	mLastUpdate = GetTickCount();
}

/************************************************************************/
cControllerInternal::~cControllerInternal()
/************************************************************************/
{
}

/************************************************************************/
void cControllerInternal::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;
}

/************************************************************************/
void cControllerInternal::Update()
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	if ( mAvatar == NULL ) return;

	mTime += vEllapsed;

	Ogre::Vector3 vLeft, vRight, vHead;
	if (false) {
	}
	else {
		float vAlpha = sinf( mTime * mFrequency * M_PI );

		vLeft = Ogre::Vector3(  50 + sinf( vAlpha ) * 50, 100, 0 );
		vRight = Ogre::Vector3( -50 + sinf( vAlpha ) * 50, 100, 0 );
		vHead = Ogre::Vector3( 0, 0, 0 );
	}

	mAvatar->ResetBones();

	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND , vLeft );
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, vRight );
	// mAvatar->SetEffectorPosition( EFFECTOR_HEAD, vHead );
}
