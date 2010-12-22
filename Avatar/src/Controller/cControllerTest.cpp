#include <Avatar/Controller/cControllerTest.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Ogre/OgreVector3.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::Avatar;

/************************************************************************/
cControllerTest::cControllerTest()
: mFrequency( 1 ), mTime( 0 ), mAvatar( NULL )
/************************************************************************/
{
	mLastUpdate = GetTickCount();
}

/************************************************************************/
cControllerTest::~cControllerTest()
/************************************************************************/
{
}

/************************************************************************/
void cControllerTest::Init( cOgreAvatar *iAvatar )
/************************************************************************/
{
	ASSERTTXT( mAvatar == NULL, _T( "Avatar to control is already set!" ) );

	mAvatar = iAvatar;
}

/************************************************************************/
void cControllerTest::Update()
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	if ( mAvatar == NULL ) return;

	mTime += vEllapsed;

	mAvatar->ResetBones();

	float vAlpha = sinf( mTime * mFrequency * M_PI );

	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND , Ogre::Vector3(  50 + sinf( vAlpha ) * 50, 100, 0 ) );
	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -50 + sinf( vAlpha ) * 50, 100, 0 ) );

//	mAvatar->SetEffectorPosition( EFFECTOR_LEFT_HAND , Ogre::Vector3(  20, 50 + sinf( vAlpha ) * 50, 50 + cosf( vAlpha  *2 ) * 50 ) );
//	mAvatar->SetEffectorPosition( EFFECTOR_RIGHT_HAND, Ogre::Vector3( -20, 50 + sinf( vAlpha ) * 50, 50 + cosf( vAlpha * 2 ) * 50 ) );
}
