#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cPCreature.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Ogre/OgreSceneManager.h>

#undef M_PI
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::MoMa;
using Loom::OgreApp::cOgreResponderOnRender;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;

/************************************************************************/
cCreature::cCreature( cPCreature *iProto, const Ogre::Vector3 &iPosition )
: mPrototype( iProto )
, mEyeSpeed( 8.0f ), mTransientSize( 10 ), mTransientStrength( 0.5f )
, mHeadSpeed( 2.0f )
/************************************************************************/
{
	int vNumParts = mPrototype->GetNumParts();

	mPosition = iPosition;

	// Body
	Ogre::BillboardSet *vBodySet = mPrototype->GetBillboardSet( cPCreature::PART_BODY );

	// Head part
	for ( int i=0; i<1; i++ )
	{	// Head part
		float vAlpha = (float)i / (float)( vNumParts / 2 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + i * 1.3f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( vAlpha * 8.0f + 12.0f ) * 3, 1.5f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}
	for ( int i=1; i<vNumParts/14; i++ )
	{	// Head part
		float vAlpha = (float)i / (float)( vNumParts / 14 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + i * 1.3f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( vAlpha * 1.0f + 19.0f ) * 3, 1.5f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}
	for ( int i=vNumParts/14; i<=vNumParts/10; i++ )
	{	// Head part
		float vAlpha = (float)i / (float)( vNumParts / 10 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + i * 1.3f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( sinf( vAlpha ) * 10.0f + 10.0f ) * 3, 1.5f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}

	// Upper body part
	for ( int i=vNumParts/10+1; i<3*vNumParts/10; i++ )
	{	// Upper body part
		int vIndex = i - vNumParts/10 - 1;
		float vStart = -5 + vNumParts * 1.3f / 10;
		float vAlpha = (float)(vIndex+1) / (float)( 3 * vNumParts / 10 - vNumParts / 10 - 3 ) * M_PI * 2 - M_PI_2;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( vStart + vIndex * 0.7f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( sinf( vAlpha ) * 2.0f + 2.0f + 6.0f ) * 3, 0.8f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}

	// Mid body part
	for ( int i=3*vNumParts/10+1; i<5*vNumParts/10; i++ )
	{	// Mid body part
		int vIndex = i - 3*vNumParts/10 - 1;
		float vStart = -5 + vNumParts * 1.3f / 10;
		vStart += ( 3*vNumParts/10 - vNumParts/10 - 1 ) * 0.7f;
		float vAlpha  = (float)(vIndex) / (float)( 5* vNumParts / 10 - 3 * vNumParts / 10 + 1 ) * M_PI * 4 - M_PI_2;
		float vAlpha2 = (float)(vIndex) / (float)( 5* vNumParts / 10 - 3 * vNumParts / 10 + 1 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( vStart + vIndex * 0.7f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( sinf( vAlpha ) * 2.0f + sinf( vAlpha2 ) * 8.0f + 3.5f + 5.0f ) * 3, 0.8f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}

	// Lower body part
	for ( int i=5*vNumParts/10+1; i<6*vNumParts/10; i++ )
	{	// Lower body part
		int vIndex = i - 5*vNumParts/10 - 1;
		float vStart = -5 + vNumParts * 1.3f / 10;
		vStart += ( 3*vNumParts/10 - vNumParts/10 - 1 ) * 0.7f;
		vStart += ( 5*vNumParts/10 - 3*vNumParts/10 - 1 ) * 0.7f;
		float vAlpha  = (float)(vIndex) / (float)( 6* vNumParts / 10 - 5 * vNumParts / 10 + 1 ) * M_PI_2 + M_PI_2;
		float vAlpha2 = (float)(vIndex+3) / (float)( 6* vNumParts / 10 - 5 * vNumParts / 10 + 1 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( vStart + vIndex * 0.6f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( sinf( vAlpha ) * 6.5f * sinf( vAlpha2 ) ) * 3, 0.7f );
		mBillboards.Add( vBillboard );
		mBillboardSizes.Add( Ogre::Vector2( vBillboard->getOwnWidth(), vBillboard->getOwnHeight() ) );
	}

	mHeadDistortion.Init( 0 );
	mHeadOffset.Init( Ogre::Vector2( 0, 0 ) );

	// Eye
	Ogre::BillboardSet *vEyeSet = mPrototype->GetBillboardSet( cPCreature::PART_EYE );
	{
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + 7 ), -0.1f );
		Ogre::Billboard *vBillboard = vEyeSet->createBillboard( vPosition );
		vBillboard->setDimensions( 15, 15 );
//		vBillboard->setDimensions( 55, 55 );
//		vBillboard->setColour( Ogre::ColourValue( 1, 1, Ogre::Math::RangeRandom( 0.5f, 1 ), Ogre::Math::RangeRandom( 0, 1 ) ) );
		vBillboard->setColour( Ogre::ColourValue( 0.5f, 0.5f, 0, 0 ) );
		mEyeOffset.Init( Ogre::Vector3( 0, 0, 0 ) );
		mEyeDistortion.Init( 0 );
		mEyeSize.Init( 1 );
		mBlink.Init( 0 );
		mEyeBillboard = vBillboard;
	}

	mLastUpdate = GetTickCount();

	cOgreResponderOnRender::Get().AddListener( *this );
}

/************************************************************************/
void Loom::MoMa::cCreature::SetEyeRotation( const float iYaw, const float iPitch )
/************************************************************************/
{
	Ogre::Vector3 vTarget( 0, 0, -1 );
	Ogre::Matrix3 vRotation;
	vRotation.FromEulerAnglesXYZ( Ogre::Radian( iPitch ), Ogre::Radian( iYaw ), Ogre::Radian( 0 ) );
	mEyeOffset.SetTarget( vRotation * vTarget );	
}

/************************************************************************/
void Loom::MoMa::cCreature::SetEyeTarget( Ogre::Vector3 vTarget)
/************************************************************************/
{
	mEyeOffset.SetTarget( vTarget );	
}

/************************************************************************/
const Ogre::Vector3 Loom::MoMa::cCreature::GetEyeTarget(  )
/************************************************************************/
{
	return mEyeOffset.GetTarget();	
}

/************************************************************************/
void Loom::MoMa::cCreature::SetEyeDistortion( const float iStrength )
/************************************************************************/
{
	mEyeDistortion.SetTarget( iStrength );
}

/************************************************************************/
void Loom::MoMa::cCreature::SetEyeSize( const float iSize )
/************************************************************************/
{
	mEyeSize.SetTarget( iSize );
}

/************************************************************************/
void Loom::MoMa::cCreature::OnRender( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	mEyeOffset.SetSpeed( mEyeSpeed );
	mEyeDistortion.SetSpeed( mEyeSpeed );
	mEyeSize.SetSpeed( mEyeSpeed );

	mEyeOffset.Update( vEllapsed );
	mEyeDistortion.Update( vEllapsed );
	mEyeSize.Update( vEllapsed );
	mBlink.Update( vEllapsed );

	mHeadOffset.Update( vEllapsed );
	mHeadDistortion.Update( vEllapsed );

	float vBlink = 1 - sinf( mBlink.GetValue() * M_PI );
	mEyeBillboard->setColour( Ogre::ColourValue( mEyeOffset.GetValue().x * 0.5f + 0.5f, mEyeOffset.GetValue().y * 0.5f + 0.5f, mEyeDistortion.GetValue() / 2.0f, vBlink ) );
	mEyeBillboard->setDimensions( 19 * mEyeSize.GetValue(), 19 * mEyeSize.GetValue() );

	UpdateTransients( vEllapsed );
}

/************************************************************************/
void cCreature::UpdateTransients( const float iEllapsed )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	Ogre::Vector2 vHeadOffset = mHeadOffset.GetValue();

	Ogre::Camera *vCamera = vScene->getCamera("MainCamera");
	Ogre::Vector4 vProjPos( mPosition );
	vProjPos = vProjPos * vCamera->getViewMatrix();
	vProjPos.x += vHeadOffset.x * 150;
	vProjPos.y += vHeadOffset.y * 40 + 30;
	vProjPos = vProjPos * vCamera->getProjectionMatrix();
	vProjPos.x /= vProjPos.w;
	vProjPos.y /= vProjPos.w;
	vProjPos.z /= vProjPos.w;

	vProjPos.x = vProjPos.x * 0.5f + 0.5f;
	vProjPos.y = vProjPos.y * 0.5f + 0.5f;

	if ( vProjPos.x < 0 ) vProjPos.x = 0; if ( vProjPos.x > 1 ) vProjPos.x = 1;
	if ( vProjPos.y < 0 ) vProjPos.y = 0; if ( vProjPos.y > 1 ) vProjPos.y = 1;

	for ( size_t i=0; i<mBillboards.GetSize(); i++ )
	{
		mBillboards[i]->setDimensions( mBillboardSizes[i].x, mBillboardSizes[i].y * 5 );
		mBillboards[i]->setColour( Ogre::ColourValue( vProjPos.x, vProjPos.y, mHeadDistortion.GetValue(), 0 ) );
	}

	for ( size_t t=0; t<mTransients.GetSize(); t++ )
	{
		cMotion<float> *vTransientMotion = mTransients[t];

		vTransientMotion->Update( iEllapsed );
		if ( fabsf( vTransientMotion->GetTarget() - vTransientMotion->GetValue() ) < 0.01f )
		{
			mTransients.erase( mTransients.begin() + t );
			t--;
			delete vTransientMotion;
			continue;
		}
		float vTransient = vTransientMotion->GetValue();

		for ( size_t i=0; i<mBillboards.GetSize(); i++ )
		{
			float vStrength = 1 - ( fabsf( (float)i - vTransient ) / mTransientSize );
			if ( vStrength < 0 ) vStrength = 0;
			float vWidth = mBillboards[i]->getOwnWidth();
			float vNewWidth = mBillboardSizes[i].x * ( 1 + vStrength * mTransientStrength );
			if ( t == 0 || vNewWidth > vWidth )
			{
//				mBillboards[i]->setDimensions( mBillboardSizes[i].x * ( 1 + vStrength * mTransientStrength ), mBillboardSizes[i].y * ( 1 + vStrength * mTransientStrength ) );
				mBillboards[i]->setDimensions( mBillboardSizes[i].x * ( 1 + vStrength * mTransientStrength ), mBillboardSizes[i].y * ( 1 + vStrength * mTransientStrength ) * 2 );
			}
		}
	}
}

/************************************************************************/
void Loom::MoMa::cCreature::StartTransient( const float iSpeed )
/************************************************************************/
{
	cMotion<float> *vTransient = new cMotion<float>();
	vTransient->Init     ( ( iSpeed < 0 ) ? -mTransientSize : mBillboards.GetSize() + mTransientSize );
	vTransient->SetTarget( ( iSpeed > 0 ) ? -mTransientSize : mBillboards.GetSize() + mTransientSize );
	vTransient->SetSpeed ( fabsf( iSpeed ) );

	mTransients.Add( vTransient );
}

/************************************************************************/
void Loom::MoMa::cCreature::Blink( const float iSpeed )
/************************************************************************/
{
	mBlink.Init( 0 );
	mBlink.SetTarget( 1 );
	mBlink.SetSpeed( iSpeed );
}

/************************************************************************/
void Loom::MoMa::cCreature::SetHeadDistortionPosition( const Ogre::Vector2 &iPosition )
/************************************************************************/
{
	mHeadOffset.SetTarget( iPosition );
}

/************************************************************************/
void Loom::MoMa::cCreature::SetHeadDistortionStrength( const float iStrength )
/************************************************************************/
{
	mHeadDistortion.SetTarget( iStrength );
}
