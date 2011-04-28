#pragma warning( disable: 4995 )	// #pragma deprecated

#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Core/Event/cDispatcherHub.h>
#include <strsafe.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>
#include <Core/Debug/Logger/cLogger.h>

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
using Loom::Core::cLogger;

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

	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

//	UpdateCamera( vEllapsed );
	UpdateAnims( vEllapsed );

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
void Loom::OgreApp::cQScene::UpdateCamera( const float iEllapsed )
/**********************************************************************/
{
	mCameraVelocity *= powf( 0.1f, iEllapsed  );
	mCameraAngularVelocity *= powf( 0.1f, iEllapsed );
	mCameraAngle += mCameraAngularVelocity * iEllapsed * 10;
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
		mCameraTarget -= mCamera->getRight() * iEllapsed * 42;
	}
	if ( mKeyStates[KEY_RIGHT] )
	{
		mCameraTarget += mCamera->getRight() * iEllapsed * 42;
	}
	if ( mKeyStates[KEY_UP] )
	{
		mCameraTarget += mCamera->getUp() * iEllapsed * 42;
	}
	if ( mKeyStates[KEY_DOWN] )
	{
		mCameraTarget -= mCamera->getUp() * iEllapsed * 42;
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
void Loom::OgreApp::cQScene::UpdateAnims( const float iEllapsed )
/**********************************************************************/
{
	for ( size_t i=0; i<mAnims.size(); i++ )
	{
		sAnim *vAnim = mAnims[i];
		cMotion<Ogre::Vector3> *vMotion = vAnim->Motion;

		vMotion->Update( iEllapsed );
		if ( ( vMotion->GetTarget() - vMotion->GetValue() ).squaredLength() < 0.01f )
		{	// Anim done
			mAnims.erase( mAnims.begin() + i );
			i--;
			delete vAnim;
			continue;
		}

		Ogre::Vector3 vPosition = vMotion->GetValue();

		if ( vAnim->Entity ) vAnim->Entity->getParentNode()->setPosition( vPosition );
		if ( vAnim->Camera ) vAnim->Camera->setPosition( vPosition );
	}
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

/************************************************************************/
void cQScene::OnMoveTo( const Ogre::String &iName, const Ogre::Vector3 &iPosition, float iSpeed )
/************************************************************************/
{
	if ( mScene->hasCamera( iName ) )
	{
		sAnim *vAnim = new sAnim();
		vAnim->Camera = mScene->getCamera( iName );
		vAnim->Motion = new cMotion<Ogre::Vector3>();
		vAnim->Motion->Init( vAnim->Camera->getPosition() );
		vAnim->Motion->SetSpeed( iSpeed );
		vAnim->Motion->SetTarget( iPosition );
		mAnims.Add( vAnim );
		return;
	}

	Ogre::Entity *vEntity = mScene->getEntity( iName );

	if ( !vEntity )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, _T("Unknown entity: %S"), iName );	// TODO: Use %s if not in unicode
		cLogger::Get().Log( cLogger::LOG_WARNING, _T("Global"), vTemp );
		return;
	}

	sAnim *vAnim = new sAnim();
	vAnim->Entity = vEntity;
	vAnim->Motion = new cMotion<Ogre::Vector3>();
	vAnim->Motion->Init( vEntity->getParentNode()->getPosition() );
	vAnim->Motion->SetSpeed( iSpeed );
	vAnim->Motion->SetTarget( iPosition );
	mAnims.Add( vAnim );
}

/************************************************************************/
void Loom::OgreApp::cQScene::OnStopMove( const Ogre::String &iName )
/************************************************************************/
{
	for ( size_t i=0; i<mAnims.size(); i++ )
	{
		sAnim *vAnim = mAnims[i];
		if ( vAnim->Camera && vAnim->Camera->getName() == iName )
		{
			mAnims.erase( mAnims.begin() + i );
			i--;
			break;
		}
		if ( vAnim->Entity && vAnim->Entity->getName() == iName )
		{
			mAnims.erase( mAnims.begin() + i );
			i--;
			break;
		}
	}
}
