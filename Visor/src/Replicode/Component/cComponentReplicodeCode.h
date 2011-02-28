/*
 *  cComponentReplicodeCode.h
 *  Visor
 *
 *  Created by mb on 7/16/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Replicode/Component/cComponentReplicode.h>

namespace Visor
{
    class cComponentReplicodeCode : public cComponentReplicode
    {
    public:
        cComponentReplicodeCode( r_code::Code *iCode , const std::string &iSource );

        r_code::Code *GetCode( void ) const { return ( mType == TYPE_CODE ) ? mCode : NULL; }
        const std::string &GetSource( void ) const { return mSource; }
    };
}
