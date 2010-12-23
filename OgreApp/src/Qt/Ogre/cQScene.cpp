#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Core/Event/cDispatcherHub.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning( push )
#pragma warning( disable: 4189 )	// local variable is initialized but not referenced
#include <Ogre/Ogre.h>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable: 4512 )	// 'QLatin1Literal' : assignment operator could not be generated
#include <QtGui/QtGui>
#pragma warning( pop )

using namespace Loom::OgreApp;
using Loom::Core::cDispatcherHub;
using Loom::OgreApp::cOgreResponderOnRender;

/************************************************************************/
cQScene::cQScene( QWidget *iParent )
: cQOgre( iParent ), mZoom( 200 ), mCameraAngle( 0, 0 ), mCameraTarget( 0, 0, 0 )
, mCameraAngularVelocity( 0, 0 ), mCameraVelocity( 0, 0, 0 )
/************************************************************************/
{
	for ( int i=0; i<KEY_NUM; i++ )
	{
		mKeyStates[i] = false;
	}

	setFocusPolicy( Qt::ClickFocus );
}

/**********************************************************************/
bool cQScene::InitResources( void )
/**********************************************************************/
{
#ifdef Q_WS_MAC
	Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	Ogre::String vPath = "resources/";
#endif
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "materials/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "fonts/", "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath, "FileSystem" );
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 

	return true;
}

/**********************************************************************/
bool cQScene::InitPost( void )
/**********************************************************************/
{
	// Setup camera
	mCamera->setPosition( Ogre::Vector3( 0, 0, -200 ) );
	mCamera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
	mCamera->setNearClipDistance( 1 );
	mCamera->setFarClipDistance( 1000 );

	// Set some fancy clear color
	Ogre::ColourValue vBGColor; vBGColor.setAsABGR( 0xccaa8844 );
	mViewport->setBackgroundColour( vBGColor );
//	mScene->setFog( Ogre::FOG_LINEAR, vBGColor, 0.001f, 0, 1 );

	mLastUpdate = GetTickCount();

	return true;
}

/**********************************************************************/
void cQScene::RenderFrame( void )
/**********************************************************************/
{
	cDispatcherHub::Get().Dispatch( cOgreResponderOnRender::Get().GetEventName(), NULL );
	
//    ASSERTTXT( mWindow, "Main window is invalid" );
//    ASSERTTXT( mCamera, "Main camera is invalid" );

	UpdateCamera();

    update();
}

/************************************************************************/
void Loom::OgreApp::cQScene::mousePressEvent( QMouseEvent *iEvent )
/************************************************************************/
{
	mLastMousePos = iEvent->pos();
}

/************************************************************************/
void Loom::OgreApp::cQScene::mouseReleaseEvent( QMouseEvent *iEvent )
/************************************************************************/
{
}

/************************************************************************/
void Loom::OgreApp::cQScene::mouseMoveEvent( QMouseEvent *iEvent )
/************************************************************************/
{
	QPointF vDelta = iEvent->pos() - mLastMousePos;
	if ( vDelta.manhattanLength() < 2 ) return;
	vDelta *= -0.001f;
	mLastMousePos = iEvent->pos();

	Rotate( vDelta.x(), vDelta.y() );
//	Move( vDelta.x(), vDelta.y() );
}

/************************************************************************/
void Loom::OgreApp::cQScene::wheelEvent( QWheelEvent *iEvent )
/************************************************************************/
{
	float vDelta = iEvent->delta(); // - mLastMousePos;
	vDelta *= -0.0004f;
	Zoom( vDelta );
}

/**********************************************************************/
void Loom::OgreApp::cQScene::UpdateCamera( void )
/**********************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	mCameraVelocity *= powf( 0.1f, vEllapsed  );
	mCameraAngularVelocity *= powf( 0.1f, vEllapsed );
	mCameraAngle += mCameraAngularVelocity * vEllapsed * 10;
	if ( mCameraAngle.y < 0 ) mCameraAngle.y = 0;
	if ( mCameraAngle.y > M_PI_4 ) mCameraAngle.y = M_PI_4;
	mCameraAngle.x = fmodf( mCameraAngle.x, M_PI * 2 );

	Ogre::Vector3 vDir( 0, 0, 1 );
	Ogre::Matrix3 vMatrix;
	vMatrix.FromAxisAngle( Ogre::Vector3( 1, 0, 0 ), -Ogre::Radian( mCameraAngle.y ) );
	vDir = vDir * vMatrix;
	vMatrix.FromAxisAngle( Ogre::Vector3( 0, 1, 0 ), -Ogre::Radian( mCameraAngle.x ) );
	vDir = vDir * vMatrix;

	Ogre::Vector3 vCenter = mCameraTarget - vDir * mZoom;
	mCamera->setPosition( vCenter );    
	mCamera->lookAt( mCameraTarget );

	if ( mKeyStates[KEY_LEFT] )
	{
		mCameraTarget -= mCamera->getRight() * vEllapsed * 42;
	}
	if ( mKeyStates[KEY_RIGHT] )
	{
		mCameraTarget += mCamera->getRight() * vEllapsed * 42;
	}
	if ( mKeyStates[KEY_UP] )
	{
		mCameraTarget += mCamera->getUp() * vEllapsed * 42;
	}
	if ( mKeyStates[KEY_DOWN] )
	{
		mCameraTarget -= mCamera->getUp() * vEllapsed * 42;
	}
}

/**********************************************************************/
void Loom::OgreApp::cQScene::keyPressEvent( QKeyEvent *iEvent )
/**********************************************************************/
{
	int vKey = iEvent->key();

	switch ( vKey )
	{
	case Qt::Key_Left:
		mKeyStates[KEY_LEFT] = true;
		break;
	case Qt::Key_Right:
		mKeyStates[KEY_RIGHT] = true;
		break;
	case Qt::Key_Up:
		mKeyStates[KEY_UP] = true;
		break;
	case Qt::Key_Down:
		mKeyStates[KEY_DOWN] = true;
		break;
	}

	cQOgre::keyPressEvent( iEvent );
}

/**********************************************************************/
void Loom::OgreApp::cQScene::keyReleaseEvent( QKeyEvent *iEvent )
/**********************************************************************/
{
	int vKey = iEvent->key();

	switch ( vKey )
	{
	case Qt::Key_Left:
		mKeyStates[KEY_LEFT] = false;
		break;
	case Qt::Key_Right:
		mKeyStates[KEY_RIGHT] = false;
		break;
	case Qt::Key_Up:
		mKeyStates[KEY_UP] = false;
		break;
	case Qt::Key_Down:
		mKeyStates[KEY_DOWN] = false;
		break;
	}

	cQOgre::keyReleaseEvent( iEvent );
}

/**********************************************************************/
void cQScene::Rotate( const float iDX, const float iDY )
/**********************************************************************/
{
	mCameraAngularVelocity.x += iDX;
	mCameraAngularVelocity.y += iDY;
}

/**********************************************************************/
void cQScene::Move( const float iDX, const float iDY )
/**********************************************************************/
{
	float vZoom = ( mCamera->getPosition() - Ogre::Vector3::ZERO ).length();
	mCameraVelocity.x += iDX * vZoom * 0.1f;
	mCameraVelocity.y -= iDY * vZoom * 0.1f;
}

/**********************************************************************/
void cQScene::Zoom( const float iDelta )
/**********************************************************************/
{
	/*
	float vZoom = ( mCamera->getPosition() - Ogre::Vector3::ZERO ).length();
	vZoom *= powf( 2, iDelta );
	mCamera->setPosition( Ogre::Vector3::ZERO - mCamera->getDirection() * vZoom );
	mCamera->lookAt( Ogre::Vector3::ZERO );                
	*/
	mZoom *= powf( 2, iDelta );
}
