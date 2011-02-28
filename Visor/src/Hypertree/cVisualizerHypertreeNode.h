/*
 *  cVisualizerHypertreeNode.h
 *  Visor
 *
 *  Created by mb on 6/3/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <math.h>
#include "MovableText.h"

#include <xmath.h>
#include <Visor/Component/Events/IListenerComponent.h> 

BEGIN_NAMESPACE( Visor )

class IComponent;
class cVisualizerHypertree;

class cVisualizerHypertreeNode : public IListenerComponent 
{
    friend class cVisualizerHypertree;

//    protected:
public:
    struct sVertex
    {
        Ogre::Vector3  Position;
        Ogre::ColourValue Color;
        Ogre::Vector2     Nodes;
    };
    struct sTexturedVertex
    {
        Ogre::Vector3  PositionFrom;
        Ogre::Vector3    PositionTo;
        float                 Alpha;
        Ogre::Vector2           Tex;
    };
    
    enum eFilter
    {
        FILTER_NEUTRAL,
        FILTER_HIDE,
        FILTER_HIGHLIGHT,
    };
    
protected:
    IComponent        *mComponent;
    cVisualizerHypertree *mVisualizer;
    
    Ogre::String mName;
    int mType;
    int mId;
    
    Ogre::Billboard   *mBillboard;
    Ogre::Billboard   *mSelectionBillboard;
    Ogre::MovableText *mText;
    Ogre::SceneNode   *mTextNode;
    
    int mLastVisited;
    static int mActVisit;
    
    eFilter mFilter;
    
    bool mCollapsed;
    Ogre::Vector3 mOriginalPosition;

    Ogre::Vector2 mConnection;
    
    // Hemisphere data
    float mSurfaceArea;        
    float mDiskArea;        
    float mRadius;        
    void ComputeArea( void );
    void Layout( const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iNormal );
    void UpdateConnections();
    
    virtual void Init( void );
    void DrawLines( void );
    void DrawTexturedLines( void );
    
    void CollapseToPosition( const Ogre::Vector3 &iPosition );
    void ExpandToOriginalPosition( void );
    
    virtual const Ogre::ColourValue GetLineColor( const cVisualizerHypertreeNode *iTarget );
            
public:
    cVisualizerHypertreeNode( cVisualizerHypertree *iVisualizer, IComponent *iComponent );
    ~cVisualizerHypertreeNode() { /*delete mBillboard;*/ }
    
    const float GetRadius     ( void ) const { return mRadius;      }
    const float GetDiskArea   ( void ) const { return mDiskArea;    }
    const float GetSurfaceArea( void ) const { return mSurfaceArea; }
    
    bool operator <( const cVisualizerHypertreeNode &iOther )
    {
        return ( mRadius < iOther.mRadius );
    }
    
    const Ogre::Vector3 &GetPosition( void ) const;
    
    const Ogre::String &GetName( void ) const { return mName; }
    void SetName( const Ogre::String &iName );
    
    void Filter( const eFilter iFiltered );
    const eFilter GetFilter( void ) { return mFilter; }
    
    void SetId( int iId ) { mId = iId; }
    int  GetId( void ) const { return mId; }
    
    IComponent *GetComponent( void ) const { return mComponent; }
    const cVisualizerHypertree *GetVisualizer() const { return mVisualizer; }
    
    void Expand( void );
    void Collapse( void );
    
    const Ogre::Vector2 &GetConnection( void ) const { return mConnection; }
    void SetConnection( const Ogre::Vector2 &iConnection ) { mConnection = iConnection; }
    void SetConnection( const Ogre::Viewport &iViewport, int vX, int vY );
    void RemoveConnection( void ) { mConnection = Ogre::Vector2( NAN, NAN ); }
    
    // IListenerComponent methods
    virtual void OnComponentCreated    ( IComponent *iComponent ) {}
    virtual void OnComponentDestroyed  ( IComponent *iComponent );
    virtual void OnComponentInitialized( IComponent *iComponent ) {}
    
    cVisualizerHypertreeNode *FindClickedNode( Ogre::SceneManager &iScene, const Ogre::Viewport &iViewport, int vX, int vY, float &oDepth );
};

END_NAMESPACE()

