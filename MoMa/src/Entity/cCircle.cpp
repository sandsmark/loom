#include <MoMa/Entity/cCircle.h>
#include <MoMa/Entity/cPCircle.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>

using namespace Loom::MoMa;

/************************************************************************/
cCircle::cCircle( cPCircle *iProto )
: mPrototype( iProto )
/************************************************************************/
{
	Ogre::BillboardSet *vBillboardSet = mPrototype->GetBillboardSet();
//	mBillboard = vBillboardSet->createBillboard( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( 0, 1000 ) );
	mBillboard = vBillboardSet->createBillboard( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), 0.0f );
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

/************************************************************************/
void cCircle::SetColour( const Ogre::ColourValue &iColour )
/************************************************************************/
{
	mBillboard->setColour( iColour );
}

/************************************************************************/
void Loom::MoMa::cCircle::SetSkew( const float iSkew )
/************************************************************************/
{
	Ogre::Vector3 vPos = mBillboard->getPosition();
	vPos.z = iSkew;
	mBillboard->setPosition( vPos );
}

/************************************************************************/
void Loom::MoMa::cCircle::SetRotation( const Ogre::Radian &iRotation )
/************************************************************************/
{
	mBillboard->setRotation( iRotation );
}

/************************************************************************/
void Loom::MoMa::cCircle::SetSize( const Ogre::Vector2 &iSize )
/************************************************************************/
{
	mBillboard->setDimensions( iSize.x, iSize.y );
}

/************************************************************************/
void Loom::MoMa::cCircle::SetPosition( const Ogre::Vector2 &iPosition )
/************************************************************************/
{
	Ogre::Vector3 vPos = mBillboard->getPosition();
	vPos.x = iPosition.x;
	vPos.y = iPosition.y;
	mBillboard->setPosition( vPos );
}
