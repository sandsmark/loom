/*
 *  cVisualizerHypertree.h
 *  Visor
 *
 *  Created by mb on 6/3/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Ogre/OgreVector3.h>
#include <Ogre/OgreMesh.h>
#include <Visor/Component/Events/IListenerComponent.h>

BEGIN_NAMESPACE( Visor )

class IComponent;
class cVisualizerHypertreeNode;

struct cComponentHash
{
	static const size_t bucket_size = 4;
	static const size_t min_buckets = 8;

    size_t operator()( const IComponent *iComponent ) const { return (size_t)iComponent; }
    bool operator()( const IComponent *iC1, const IComponent *iC2 ) const { return false; }
};

class cVisualizerHypertree : public IListenerComponent 
{
public:
    Ogre::Vector3 mBoundsMin;
    Ogre::Vector3 mBoundsMax;

	typedef stdext::hash_map<const IComponent*,cVisualizerHypertreeNode*> tNodeHash;

	typedef stdext::hash_map<void*,void*> tGeci;
    
protected:
    Ogre::SceneManager      *mScene;
    std::vector<Ogre::BillboardSet*> mBillboards;
    Ogre::MeshPtr            mLines;
    Ogre::BillboardSet*      mArrowHeads;
    Ogre::BillboardSet*      mArrowFoots;
    Ogre::BillboardSet*      mSelections;
    Ogre::MeshPtr            mTexturedLines;
    
    const IComponent *mFocus;

    tNodeHash mNodes;
    
    float mSeparation;
            
    void CreateLines( void );
    void CreateTexturedLines( void );
public:
    cVisualizerHypertree( Ogre::SceneManager *iScene );
    
    Ogre::BillboardSet *GetArrowheads( void ) const { return mArrowHeads;    }
    Ogre::BillboardSet *GetArrowfoots( void ) const { return mArrowFoots;    }
    Ogre::MeshPtr            GetLines( void ) const { return mLines;         }
    Ogre::MeshPtr    GetTexturedLines( void ) const { return mTexturedLines; }
    Ogre::SceneManager      *GetScene( void ) const { return mScene;         }
    Ogre::BillboardSet *GetBillboards( const int iType ) const { return mBillboards[ iType ]; }
    
    void SetFocus( const IComponent *iComponent ) { mFocus = iComponent; }
    const IComponent *GetFocus( void ) const { return mFocus; }
    cVisualizerHypertreeNode *FindNodeByComponent( const IComponent *iComponent ) const;
    
    const float GetSeparation( void ) const { return mSeparation; }
    void SetSeparation( const float iSeparation ) { mSeparation = iSeparation; }
    void Layout( void );
    void UpdateConnections( void );
    
    cVisualizerHypertreeNode *FindClickedNode( const Ogre::Viewport &iViewport, int vX, int vY );
    cVisualizerHypertreeNode *FindNodeByName( const Ogre::String &iName );
    
    const size_t GetNumNodes( void ) const { return mNodes.size(); }
    const tNodeHash &GetNodes( void ) const { return mNodes; }
    
    void Clean( void );
    
    void SetActNode( int iIndex );
    
    // IListenerComponent methods
    virtual void OnComponentCreated  ( IComponent *iComponent );
    virtual void OnComponentDestroyed( IComponent *iComponent );        
};

END_NAMESPACE()
