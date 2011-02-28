/*
 *  cVisualizerHypertree.cpp
 *  Visor
 *
 *  Created by mb on 6/3/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerHypertree.h"
#include <Visor/Hypertree/cVisualizerHypertreeNode.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreMaterialManager.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>
#include <Ogre/OgreMesh.h>
#include <Ogre/OgreMeshManager.h>
#include <Ogre/OgreSubMesh.h>
#include <Ogre/OgreHardwareBufferManager.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreEntity.h>
//#include <openGL/openGL.h>
using namespace Loom::Visor;

/**********************************************************************/
cVisualizerHypertree::cVisualizerHypertree( Ogre::SceneManager *iScene )
: mScene( iScene ), mFocus( NULL ), mSeparation( 2 )
/**********************************************************************/
{
    const int vNumTypes = 7;    // TODO: Iterate on files
    for ( size_t i=0; i<vNumTypes; i++ )
    {
        std::stringstream vName;
        vName.str("");
        vName << i << " Components";
        
        Ogre::MaterialPtr vMaterial = ((Ogre::MaterialPtr)(Ogre::MaterialManager::getSingleton().getByName( "ComponentMaterial", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME )))->clone( vName.str() );
        
        Ogre::BillboardSet *vBillboards = mScene->createBillboardSet( vName.str() );
        vBillboards->setMaterialName( vName.str() );
        vBillboards->setBillboardType( Ogre::BBT_POINT );
        vBillboards->setDefaultDimensions( 1, 1 );
        vBillboards->setPointRenderingEnabled( true );
        vBillboards->setAutoextend( true );
        vBillboards->setAutoUpdate( true );
        vBillboards->setVisible( true );
    
        vName.str("");
        vName << "NodeTypes/" << ( i + 1 ) << ".png";
        std::string vCucc = vName.str();
        vMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName( vName.str() );
        
//        mScene->getRootSceneNode()->attachObject( vBillboards );

        mBillboards.push_back( vBillboards );
    }
    
    CreateLines();
    CreateTexturedLines();
    
    mArrowHeads = mScene->createBillboardSet( "ArrowHeads" );
    mArrowHeads->setMaterialName( "ArrowheadMaterial" );
    mArrowHeads->setBillboardType( Ogre::BBT_POINT );
    mArrowHeads->setDefaultDimensions( 1, 1 );
    mArrowHeads->setPointRenderingEnabled( false );
    mArrowHeads->setAutoextend( true );
    mArrowHeads->setAutoUpdate( true );
    mArrowHeads->setVisible( true );

    mArrowFoots = mScene->createBillboardSet( "ArrowFoots" );
    mArrowFoots->setMaterialName( "ArrowfootMaterial" );
    mArrowFoots->setBillboardType( Ogre::BBT_POINT );
    mArrowFoots->setDefaultDimensions( 1, 1 );
    mArrowFoots->setPointRenderingEnabled( false );
    mArrowFoots->setAutoextend( true );
    mArrowFoots->setAutoUpdate( true );
    mArrowFoots->setVisible( true );
    
    mSelections = mScene->createBillboardSet( "Selections" );
    mSelections->setMaterialName( "SelectionMaterial" );
    mSelections->setBillboardType( Ogre::BBT_POINT );
    mSelections->setDefaultDimensions( 1, 1 );
    mSelections->setPointRenderingEnabled( false );
    mSelections->setAutoextend( true );
    mSelections->setAutoUpdate( true );
    mSelections->setVisible( true );
    
    mScene->getRootSceneNode()->attachObject( mArrowHeads );
    mScene->getRootSceneNode()->attachObject( mArrowFoots );
}

/**********************************************************************/
void cVisualizerHypertree::CreateLines( void )
/**********************************************************************/
{
    mLines = Ogre::MeshManager::getSingleton().createManual( "Lines", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    
    // Create mesh for lines
    Ogre::SubMesh *vSub = mLines->createSubMesh();
    vSub->operationType = Ogre::RenderOperation::OT_LINE_LIST;
    vSub->setMaterialName( "LineMaterial" );
    vSub->useSharedVertices = false;
    vSub->vertexData = new Ogre::VertexData;
    vSub->indexData  = new Ogre::IndexData;
    vSub->vertexData->vertexDeclaration->addElement( 0,  0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    vSub->vertexData->vertexDeclaration->addElement( 0, 12, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE  );
    vSub->vertexData->vertexDeclaration->addElement( 0, 28, Ogre::VET_FLOAT2,  Ogre::VES_BLEND_INDICES );

    SetActNode( 0 );
    
//    const int vMaxNumLines = 30000 * 5;
    const int vMaxNumLines = 30000;
    Ogre::HardwareVertexBufferSharedPtr vVertices = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vSub->vertexData->vertexDeclaration->getVertexSize( 0 ), vMaxNumLines * 2, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
    Ogre::HardwareIndexBufferSharedPtr  vIndices  = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer ( Ogre::HardwareIndexBuffer::IT_32BIT, vMaxNumLines * 2 * sizeof( Ogre::ulong ), Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
    
    vSub->vertexData->vertexBufferBinding->setBinding( 0, vVertices );
    vSub->vertexData->vertexStart = 0;
    vSub->vertexData->vertexCount = 0;
    
    vSub->indexData->indexBuffer = vIndices;
    
    mLines->_setBounds( Ogre::AxisAlignedBox( -10, -10, -10, 10, 10, 10 ) );
    mLines->_setBoundingSphereRadius( 20 );
    
    Ogre::Entity *vLinesEntity = mScene->createEntity( "LinesEntity", "Lines" );
    mScene->getRootSceneNode()->attachObject( vLinesEntity );
}

/**********************************************************************/
void cVisualizerHypertree::CreateTexturedLines( void )
/**********************************************************************/
{
    mTexturedLines = Ogre::MeshManager::getSingleton().createManual( "TexturedLines", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    
    // Create mesh for textured lines
    Ogre::SubMesh *vSub = mTexturedLines->createSubMesh();
    vSub->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    vSub->setMaterialName( "TexturedLineMaterial" );
    vSub->useSharedVertices = false;
    vSub->vertexData = new Ogre::VertexData;
    vSub->indexData  = new Ogre::IndexData;
    vSub->vertexData->vertexDeclaration->addElement( 0,  0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    vSub->vertexData->vertexDeclaration->addElement( 0, 12, Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES, 1 );
    vSub->vertexData->vertexDeclaration->addElement( 0, 24, Ogre::VET_FLOAT1, Ogre::VES_TEXTURE_COORDINATES, 2 );
    vSub->vertexData->vertexDeclaration->addElement( 0, 28, Ogre::VET_FLOAT2,  Ogre::VES_TEXTURE_COORDINATES, 0 );
    
    SetActNode( 0 );
    
    //    const int vMaxNumLines = 30000 * 5;
    const int vMaxNumLines = 30000;
    Ogre::HardwareVertexBufferSharedPtr vVertices = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vSub->vertexData->vertexDeclaration->getVertexSize( 0 ), vMaxNumLines * 2, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
    Ogre::HardwareIndexBufferSharedPtr  vIndices  = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer ( Ogre::HardwareIndexBuffer::IT_32BIT, vMaxNumLines * 2 * sizeof( Ogre::ulong ), Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
    
    vSub->vertexData->vertexBufferBinding->setBinding( 0, vVertices );
    vSub->vertexData->vertexStart = 0;
    vSub->vertexData->vertexCount = 0;
    
    vSub->indexData->indexBuffer = vIndices;
    
    mTexturedLines->_setBounds( Ogre::AxisAlignedBox( -10, -10, -10, 10, 10, 10 ) );
    mTexturedLines->_setBoundingSphereRadius( 20 );
    
    Ogre::Entity *vLinesEntity = mScene->createEntity( "TexturedLinesEntity", "TexturedLines" );
    mScene->getRootSceneNode()->attachObject( vLinesEntity );
}

/**********************************************************************/
void cVisualizerHypertree::OnComponentCreated( IComponent *iComponent )
/**********************************************************************/
{
//    printf( "[cVisualizerHypertree::OnComponentCreated] 0x%08x\n", (unsigned int)iComponent );
    
    cVisualizerHypertreeNode *vNode = new cVisualizerHypertreeNode( this, iComponent );
    vNode->SetId( mNodes.size() );
    mNodes[ iComponent ] = vNode;
    
    if ( !mFocus )
    {
        mFocus = iComponent;
        vNode->SetConnection( Ogre::Vector2( -1, 1 ) );
    }
}

/**********************************************************************/
void cVisualizerHypertree::OnComponentDestroyed( IComponent *iComponent )
/**********************************************************************/
{
    printf( "[cVisualizerHypertree::OnComponentDestroyed] 0x%08x\n", (unsigned int)iComponent );
    
    delete [] mNodes[ iComponent ];
    mNodes.erase( iComponent );
}

/**********************************************************************/
cVisualizerHypertreeNode *cVisualizerHypertree::FindNodeByComponent( const IComponent *iComponent ) const
/**********************************************************************/
{
    tNodeHash::const_iterator it = mNodes.find( iComponent );
    if ( it == mNodes.end() ) return NULL;
    
    return it->second;
}


/**********************************************************************/
void cVisualizerHypertree::Layout( void )
/**********************************************************************/
{
    printf( "Separation is %f\n", GetSeparation() );
    Ogre::SubMesh *vSub = mLines->getSubMesh( 0 );
    vSub->vertexData->vertexCount = 0;
    vSub->indexData->indexCount = 0;

    vSub = mTexturedLines->getSubMesh( 0 );
    vSub->vertexData->vertexCount = 0;
    vSub->indexData->indexCount = 0;
    
    GetArrowheads()->clear();
    GetArrowfoots()->clear();
    
    mBoundsMin = Ogre::Vector3( FLT_MAX, FLT_MAX, FLT_MAX );
    mBoundsMax = Ogre::Vector3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
    
    cVisualizerHypertreeNode *vFocusNode = mNodes[ mFocus ];
    cVisualizerHypertreeNode::mActVisit++;
    vFocusNode->ComputeArea();
    cVisualizerHypertreeNode::mActVisit++;
    vFocusNode->Layout( Ogre::Vector3( 0, 0, 0 ), Ogre::Vector3( 0, -1, 0 ) );
    
    mLines->_setBounds( Ogre::AxisAlignedBox( mBoundsMin.x, mBoundsMin.y, mBoundsMin.z, mBoundsMax.x, mBoundsMax.y, mBoundsMax.z ) );
    mLines->_setBoundingSphereRadius( ( mBoundsMax - mBoundsMin ).length() * 0.5f );
    mTexturedLines->_setBounds( Ogre::AxisAlignedBox( mBoundsMin.x, mBoundsMin.y, mBoundsMin.z, mBoundsMax.x, mBoundsMax.y, mBoundsMax.z ) );
    mTexturedLines->_setBoundingSphereRadius( ( mBoundsMax - mBoundsMin ).length() * 0.5f );
}

/**********************************************************************/
cVisualizerHypertreeNode *cVisualizerHypertree::FindClickedNode( const Ogre::Viewport &iViewport, int vX, int vY )
/**********************************************************************/
{
    Ogre::Camera *vCamera = iViewport.getCamera();
    
    float vMinDepth = FLT_MAX;
    cVisualizerHypertreeNode *vResult = NULL;
    
    Ogre::Matrix4 vWorld      = mScene->getRootSceneNode()->_getFullTransform();
    Ogre::Matrix4 vView       = vCamera->getViewMatrix();
    Ogre::Matrix4 vProjection = vCamera->getProjectionMatrix();
    for ( stdext::hash_map<const IComponent*,cVisualizerHypertreeNode*,cComponentHash>::iterator it=mNodes.begin(); it != mNodes.end(); it++ )
    {
        cVisualizerHypertreeNode *vNode = it->second;
        
        float vDepth;
        cVisualizerHypertreeNode *vClicked = vNode->FindClickedNode( *mScene, iViewport, vX, vY, vDepth );
        if ( !vClicked ) continue;
        if ( vDepth > vMinDepth ) continue;
        
        vMinDepth = vDepth;
        vResult = vClicked;
    }
    
    return vResult;
}

/**********************************************************************/
cVisualizerHypertreeNode *cVisualizerHypertree::FindNodeByName( const Ogre::String &iName )
/**********************************************************************/
{
    for ( tNodeHash::iterator it = mNodes.begin(); it != mNodes.end(); it++ )
    {
        cVisualizerHypertreeNode *vNode = it->second;
        if ( vNode->GetName() == iName ) return vNode;
    }
    
    return NULL;
}

/**********************************************************************/
void cVisualizerHypertree::Clean( void )
/**********************************************************************/
{
//    return;
    
    while ( !mNodes.empty() )
    {
        cVisualizerHypertreeNode *vNode = mNodes.begin()->second;
        delete vNode->GetComponent();
        delete vNode;
        mNodes.erase( mNodes.begin() );
    }
    
    for ( size_t i=0; i<mBillboards.size(); i++ )
    {
        mBillboards[i]->clear();
    }
    
    GetArrowheads()->clear();
    GetArrowfoots()->clear();
    mFocus = NULL;
}

/**********************************************************************/
void cVisualizerHypertree::SetActNode( int iIndex )
/**********************************************************************/
{
    Ogre::MaterialPtr vMaterial = Ogre::MaterialManager::getSingleton().getByName( "LineMaterial" );
    Ogre::Technique *vTechnique = vMaterial->getTechnique(0);
    Ogre::Pass *vPass = vTechnique->getPass(0);
    Ogre::GpuProgramParametersSharedPtr vParams = vPass->getVertexProgramParameters();
    if ( vParams->_findNamedConstantDefinition( "mActNode" ) )
    {
        vParams->setNamedConstant( "mActNode", (float)iIndex );
    }
}

/**********************************************************************/
void cVisualizerHypertree::UpdateConnections( void )
/**********************************************************************/
{
    cVisualizerHypertreeNode::mActVisit++;
    
    Ogre::SubMesh *vSub = mTexturedLines->getSubMesh( 0 );
    vSub->vertexData->vertexCount = 0;
    vSub->indexData->indexCount = 0;

    cVisualizerHypertreeNode *vFocusNode = mNodes[ mFocus ];
    vFocusNode->UpdateConnections();
}
