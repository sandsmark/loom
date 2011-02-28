/*
 *  cComponentReplicode.cpp
 *  Visor
 *
 *  Created by mb on 7/7/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cComponentReplicode.h"
#include <Replicode/r_code/object.h>
using namespace Loom::Visor;

/**********************************************************************/
cComponentReplicode::cComponentReplicode( r_code::Code *iCode, const std::string &iSource )
: mCode( iCode ), mSource( iSource ), mType( TYPE_CODE ), mView( NULL ), mNew( true )
/**********************************************************************/
{
    GetRegistry().OnComponentInitialized( this );
}

#if 0
/**********************************************************************/
cComponentReplicode::cComponentReplicode( r_exec::Group *iGroup, const std::string &iSource )
: mGroup( iGroup ), mSource( iSource ), mType( TYPE_GROUP ), mCode( NULL ), mNew( true )
/**********************************************************************/
{
    GetRegistry().OnComponentInitialized( this );
}
#endif

/**********************************************************************/
cComponentReplicode::cComponentReplicode( r_code::View *iView, bool iNew )
: mView( iView ), mSource( "" ), mType( TYPE_VIEW ), mCode( iView->object ), mNew( iNew )
/**********************************************************************/
{
    GetRegistry().OnComponentInitialized( this );    
}
