#include <MoMa/Entity/cCircle.h>
#include <MoMa/Entity/cPCircle.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

using namespace Loom::MoMa;
using Loom::Core::cModuleManager;
using Loom::OgreApp::cOgreResponderOnRender;

/************************************************************************/
cCircle::cCircle( cPCircle *iProto )
: mPrototype( iProto ), mInited( false )
/************************************************************************/
{
	cOgreResponderOnRender::Get().AddListener( *this );
}

/************************************************************************/
void cCircle::SetColour( const Ogre::ColourValue &iColour )
/************************************************************************/
{
	mColour = iColour;
	mDirty = true;
}

/************************************************************************/
const Ogre::ColourValue& cCircle::GetColour( )
/************************************************************************/
{
	return mColour;
}


/************************************************************************/
void Loom::MoMa::cCircle::SetSkew( const float iSkew )
/************************************************************************/
{
	mPosition.z = iSkew;
	mDirty = true;
}

/************************************************************************/
void Loom::MoMa::cCircle::SetRotation( const Ogre::Radian &iRotation )
/************************************************************************/
{
	mRotation = iRotation;
	mDirty = true;
}

/************************************************************************/
void Loom::MoMa::cCircle::SetSize( const Ogre::Vector2 &iSize )
/************************************************************************/
{
	mDimensions = iSize;
	mDirty = true;
}

/************************************************************************/
void Loom::MoMa::cCircle::SetPosition( const Ogre::Vector2 &iPosition )
/************************************************************************/
{
	mPosition.x = iPosition.x;
	mPosition.y = iPosition.y;
	mDirty = true;
}

/************************************************************************/
void cCircle::OnRender( void )
/************************************************************************/
{
	if ( !mInited )
	{
		Init();
		mInited = true;
	}

	if ( mDirty )
	{
		mBillboard->setColour( mColour );
		mBillboard->setPosition( mPosition );
		mBillboard->setRotation( mRotation );
		mBillboard->setDimensions( mDimensions.x, mDimensions.y );
		mDirty = false;
	}
}

/************************************************************************/
void cCircle::Init( void )
/************************************************************************/
{
	Ogre::BillboardSet *vBillboardSet = mPrototype->GetBillboardSet();
//	mBillboard = vBillboardSet->createBillboard( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( 0, 1000 ) );

	mPosition = Ogre::Vector3( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), 0.0f );
	mDimensions = Ogre::Vector2( 16, 16 );
	mColour = Ogre::ColourValue( 141.0f / 255.0f, 248.0f / 255.0f, 170.0f / 255.0f, 0.5f );
	mRotation = Ogre::Radian( 0 );
	mDirty = true;

	mBillboard = vBillboardSet->createBillboard( mPosition );
	mBillboard->setDimensions( 16, 16 );
	mBillboard->setColour( Ogre::ColourValue( 141.0f / 255.0f, 248.0f / 255.0f, 170.0f / 255.0f, 0.5f ) );
	mBillboards.Add( mBillboard );

	/*
	Ogre::MaterialPtr vMaterial = Ogre::MaterialManager::getSingleton().getByName( "LineMaterial" );
	Ogre::Technique *vTechnique = vMaterial->getTechnique(0);
	Ogre::Pass *vPass = vTechnique->getPass(0);
	Ogre::GpuProgramParametersSharedPtr vParams = vPass->getVertexProgramParameters();
	if ( vParams->_findNamedConstantDefinition( "mActNode" ) )
	{
		vParams->setNamedConstant( "mActNode", (float)iIndex );
	} 
	*/
}
