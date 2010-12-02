#include <MoMa/Entity/cPCreature.h>
#include <typeinfo>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreSceneManager.h>
#include <MoMa/Entity/cCreature.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>

using namespace Loom::MoMa;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;

/************************************************************************/
cPCreature::cPCreature()
: mNumParts( 100 )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	for ( size_t i=0; i<PART_MAX; i++ )
	{
		char Temp[ 256 ];
		sprintf( Temp, "MoMa.Creature.%d", i );
		Ogre::BillboardSet *vBillboardSet = vScene->createBillboardSet( Temp );
		Ogre::BillboardSet *vSet = vBillboardSet;
		Ogre::String vFosFasz = Temp;
		vSet->setMaterialName( vFosFasz );
		vBillboardSet->setDefaultDimensions( 4, 1 );
		vBillboardSet->setPointRenderingEnabled( false );
		vBillboardSet->setAutoextend( true );
		vBillboardSet->setAutoUpdate( true );
		vBillboardSet->setVisible( true );
		Ogre::SceneNode *vNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, -10 ) );
		vNode->attachObject( vBillboardSet );

		mBillboardSet[i] = vBillboardSet;
	}
}

/************************************************************************/
cPCreature::~cPCreature()
/************************************************************************/
{
	for ( size_t i=0; i<PART_MAX; i++ )
	{
		delete mBillboardSet[i];
	}
}

/************************************************************************/
cCreature *cPCreature::CreateInstance( const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	cCreature *vCreature = new cCreature( this, iPosition );

	return vCreature;
}
