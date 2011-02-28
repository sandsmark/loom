/*
 *  cVisualizerReplicode.cpp
 *  Visor
 *
 *  Created by mb on 7/15/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerReplicode.h"
#include <Visor/Replicode/Hypertree/cVisualizerReplicodeNode.h>
using namespace Loom::Visor;

/**********************************************************************/
void cVisualizerReplicode::OnComponentCreated( IComponent *iComponent )
/**********************************************************************/
{
    //    printf( "[cVisualizerHypertree::OnComponentCreated] 0x%08x\n", (unsigned int)iComponent );
    
    cVisualizerReplicodeNode *vNode = new cVisualizerReplicodeNode( this, (cComponentReplicode*)iComponent );
    vNode->SetId( mNodes.size() );
    mNodes[ iComponent ] = vNode;
    
    if ( !mFocus )
    {
        mFocus = iComponent;
//        vNode->SetConnection( Ogre::Vector2( -1, 0.9f ) );
    }
}

/**********************************************************************/
void cVisualizerReplicode::OnComponentInitialized( IComponent *iComponent )
/**********************************************************************/
{
    ((cVisualizerReplicodeNode*)mNodes[ iComponent ])->Init();
}
