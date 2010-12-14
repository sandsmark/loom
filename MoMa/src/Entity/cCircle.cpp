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
	Ogre::Billboard *vBillboard = vBillboardSet->createBillboard( Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( -10, 10 ), Ogre::Math::RangeRandom( 0, 1000 ) );
	float vSize = Ogre::Math::RangeRandom( 20, 70 );
	vBillboard->setDimensions( vSize, vSize );
	vBillboard->setColour( Ogre::ColourValue( 0.5, 0.5f, 0.5f, 1.0f ) );
	mBillboards.Add( vBillboard );

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
