#include <MoMa/Entity/cCreature.h>
#include <MoMa/Entity/cPCreature.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Loom::MoMa;
using Loom::OgreApp::cOgreResponderOnRender;

/************************************************************************/
cCreature::cCreature( cPCreature *iProto, const Ogre::Vector3 &iPosition )
: mPrototype( iProto )
, mEyeSpeed( 8.0f )
/************************************************************************/
{
	int vNumParts = mPrototype->GetNumParts();

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
	}
	for ( int i=1; i<vNumParts/14; i++ )
	{	// Head part
		float vAlpha = (float)i / (float)( vNumParts / 14 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + i * 1.3f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( vAlpha * 1.0f + 19.0f ) * 3, 1.5f );
		mBillboards.Add( vBillboard );
	}
	for ( int i=vNumParts/14; i<=vNumParts/10; i++ )
	{	// Head part
		float vAlpha = (float)i / (float)( vNumParts / 10 ) * M_PI;
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + i * 1.3f ), 0 );
		Ogre::Billboard *vBillboard = vBodySet->createBillboard( vPosition );
		vBillboard->setDimensions( ( sinf( vAlpha ) * 10.0f + 10.0f ) * 3, 1.5f );
		mBillboards.Add( vBillboard );
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
	}

	// Eye
	Ogre::BillboardSet *vEyeSet = mPrototype->GetBillboardSet( cPCreature::PART_EYE );
	{
		Ogre::Vector3 vPosition = iPosition + Ogre::Vector3( 0, -( -5 + 7 ), -0.1f );
		Ogre::Billboard *vBillboard = vEyeSet->createBillboard( vPosition );
		vBillboard->setDimensions( 15, 15 );
//		vBillboard->setDimensions( 55, 55 );
//		vBillboard->setColour( Ogre::ColourValue( 1, 1, Ogre::Math::RangeRandom( 0.5f, 1 ), Ogre::Math::RangeRandom( 0, 1 ) ) );
		vBillboard->setColour( Ogre::ColourValue( 0.5f, 0.5f, 0, 0 ) );
		mEyeTarget = mEyeOffset = Ogre::Vector3( 0, 0, 0 );
		mEyeBillboard = vBillboard;
		mDistortionStrength = mDistortionTarget = 0;
	}

	mLastUpdate = GetTickCount();

	cOgreResponderOnRender::Get().AddListener( *this );
}

/************************************************************************/
void Loom::MoMa::cCreature::RotateEye( float iYaw, float iPitch )
/************************************************************************/
{
	mEyeOffset = mEyeTarget;
	mEyeTarget = Ogre::Vector3( 0, 0, -1 );
	Ogre::Matrix3 vRotation;
	vRotation.FromEulerAnglesXYZ( Ogre::Radian( iPitch ), Ogre::Radian( iYaw ), Ogre::Radian( 0 ) );
	mEyeTarget = vRotation * mEyeTarget;
}

/************************************************************************/
void Loom::MoMa::cCreature::SetDistortionStrength( float iStrength )
/************************************************************************/
{
	mDistortionStrength = mDistortionTarget;
	mDistortionTarget = iStrength;
}

/************************************************************************/
void Loom::MoMa::cCreature::OnRender( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	Ogre::Vector3 vDiff = mEyeTarget - mEyeOffset;
	float vAlpha = vEllapsed * mEyeSpeed;
	if ( vAlpha > 1 ) vAlpha = 1;
	mEyeOffset += vDiff * vAlpha;

	float vDistDiff = mDistortionTarget - mDistortionStrength;
	mDistortionStrength += vDistDiff * vAlpha;

	mEyeBillboard->setColour( Ogre::ColourValue( mEyeOffset.x * 0.5f + 0.5f, mEyeOffset.y * 0.5f + 0.5f, mDistortionStrength / 2.0f, 0 ) );
}

