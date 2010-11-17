#include <MoMa/Entity/cWaveform.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Core/Module/cModuleManager.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreBillboard.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

using namespace Loom::MoMa;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;
using Loom::OgreApp::cOgreResponderOnRender;

/************************************************************************/
cWaveform::cWaveform()
: mNumParts( 128 ), mInertia( 0.1f )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	mBillboardSet = vScene->createBillboardSet( "MoMa.Waveform" );
	mBillboardSet->setMaterialName( "MoMa.Waveform" );
	mBillboardSet->setDefaultDimensions( 4, 1 );
	mBillboardSet->setPointRenderingEnabled( false );
	mBillboardSet->setAutoextend( true );
	mBillboardSet->setAutoUpdate( true );
	mBillboardSet->setVisible( true );
	Ogre::SceneNode *vNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 50, 0 ) );
	vNode->attachObject( mBillboardSet );

	mHeights.Set( new float[ mNumParts ] );
	mTargetHeights.Set( new float[ mNumParts ] );

	for ( size_t i=0; i<mNumParts; i++ )
	{
		Ogre::Billboard *vBillboard = mBillboardSet->createBillboard( (int)i, 0, 0 );
		float vHeight = ( sinf( i * 0.14f ) * 5.0f + 5.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) ) * 2.0f;
		vBillboard->setDimensions( 1, vHeight );
		mHeights[i] = vHeight;
		mTargetHeights[i] = Ogre::Math::RangeRandom( 0.5f, 15.0f );
	}

	cOgreResponderOnRender::Get().AddListener( *this );
	mFirstPart = 0;
	mMockInput = 0;

	mLastUpdate = GetTickCount();
}

/************************************************************************/
void cWaveform::Update( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	size_t vSkip = (size_t)( vEllapsed * 500 );

	for ( size_t i=mNumParts; i-->0; )
	{
		Ogre::Billboard *vBillboard = mBillboardSet->getBillboard(i);
		Ogre::Vector3 vPos = vBillboard->getPosition();
		vPos.x -= -1.0f * vEllapsed;
		if ( vPos.x > mNumParts )
		{
			vPos.x = 0;
			float vHeight = ( sinf( mNumParts + vTime * 0.001f * 0.14f ) * 5.0f + 5.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) ) * 2.0f;
			vBillboard->setDimensions( 1, vHeight );
			mHeights[i] = vHeight;
//			vBillboard->setDimensions( 1, 10.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) * 3 );
		}
		if ( i >= vSkip )
		{
			mTargetHeights[i] = mTargetHeights[i-vSkip];
		}
		else
		{
//			mTargetHeights[i] = Ogre::Math::RangeRandom( 0.5f, 15.0f ) * 3;
			mTargetHeights[i] = ( mMockInput * 15.0f + Ogre::Math::RangeRandom( -4.0f, 4.0f ) * mMockInput ) * 5; //3;
		}
		float vHeight = vBillboard->getOwnHeight();
		/*
		if ( fabsf( vHeight - mTargetHeights[i] ) < mInertia )
		{
			vHeight = mTargetHeights[i];
		}
		else
		{
			vHeight += ( mTargetHeights[i] < vHeight ) ? -mInertia : mInertia;
		}
		*/
		vHeight = mHeights[i];
		float vDiff = mTargetHeights[i] - vHeight;
		float vInertia = mInertia;
		if ( vInertia > 1 ) vInertia = 1;
		vInertia *= 0.998f;
		float vAlpha = 1 - powf( 1 - vInertia, vEllapsed );
		vHeight = vHeight + vDiff * vAlpha;
		vBillboard->setDimensions( 1, vHeight );
		vBillboard->setPosition( vPos );
	}

	mMockInput *= powf( 0.001f, vEllapsed );
	mInertia -= vEllapsed * 10;
	if ( mInertia < 0 ) mInertia = 0;
//	if ( mMockInput > 0.1f && mInertia < 0.1f && Ogre::Math::RangeRandom( 0, 1 ) < 0.1f )
	if ( mMockInput > 0.1f )
	{
//		mInertia = 3.0f;
		mInertia = 1.0f;
	}
	if ( Ogre::Math::RangeRandom( 0, 1 ) < 0.05f )
	{
		mMockInput = Ogre::Math::RangeRandom( 0.7f, 1 );
	}
}

/************************************************************************/
void cWaveform::OnRender( void )
/************************************************************************/
{
	Update();
}
