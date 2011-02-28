/*
 *  cVisualizerReplicodeNode.cpp
 *  Visor
 *
 *  Created by mb on 7/15/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerReplicodeNode.h"
#include <Visor/Hypertree/cVisualizerHypertree.h>
#include <Visor/Replicode/Component/cComponentReplicode.h>
#include <Visor/Replicode/Hypertree/cVisualizerReplicodeSettings.h>
#include <Replicode/r_code/object.h>
#include <Replicode/r_exec/opcodes.h>
#include <Ogre/OgreBillboard.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreSceneNode.h>

using namespace Loom::Visor;

/**********************************************************************/
cVisualizerReplicodeNode::cVisualizerReplicodeNode( cVisualizerHypertree *iVisualizer, cComponentReplicode *iComponent )
/**********************************************************************/
: cVisualizerHypertreeNode( iVisualizer, (IComponent*)iComponent ), mTextCode( NULL )
{
}

/**********************************************************************/
void cVisualizerReplicodeNode::Init( void )
/**********************************************************************/
{
    cComponentReplicode *vComponent = (cComponentReplicode*)mComponent;
    r_code::Code *vCode = vComponent->GetCode();
    if ( vCode )
    {
        r_code::Atom vAtom = vCode->code(0);
        uint16 vOpCode = vAtom.asOpcode();
        
        mType = 0; //( ((cComponentReplicode*)mComponent)->GetCode()->code(0).asOpcode() & 0xfff ) % 7;
    }
    
    cVisualizerHypertreeNode::Init();

    if ( vCode->code(0).asOpcode() == r_exec::Opcodes::MkRdx )
    {
        mBillboard->setColour( cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_NODE_REDUCTION ) );
    }
    else
    {
        mBillboard->setColour( cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_NODE_DEFAULT ) );
    }
    
    char Temp[ 256 ];
    uint8 vOpcode = vCode->code(0).asOpcode();
    if ( vComponent->GetView() )
    {
        r_code::View *vView = vComponent->GetView();
        sprintf( Temp, "| %sview %04d |", ( vComponent->IsNew() ) ? "*" : "", vComponent->GetView()->code( VIEW_OID ).asOpcode() );
    }
    else if ( vOpcode == r_exec::Opcodes::IPgm )
    {
        sprintf( Temp, "[ ipgm %04d ]>", rand() % 9999 );
        Ogre::ColourValue vColor = mBillboard->getColour();
        vColor.a = 1 - vColor.a;   // Controls inverse display
        mBillboard->setColour( vColor );
    }
    else if ( vOpcode == r_exec::Opcodes::Pgm || vOpcode == r_exec::Opcodes::AntiPgm )
    {
        sprintf( Temp, "[ pgm %04d ]>", rand() % 9999 );
    }
	/*
    else if ( vOpcode == r_exec::Opcodes::IGoal )
    {
        sprintf( Temp, "[ igoal %04d ]<", rand() % 9999 );
        Ogre::ColourValue vColor = mBillboard->getColour();
        vColor.a = 1 - vColor.a;   // Controls inverse display
        mBillboard->setColour( vColor );
    }
    else if ( vOpcode == r_exec::Opcodes::Goal || vOpcode == r_exec::Opcodes::AntiGoal )
    {
        sprintf( Temp, "[ goal %04d ]<", rand() % 9999 );
    }
	*/
	else if ( vOpcode == r_exec::Opcodes::MkGoal )
	{
		sprintf( Temp, "[ goal %04d ]<", rand() % 9999 );
	}
    else if ( vOpcode == r_exec::Opcodes::MkRdx || vOpcode == r_exec::Opcodes::MkAntiRdx )
    {
        sprintf( Temp, "[ > ]" );
    }
    else if ( vOpcode == r_exec::Opcodes::MkNew ||

              vOpcode == r_exec::Opcodes::MkLowRes  ||
              vOpcode == r_exec::Opcodes::MkLowSln  ||
              vOpcode == r_exec::Opcodes::MkHighSln ||
              vOpcode == r_exec::Opcodes::MkLowAct  ||
              vOpcode == r_exec::Opcodes::MkHighAct ||
              vOpcode == r_exec::Opcodes::MkSlnChg  ||
              vOpcode == r_exec::Opcodes::MkActChg )
    {
        sprintf( Temp, "[ marker %04d ]", rand() % 9999 );
    }
    else if ( vOpcode == r_exec::Opcodes::Ent )
    {
        sprintf( Temp, "( entity %04d )", rand() % 9999 );
    }
    else
    {
        sprintf( Temp, "| object %04d |", rand() % 9999 );
    }
    SetName( Temp );
}

/**********************************************************************/
void cVisualizerReplicodeNode::SetCodeReference( const Ogre::String &iCaption )
/**********************************************************************/
{
    mCaption = iCaption;
    
    if ( mCaption == "" && mTextCode )
    {
        SAFE_DELETE( mTextCode );
        return;
    }
    
    if ( !mTextCode )
    {
        mTextCode = new Ogre::MovableText( "ID", mCaption, "CustomCaptionTextMaterial", "VisorFont" );
        mTextCode->setTextAlignment( Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE );
        mTextCode->setCastShadows( false );
        mTextCode->setCharacterHeight( 4.0f );   
        mTextCode->setColor( Ogre::ColourValue( 1, 1, 1, 1 ) );
        mTextCode->ForceVisible( true );
        Ogre::SceneNode *vNode = mVisualizer->GetScene()->getRootSceneNode()->createChildSceneNode( mBillboard->getPosition() );
        Ogre::AxisAlignedBox vBounds;
        vBounds = vNode->_getWorldAABB();
        vNode->attachObject( mTextCode );
        vNode->_updateBounds();
        vBounds = vNode->_getWorldAABB();
    }
    else
    {
        mTextCode->setCaption( mCaption );
    }
}

/**********************************************************************/
const Ogre::ColourValue cVisualizerReplicodeNode::GetLineColor( const cVisualizerHypertreeNode *iTarget )
/**********************************************************************/
{
    cComponentReplicode *vComponent = (cComponentReplicode*)mComponent;
    r_code::Code *vCode = vComponent->GetCode();
    
    cComponentReplicode *vTargetComponent = (cComponentReplicode*)iTarget->GetComponent();
    r_code::Code *vTargetCode = vTargetComponent->GetCode();
    
    if ( vCode->code(0).asOpcode() == r_exec::Opcodes::MkRdx )
    {
        int vNumInputs = vCode->code(2).getAtomCount();
        int vNumProducts = vCode->code(3).getAtomCount();
        if ( vCode->get_reference(0) == vTargetCode )
        {   // ipgm
            return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_REDUCTION_MEMBER );
        }

        for ( int i=0; i<vNumInputs; i++ )
        {
            if ( vCode->get_reference(1+i) == vTargetCode )
            {   // input
                return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_REDUCTION_INPUT );
            }
        }

        for ( int i=0; i<vNumProducts; i++ )
        {
            if ( vCode->get_reference(1+vNumInputs+i) == vTargetCode )
            {   // input
                return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_REDUCTION_PRODUCT );
            }
        }
    }
    
    if ( vCode->code(0).asOpcode() == r_exec::Opcodes::MkAntiRdx )
    {
        int vNumProducts = vCode->code(2).getAtomCount();
        if ( vCode->get_reference(0) == vTargetCode )
        {   // ipgm
            return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_REDUCTION_MEMBER );
        }
        
        for ( int i=0; i<vNumProducts; i++ )
        {
            if ( vCode->get_reference(1+i) == vTargetCode )
            {   // input
                return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_REDUCTION_PRODUCT );
            }
        }
    }    
    
    if ( vCode->code(0).asOpcode() == r_exec::Opcodes::Grp )
    {
        /*
        if ( vCode->get_reference(0) == vTargetCode )
        {   // code injection? TODO: Is it really the code injection?
            return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_CODE_INJECTION );
        }
        */
    }
    
    /*
    if ( vComponent->GetView() )
    {
        return Ogre::ColourValue( 1, 1, 0, 1 );
    }
    
    if ( vCode->code(0).asOpcode() == r_exec::Opcodes::Group )
    {
        return Ogre::ColourValue( 1, 0, 0, 1 );
    }
    */

    return cVisualizerReplicodeSettings::Get().GetColor( cVisualizerReplicodeSettings::COLOR_LINE_DEFAULT );
}

