/*
 *  cVisualizerHypertreeNode.cpp
 *  Visor
 *
 *  Created by mb on 6/3/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#define _USE_MATH_DEFINES

#include "cVisualizerHypertreeNode.h"
#include <Visor/Component/IComponent.h>
#include <Visor/Hypertree/cVisualizerHypertree.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <Ogre/OgreBillboardSet.h>
#include <Ogre/OgreBillboard.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreSubMesh.h>
#include <Ogre/OgreSceneManager.h>
#include "MovableText.h"
using namespace Loom::Visor;

//#define HYPERBOLIC

int cVisualizerHypertreeNode::mActVisit = 0;

/**********************************************************************/
cVisualizerHypertreeNode::cVisualizerHypertreeNode( cVisualizerHypertree *iVisualizer, IComponent *iComponent )
/**********************************************************************/
: mComponent( iComponent ), mVisualizer( iVisualizer ), mName( "" ), mText( NULL ), mLastVisited( 0 ), mFilter( FILTER_NEUTRAL )
, mCollapsed( false ), mTextNode( NULL ),  mConnection( NAN, NAN )
{
    //    cDebug::Notify( "Basic visualizer created for component" );
    
    mType = 0;
}

/**********************************************************************/
void cVisualizerHypertreeNode::Init( void )
/**********************************************************************/
{    
    Ogre::BillboardSet *vBillboards = mVisualizer->GetBillboards( mType );
    
    Ogre::Vector3 vPosition = Ogre::Vector3( Ogre::Math::RangeRandom( -5, 5 ), Ogre::Math::RangeRandom( -5, 5 ), Ogre::Math::RangeRandom( -5, 5 ) );
    mBillboard = vBillboards->createBillboard( vPosition );
    mBillboard->setColour( Ogre::ColourValue( Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), Ogre::Math::RangeRandom( 0, 1 ), 0.5f ) );
    float vSize = Ogre::Math::RangeRandom( 1, 5 );
    vSize = 1;
    mBillboard->setDimensions( vSize, vSize );
    
    /*
    Ogre::MovableText *vText = new Ogre::MovableText( "FOS001", "Ez egy kis fos" );
    vText->setTextAlignment( MovableText::H_CENTER, MovableText::V_ABOVE );
    vText->setCastShadows( false );
    vText->setCharacterHeight( 2.0f );
    
    Ogre::SceneNode *vNode = mVisualizer->GetScene()->getRootSceneNode()->createChildSceneNode( iPosition );
    */
        
    mComponent->AddListener( *this );
}

/**********************************************************************/
void cVisualizerHypertreeNode::OnComponentDestroyed( IComponent *iComponent )
/**********************************************************************/
{
    Ogre::BillboardSet *vBillboards = mVisualizer->GetBillboards( mType );
    vBillboards->removeBillboard( mBillboard );
}

/**********************************************************************/
void cVisualizerHypertreeNode::ComputeArea( void )
/**********************************************************************/
{
    if ( mLastVisited == mActVisit ) return;
    mLastVisited = mActVisit;
    
   size_t vNumChildren = mComponent->GetNumChildren();
   mSurfaceArea = 0.005f;
#ifdef HYPERBOLIC
   mRadius   = asinhf( sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) ) );
   mDiskArea = 2 * (float)M_PI * ( coshf( mRadius ) - 1 );
#else
   mRadius   = sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) );
   mDiskArea    = mRadius * mRadius * (float)M_PI;
#endif
   //        mSurfaceArea = 0;
   if ( vNumChildren == 0 )
   {
	   return;
   }

    float mSurfaceArea = 0;
    for ( size_t i=0; i<vNumChildren; i++ )
    {
		const IComponent *vChild = mComponent->GetChild( i );
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );

        vNode->ComputeArea();
        mSurfaceArea += vNode->GetDiskArea();
    }
    
//    mSurfaceArea *= 5;
    mSurfaceArea *= mVisualizer->GetSeparation();
//    ASSERTTXT( mSurfaceArea > 0, "Surface area cannot be 0!" );
    if ( mSurfaceArea == 0 )
    {   // Treat like a leaf
        mSurfaceArea = 0.005f;
#ifdef HYPERBOLIC
        mRadius   = asinhf( sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) ) );
        mDiskArea = 2 * (float)M_PI * ( coshf( mRadius ) - 1 );
#else
        mRadius   = sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) );
        mDiskArea    = mRadius * mRadius * (float)M_PI;
#endif
        //        mSurfaceArea = 0;
        return;
    }
#ifdef HYPERBOLIC
    mRadius   = asinhf( sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) ) );
    mDiskArea = 2 * (float)M_PI * ( coshf( mRadius ) - 1 );
#else
    mRadius   = sqrtf( mSurfaceArea / ( 2 * (float)M_PI ) );
    mDiskArea = mRadius * mRadius * (float)M_PI;
#endif
}

/**********************************************************************/
void cVisualizerHypertreeNode::Layout( const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iNormal )
/**********************************************************************/
{
    if ( mLastVisited == mActVisit ) return;
    mLastVisited = mActVisit;
    
//    ASSERTTXT( iPosition.x == iPosition.x, "Invalid node position" );
	ASSERT( iPosition.x == iPosition.x );
    
    // Set our position
    mBillboard->setPosition( iPosition );
    if ( mTextNode ) mTextNode->setPosition( iPosition );
    mOriginalPosition = iPosition;
//    mBillboard->setColour( Ogre::ColourValue( iPosition.x * 0.02f, iPosition.y * 0.02f, iPosition.z * 0.02f, 1 ) );
//    mBillboard->setColour( Ogre::ColourValue( iPosition.x * 0.2f, iPosition.y * 0.2f, iPosition.z * 0.2f, 1 ) );

    /*
    static int vNumText = 0;
    if ( vNumText < 100 )
    {
        Ogre::MovableText *vText = new Ogre::MovableText( "FOS001", "Ez egy kis fos", "StarWars" );
        vText->setTextAlignment( Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE );
        vText->setCastShadows( false );
        vText->setCharacterHeight( 0.3f );   
        vText->setColor( Ogre::ColourValue( 1, 1, 1, 1 ) );
        Ogre::SceneNode *vNode = mVisualizer->GetScene()->getRootSceneNode()->createChildSceneNode( iPosition );
//        Ogre::SceneNode *vNode = mVisualizer->GetScene()->getRootSceneNode();
        vNode->attachObject( vText );
        vNumText++;
    }
    */
    
    if ( iPosition.x < mVisualizer->mBoundsMin.x ) mVisualizer->mBoundsMin.x = iPosition.x;
    if ( iPosition.y < mVisualizer->mBoundsMin.y ) mVisualizer->mBoundsMin.y = iPosition.y;
    if ( iPosition.z < mVisualizer->mBoundsMin.z ) mVisualizer->mBoundsMin.z = iPosition.z;
    if ( iPosition.x > mVisualizer->mBoundsMax.x ) mVisualizer->mBoundsMax.x = iPosition.x;
    if ( iPosition.y > mVisualizer->mBoundsMax.y ) mVisualizer->mBoundsMax.y = iPosition.y;
    if ( iPosition.z > mVisualizer->mBoundsMax.z ) mVisualizer->mBoundsMax.z = iPosition.z;
    
/*    
    Ogre::Entity *vSphere = mVisualizer->GetScene()->createEntity( Ogre::SceneManager::PT_SPHERE );
    Ogre::SceneNode *vNode = mVisualizer->GetScene()->getRootSceneNode()->createChildSceneNode( iPosition );
    vNode->attachObject( vSphere );
//    vNode->setScale( Ogre::Vector3( 0.01f, 0.01f, 0.01f ) );
    vNode->setScale( Ogre::Vector3( 0.01f, 0.01f, 0.01f ) * mRadius );
 */
    
    // Sort our children ascending by radius
    std::vector<cVisualizerHypertreeNode*> vChildNodes;
    size_t vNumChildren = mComponent->GetNumChildren();
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        
        const IComponent *vChild = mComponent->GetChild( i );
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        vChildNodes.push_back( vNode );
    }
    std::sort( vChildNodes.begin(), vChildNodes.end() );
    
    // Initialize values
    float vTheta = FLT_MAX;
    float vPhi   = 0;
#ifdef HYPERBOLIC
    float vBandRadius = sinhf( mRadius ) * sinf( vPhi );
#else
    float vBandRadius = mRadius * sinf( vPhi );
#endif
    float vPrevRadius = 0;
    
    // Iterative circle packing on the hemisphere
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        cVisualizerHypertreeNode *vNode = vChildNodes[i];
        
        // Step with theta
        float vNodeRadius = vNode->GetRadius();
#ifdef HYPERBOLIC
        float vThetaD = atanf( tanhf( vPrevRadius ) / vBandRadius ) + atanf( tanhf( vNodeRadius ) / vBandRadius );
#else
        float vThetaD = atanf( vPrevRadius / vBandRadius ) + atanf( vNodeRadius / vBandRadius );
#endif
        vTheta += vThetaD;
        
        if ( vPrevRadius == 0 || vTheta > 2 * (float)M_PI )
        {   // In case of wrapping, start the next band of circles
#ifdef HYPERBOLIC
            float vPhiD = atanf( tanhf( vPrevRadius ) / sinhf( mRadius ) ) + atanf( tanhf( vNodeRadius ) / sinhf( mRadius ) );
#else
            float vPhiD = atanf( vPrevRadius / mRadius ) + atanf( vNodeRadius / mRadius );
#endif
            vPhi += vPhiD;
//            ASSERTTXT( vPhiD < (float)M_PI, "Child is out of hemisphere surface" );
            if ( vPhiD >= (float)M_PI ) vPhiD = M_PI;
            vTheta = 0;
#ifdef HYPERBOLIC
            vBandRadius = sinhf( mRadius ) * sinf( vPhi );
#else
            vBandRadius = mRadius * sinf( vPhi );
#endif
        }
        vPrevRadius = vNodeRadius;
        
        // Compute position from the angles
        Ogre::Vector3 vPosition = iNormal * mRadius;
        Ogre::Vector3 vRight = iNormal.crossProduct( Ogre::Vector3::UNIT_Y );
        if ( vRight.length() < 0.01f )
        {
            vRight = iNormal.crossProduct( Ogre::Vector3::UNIT_X );
        }
        vRight /= vRight.length();
        Ogre::Matrix3 vRotation;
        vRotation.FromAxisAngle( vRight, Ogre::Radian( vPhi ) );
        vPosition = vPosition * vRotation;
        vRotation.FromAxisAngle( iNormal, Ogre::Radian( vTheta ) );
        vPosition = vPosition * vRotation;
        
        Ogre::Vector3 vNormal = vPosition;
        float vLength = vNormal.length();
//        ASSERTTXT( vLength == vLength, "Invalid normal vector" );
		ASSERT( vLength == vLength );
        if ( vLength < 0.001f ) vNormal = Ogre::Vector3( 0, 1, 0 );
        vNormal /= vNormal.length();
        vPosition += iPosition;
        
        // Layout child
        vNode->Layout( vPosition, vNormal );
    }
    
    DrawLines();
    DrawTexturedLines();
}

/**********************************************************************/
const Ogre::ColourValue cVisualizerHypertreeNode::GetLineColor( const cVisualizerHypertreeNode *iTarget )
/**********************************************************************/
{
    return Ogre::ColourValue( 0.8f, 0.8f, 0.8f, 1 );
}

//size_t gNumSegments = 5;
size_t gNumSegments = 1;
/**********************************************************************/
void cVisualizerHypertreeNode::DrawLines( void )
/**********************************************************************/
{
    size_t vNumChildren = mComponent->GetNumChildren();
    if ( vNumChildren == 0 ) return;
    
    Ogre::SubMesh *vSub = mVisualizer->GetLines()->getSubMesh( 0 );
    
    // Fill vertices
    Ogre::HardwareVertexBufferSharedPtr vVertices = vSub->vertexData->vertexBufferBinding->getBuffer(0);
    
//    const Ogre::ColourValue vColor( 0, 0.4f, 0, 1 );
//    const Ogre::ColourValue vColorFrom( 1.0f, 0.0f, 0.0f, 1 );
//    const Ogre::ColourValue vColor( 0.0f, 1.0f, 0.0f, 1 );
    
//    const Ogre::ColourValue vColorFrom( 0.8f, 0.8f, 0.8f, 1 );
//    const Ogre::ColourValue vColor( 0.8f, 0.8f, 0.8f, 1 );
    
//    sVertex *vVtx = (sVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * ( 1 + vNumChildren * ( gNumSegments ) ), Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    sVertex *vVtx = (sVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * ( vNumChildren * ( gNumSegments + 1 ) ), Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        
        const Ogre::ColourValue vColor = GetLineColor( vNode );
        const Ogre::ColourValue vColorFrom = vColor;

        
        vVtx->Position = mBillboard->getPosition(); vVtx->Color = vColorFrom;
        vVtx->Nodes = Ogre::Vector2( (float)mId, (float)vNode->GetId() );
        vVtx++;
        for ( size_t s=0; s<gNumSegments; s++ )
        {
            vVtx->Position = mBillboard->getPosition() + ( vNode->mBillboard->getPosition() - mBillboard->getPosition() ) * (float)( s + 1 ) / (float)gNumSegments;
            vVtx->Color    = vColor;
            vVtx->Nodes    = Ogre::Vector2( (float)mId, (float)vNode->GetId() );
            vVtx++;
        }
    }
    vVertices->unlock();
    
    // Fill indices
    Ogre::HardwareIndexBufferSharedPtr vIndices = vSub->indexData->indexBuffer;
    Ogre::ulong *vIdx = (Ogre::ulong*)( vIndices->lock( vSub->indexData->indexCount * sizeof( Ogre::ulong ), sizeof( Ogre::ulong ) * ( gNumSegments ) * vNumChildren, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        for ( size_t s=0; s<gNumSegments; s++ )
        {
            /*
            if ( s == 0 )
            {
                *vIdx++ = vSub->vertexData->vertexCount;
            }
            else
            {
                *vIdx++ = vSub->vertexData->vertexCount + 1 + i * ( gNumSegments ) + s - 1;
            }

            *vIdx++ = vSub->vertexData->vertexCount + 1 + i * ( gNumSegments ) + s;
            */
            *vIdx++ = vSub->vertexData->vertexCount + 0 + i * ( gNumSegments + 1 ) + s;
            *vIdx++ = vSub->vertexData->vertexCount + 1 + i * ( gNumSegments + 1 ) + s;
        }
    }
    vIndices->unlock();
    
//    vSub->vertexData->vertexCount += vNumChildren * ( gNumSegments ) + 1;
    vSub->vertexData->vertexCount += vNumChildren * ( gNumSegments + 1 );
    vSub->indexData->indexCount   += vNumChildren * ( gNumSegments * 2 );
    
    // Arrowheads and foots
    Ogre::BillboardSet *vArrowHeads = mVisualizer->GetArrowheads();
    Ogre::BillboardSet *vArrowFoots = mVisualizer->GetArrowfoots();
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        if ( vChild == mComponent ) continue;
        
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );

        Ogre::Vector3 vNormal = ( mBillboard->getPosition() - vNode->mBillboard->getPosition() );
        float vLength = vNormal.length();
        if ( vLength < 0.001f ) continue;
        
        // Arrowhead
        Ogre::Billboard *vArrowHead = vArrowHeads->createBillboard( vNode->mBillboard->getPosition() );
        vNormal /= vLength;
        vNormal *= 0.5f;
        vNormal += Ogre::Vector3( 0.5f, 0.5f, 0.5f );
        vArrowHead->setColour( Ogre::ColourValue( vNormal.x, vNormal.y, vNormal.z, vLength / 2 ) );
        float vSize = 0;
        vArrowHead->setDimensions( vSize, vSize );
        
        // Arrowfoot
        Ogre::Billboard *vArrowFoot = vArrowFoots->createBillboard( mBillboard->getPosition() );        
        vArrowFoot->setColour( Ogre::ColourValue( -vNormal.x, -vNormal.y, -vNormal.z, vLength / 2 ) );
        vArrowFoot->setDimensions( vSize, vSize );        
    }
}

#if 0
/**********************************************************************/
void cVisualizerHypertreeNode::DrawTexturedLines( void )
/**********************************************************************/
{
    size_t vNumChildren = mComponent->GetNumChildren();
    if ( vNumChildren == 0 ) return;
    
    Ogre::SubMesh *vSub = mVisualizer->GetTexturedLines()->getSubMesh( 0 );
    
    // Fill vertices
    Ogre::HardwareVertexBufferSharedPtr vVertices = vSub->vertexData->vertexBufferBinding->getBuffer(0);
    
    sTexturedVertex *vVtx = (sTexturedVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * ( vNumChildren * ( gNumSegments + 1 ) * 2 ), Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        
        const Ogre::ColourValue vColor = GetLineColor( vNode );
        const Ogre::ColourValue vColorFrom = vColor;
        
        
        float vLength = 0;
        for ( size_t s=0; s<=gNumSegments; s++ )
        {
            vVtx->PositionFrom = mBillboard->getPosition();
            vVtx->PositionTo   = vNode->mBillboard->getPosition();
            vVtx->Alpha = (float)( s ) / (float)gNumSegments;
//            vVtx->PositionFrom = mBillboard->getPosition() + ( vNode->mBillboard->getPosition() - mBillboard->getPosition() ) * vVtx->Alpha;
            vVtx->Tex      = Ogre::Vector2( 0, 0 );
            vVtx++;
            *vVtx = vVtx[-1];
            vVtx->Tex      = Ogre::Vector2( 0, 1 );
            vVtx++;
        }
    }
    vVertices->unlock();
    
    // Fill indices
    Ogre::HardwareIndexBufferSharedPtr vIndices = vSub->indexData->indexBuffer;
    Ogre::ulong *vIdx = (Ogre::ulong*)( vIndices->lock( vSub->indexData->indexCount * sizeof( Ogre::ulong ), sizeof( Ogre::ulong ) * ( gNumSegments * 6 ) * vNumChildren, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        int vBase = vSub->vertexData->vertexCount + i * ( ( gNumSegments + 1 ) * 2 );
        for ( size_t s=0; s<gNumSegments; s++ )
        {
            int vSegmentBase = vBase + s * 2;
            
            *vIdx++ = vSegmentBase + 1;
            *vIdx++ = vSegmentBase + 0;
            *vIdx++ = vSegmentBase + 3;
            
            *vIdx++ = vSegmentBase + 3;
            *vIdx++ = vSegmentBase + 2;
            *vIdx++ = vSegmentBase + 1;
        }
    }
    vIndices->unlock();
    
    //    vSub->vertexData->vertexCount += vNumChildren * ( gNumSegments ) + 1;
    vSub->vertexData->vertexCount += vNumChildren * ( ( gNumSegments + 1 ) * 2 );
    vSub->indexData->indexCount   += vNumChildren * ( gNumSegments * 6 );    
}
#endif

size_t gNumTexturedSegments = 15;
/**********************************************************************/
void cVisualizerHypertreeNode::DrawTexturedLines( void )
/**********************************************************************/
{
    if ( mConnection.x == NAN ) return;
    
    Ogre::SubMesh *vSub = mVisualizer->GetTexturedLines()->getSubMesh( 0 );
    
    // Fill vertices
    Ogre::HardwareVertexBufferSharedPtr vVertices = vSub->vertexData->vertexBufferBinding->getBuffer(0);
    
    sTexturedVertex *vVtx = (sTexturedVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * ( gNumTexturedSegments + 1 ) * 2, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );
    
    float vLength = 0;
    for ( size_t s=0; s<=gNumTexturedSegments; s++ )
    {
        vVtx->PositionFrom = Ogre::Vector3( mConnection.x, mConnection.y, 0 );
        vVtx->PositionTo   = mBillboard->getPosition();
        vVtx->Alpha = (float)( s ) / (float)gNumTexturedSegments;
        //            vVtx->PositionFrom = mBillboard->getPosition() + ( vNode->mBillboard->getPosition() - mBillboard->getPosition() ) * vVtx->Alpha;
        vVtx->Tex      = Ogre::Vector2( 0, 0 );
        vVtx++;
        *vVtx = vVtx[-1];
        vVtx->Tex      = Ogre::Vector2( 0, 1 );
        vVtx++;
    }

    vVertices->unlock();
    
    // Fill indices
    Ogre::HardwareIndexBufferSharedPtr vIndices = vSub->indexData->indexBuffer;
    Ogre::ulong *vIdx = (Ogre::ulong*)( vIndices->lock( vSub->indexData->indexCount * sizeof( Ogre::ulong ), sizeof( Ogre::ulong ) * ( gNumTexturedSegments * 6 ), Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );

    int vBase = vSub->vertexData->vertexCount;
    for ( size_t s=0; s<gNumTexturedSegments; s++ )
    {
        int vSegmentBase = vBase + s * 2;
        
        *vIdx++ = vSegmentBase + 1;
        *vIdx++ = vSegmentBase + 0;
        *vIdx++ = vSegmentBase + 3;
        
        *vIdx++ = vSegmentBase + 3;
        *vIdx++ = vSegmentBase + 2;
        *vIdx++ = vSegmentBase + 1;
    }

    vIndices->unlock();
    
    vSub->vertexData->vertexCount += ( gNumTexturedSegments + 1 ) * 2;
    vSub->indexData->indexCount   += ( gNumTexturedSegments * 6 );    
}

/**********************************************************************/
const Ogre::Vector3 &cVisualizerHypertreeNode::GetPosition( void ) const
/**********************************************************************/
{
    return mBillboard->getPosition();
}

/**********************************************************************/
void cVisualizerHypertreeNode::SetName( const Ogre::String &iName )
/**********************************************************************/
{
    mName = iName;

    if ( mName == "" && mText )
    {
        SAFE_DELETE( mText );
        SAFE_DELETE( mTextNode );
        return;
    }
    
    if ( !mText )
    {
        mText = new Ogre::MovableText( "ID", iName, "CustomTextMaterial", "VisorFont" );
        mText->setTextAlignment( Ogre::MovableText::H_CENTER, Ogre::MovableText::V_BELOW );
        mText->setCastShadows( false );
//        mText->setCharacterHeight( 0.1f );   
//        mText->setColor( Ogre::ColourValue( 1, 1, 1, 1 ) );
        mText->setColor( mBillboard->getColour() );
        mText->ForceVisible( true );
        mTextNode = mVisualizer->GetScene()->getRootSceneNode()->createChildSceneNode( mBillboard->getPosition() );
        Ogre::AxisAlignedBox vBounds;
        vBounds = mTextNode->_getWorldAABB();
        mTextNode->attachObject( mText );
        mTextNode->_updateBounds();
        vBounds = mTextNode->_getWorldAABB();
        bool vVisible;
        vVisible = mVisualizer->GetScene()->getCamera( "MainCamera" )->isVisible( vBounds );
    }
    else
    {
        mText->setCaption( iName );
    }
}

/**********************************************************************/
void cVisualizerHypertreeNode::Filter( const eFilter iFiltered )
/**********************************************************************/
{
    mFilter = iFiltered;
    
    Ogre::ColourValue vColor = mBillboard->getColour();
    switch ( iFiltered )
    {
    case FILTER_HIDE:
        vColor.a =    0;
        break;
    case FILTER_HIGHLIGHT:
        vColor.a =    1;
        break;
    default:
        vColor.a = 0.5f;
        break;
    }

    mBillboard->setColour( vColor );
}

/**********************************************************************/
void cVisualizerHypertreeNode::Expand( void )
/**********************************************************************/
{
}

/**********************************************************************/
void cVisualizerHypertreeNode::Collapse( void )
/**********************************************************************/
{
    size_t vNumChildren = mComponent->GetNumChildren();
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        if ( vChild == mComponent ) continue;
        
    }
}

/**********************************************************************/
void cVisualizerHypertreeNode::CollapseToPosition( const Ogre::Vector3 &iPosition )
/**********************************************************************/
{
}

/**********************************************************************/
void cVisualizerHypertreeNode::ExpandToOriginalPosition( void )
/**********************************************************************/
{
}

/**********************************************************************/
void cVisualizerHypertreeNode::SetConnection( const Ogre::Viewport &iViewport, int vX, int vY )
/**********************************************************************/
{
    Ogre::Camera *vCamera = iViewport.getCamera();
    Ogre::Matrix4 vView       = vCamera->getViewMatrix();
    Ogre::Matrix4 vProjection = vCamera->getProjectionMatrix();
    Ogre::Matrix4 vInvProjection = vProjection.inverse();
    
    Ogre::Vector4 vViewpos( vX, vY, 0, 1 );
    vViewpos = vInvProjection * vViewpos;
    vViewpos.x /= vViewpos.w;
    vViewpos.y /= vViewpos.w;
    SetConnection( Ogre::Vector2( vViewpos.x, vViewpos.y ) );
}

/**********************************************************************/
cVisualizerHypertreeNode *cVisualizerHypertreeNode::FindClickedNode( Ogre::SceneManager &iScene, const Ogre::Viewport &iViewport, int vX, int vY, float &oDepth )
/**********************************************************************/
{
    Ogre::Camera *vCamera = iViewport.getCamera();
    
    float vDepth = FLT_MAX;
    cVisualizerHypertreeNode *vResult = NULL;
    
    Ogre::Matrix4 vWorld      = iScene.getRootSceneNode()->_getFullTransform();
    Ogre::Matrix4 vView       = vCamera->getViewMatrix();
    Ogre::Matrix4 vProjection = vCamera->getProjectionMatrix();

    
    Ogre::Vector3 vViewPos = GetPosition();
    vViewPos = vWorld * vViewPos;
    float vLength = vViewPos.length();
    if ( vLength > 0.01f )
    {   // Hyperbolic transformation
        vViewPos /= vLength;
        vLength = tanhf( vLength );
        vViewPos *= vLength;
    }
    vViewPos *= 10;
    vViewPos = vView * vViewPos;
    
    // Check main node
    Ogre::Vector3 vProjPos = vViewPos;
    vProjPos = vProjection * vProjPos;
    vProjPos.x *=  0.5f; vProjPos.x += 0.5f;
    vProjPos.y *= -0.5f; vProjPos.y += 0.5f;
    vProjPos.x *= iViewport.getActualWidth();
    vProjPos.y *= iViewport.getActualHeight();
    
    if ( vProjPos.z < 0 ) return NULL;
    
    Ogre::Vector2 vDiff( vProjPos.x - vX, vProjPos.y - vY );
    float vDistance = vDiff.length();
    
    float vMinDistance = vDistance;
    if ( fabsf( vProjPos.x - vX ) > 15 || fabsf( vProjPos.y - vY ) > 15 )
    {
        vResult = NULL;
    }
    else
    {
        vResult = this;
    }
    vDepth = vProjPos.z;
    oDepth = vDepth;

    size_t vNumChildren = mComponent->GetNumChildren();
    
    // Check line heads
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        if ( vChild == mComponent ) continue;
        
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        
        Ogre::Vector3 vPos = vNode->GetPosition();
        vPos = vWorld * vPos;
        float vLength = vPos.length();
        if ( vLength > 0.01f )
        {   // Hyperbolic transformation
            vPos /= vLength;
            vLength = tanhf( vLength );
            vPos *= vLength;
        }
        vPos *= 10;
        vPos = vView * vPos;
        
        Ogre::Vector3 vNormal = ( vPos - vViewPos );
        vLength = vNormal.length();
        if ( vLength < 0.01f ) continue;
        
        vNormal /= vLength;
        Ogre::Vector3 vProjPos = vViewPos + vNormal * 0.25f;
        vProjPos = vProjection * vProjPos;
        vProjPos.x *=  0.5f; vProjPos.x += 0.5f;
        vProjPos.y *= -0.5f; vProjPos.y += 0.5f;
        vProjPos.x *= iViewport.getActualWidth();
        vProjPos.y *= iViewport.getActualHeight();

        if ( fabsf( vProjPos.x - vX ) > 15 || fabsf( vProjPos.y - vY ) > 15 ) continue;
        
        Ogre::Vector2 vDiff( vProjPos.x - vX, vProjPos.y - vY );
        float vDistance = vDiff.length();
        if ( vDistance >= vMinDistance ) continue;
        
        vMinDistance = vDistance;
        vResult = vNode;
    }

    // Check line ends
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        if ( vChild == mComponent ) continue;
        
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        
        Ogre::Vector3 vPos = vNode->GetPosition();
        vPos = vWorld * vPos;
        float vLength = vPos.length();
        if ( vLength > 0.01f )
        {   // Hyperbolic transformation
            vPos /= vLength;
            vLength = tanhf( vLength );
            vPos *= vLength;
        }
        vPos *= 10;
        vPos = vView * vPos;
        
        Ogre::Vector3 vNormal = ( vPos - vViewPos );
        vLength = vNormal.length();
        if ( vLength < 0.01f ) continue;
        
        vNormal /= vLength;
        Ogre::Vector3 vProjPos = vPos - vNormal * 0.25f;
        vProjPos = vProjection * vProjPos;
        vProjPos.x *=  0.5f; vProjPos.x += 0.5f;
        vProjPos.y *= -0.5f; vProjPos.y += 0.5f;
        vProjPos.x *= iViewport.getActualWidth();
        vProjPos.y *= iViewport.getActualHeight();
        
        if ( fabsf( vProjPos.x - vX ) > 15 || fabsf( vProjPos.y - vY ) > 15 ) continue;
        
        Ogre::Vector2 vDiff( vProjPos.x - vX, vProjPos.y - vY );
        float vDistance = vDiff.length();
        if ( vDistance >= vMinDistance ) continue;
        
        vMinDistance = vDistance;
        vResult = this;
    }
    
    return vResult;
}

/**********************************************************************/
void cVisualizerHypertreeNode::UpdateConnections()
/**********************************************************************/
{
    if ( mLastVisited == mActVisit ) return;
    mLastVisited = mActVisit;

    size_t vNumChildren = mComponent->GetNumChildren();
    for ( size_t i=0; i<vNumChildren; i++ )
    {
        const IComponent *vChild = mComponent->GetChild( i );
        cVisualizerHypertreeNode *vNode = mVisualizer->FindNodeByComponent( vChild );
        
        vNode->UpdateConnections();
    }
    
    DrawTexturedLines();
}
