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

	for ( size_t i=0; i<2; i++ )
	{
		char Temp[ 256 ];
		sprintf( Temp, "MoMa.Waveform.%d", i );
		Ogre::BillboardSet *vBillboardSet = vScene->createBillboardSet( Temp );
		vBillboardSet->setMaterialName( Temp );
		vBillboardSet->setDefaultDimensions( 4, 1 );
		vBillboardSet->setPointRenderingEnabled( false );
		vBillboardSet->setAutoextend( true );
		vBillboardSet->setAutoUpdate( true );
		vBillboardSet->setVisible( true );
		Ogre::SceneNode *vNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( -100 + (int)i * 100, 50 + (int)20 * i, i * 100 ) );
		vNode->attachObject( vBillboardSet );
		mBillboardSet[i] = vBillboardSet;
	}

	mHeights.Set( new float[ mNumParts ] );
	mTargetHeights.Set( new float[ mNumParts ] );

	for ( size_t b=0; b<2; b++ )
	{
		for ( size_t i=0; i<mNumParts; i++ )
		{
			Ogre::Billboard *vBillboard = mBillboardSet[b]->createBillboard( (int)i, 0, 0 );
			float vHeight = ( sinf( i * 0.14f ) * 5.0f + 5.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) ) * 2.0f;
			vBillboard->setDimensions( 1, vHeight );
			mHeights[i] = vHeight;
			mTargetHeights[i] = Ogre::Math::RangeRandom( 0.5f, 15.0f );
		}
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

	float vMinX = FLT_MAX;
	for ( size_t i=mNumParts; i-->0; )
	{
		Ogre::Billboard *vBillboard = mBillboardSet[0]->getBillboard(i);
		Ogre::Vector3 vPos = vBillboard->getPosition();
		vPos.x -= -1.0f * vEllapsed;
		vBillboard->setPosition( vPos );
		if ( vPos.x < vMinX ) vMinX = vPos.x;
	}

	for ( size_t i=mNumParts; i-->0; )
	{
		Ogre::Billboard *vBillboard = mBillboardSet[0]->getBillboard(i);
		Ogre::Vector3 vPos = vBillboard->getPosition();
		if ( vPos.x > mNumParts )
		{
			vPos.x = vMinX - 1;
			vMinX = vPos.x;
			float vHeight = ( sinf( mNumParts + vTime * 0.001f * 0.14f ) * 5.0f + 5.0f + Ogre::Math::RangeRandom( 0.5f, 5 ) ) * 2.0f;
			vBillboard->setDimensions( 1, vHeight );
			mHeights[i] = vHeight;
		}
		if ( i >= vSkip )
		{
			mTargetHeights[i] = mTargetHeights[i-vSkip];
		}
		else
		{
			mTargetHeights[i] = ( mMockInput * 15.0f + Ogre::Math::RangeRandom( -4.0f, 4.0f ) * mMockInput ) * 5; //3;
		}
		float vHeight = vBillboard->getOwnHeight();
		vHeight = mHeights[i];
		float vDiff = mTargetHeights[i] - vHeight;
		float vInertia = mInertia;
		if ( vInertia > 1 ) vInertia = 1;
		vInertia *= 0.998f;
		float vAlpha = 1 - powf( 1 - vInertia, vEllapsed );
		vHeight = vHeight + vDiff * vAlpha;
		for ( size_t b=0; b<2; b++ )
		{
			Ogre::Billboard *vBillboard = mBillboardSet[b]->getBillboard(i);
			vBillboard->setDimensions( 1, vHeight );
			vBillboard->setPosition( vPos );
		}
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
