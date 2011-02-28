/*
 *  cComponentReplicode.h
 *  Visor
 *
 *  Created by mb on 7/7/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <Visor/Component/IComponent.h>

namespace r_code
{
    class Code;
    class View;
};
namespace r_exec
{
    class Group;
};

BEGIN_NAMESPACE( Visor )

class cComponentReplicode : public IComponent
{
public:
    enum eType
    {
        TYPE_CODE,
        TYPE_GROUP,
        TYPE_VIEW,
    };
    
protected:
    union 
    {
        r_exec::Group *mGroup;
        r_code::View  *mView;
    };        
    r_code::Code  *mCode;
    eType mType;
    
    std::string  mSource;
    bool mNew;
    
public:
    // TODO: Get source code from r_code::Code
    cComponentReplicode( r_code::Code *iCode , const std::string &iSource );
//        cComponentReplicode( r_exec::Group *iCode, const std::string &iSource );
    cComponentReplicode( r_code::View *iCode, bool iNew );
    
    r_code::Code *GetCode( void ) const { return mCode; }
    const std::string &GetSource( void ) const { return mSource; }
    
    r_code::View *GetView( void ) const { return mView; }
    
    const eType GetType( void ) const { return mType; }
    
    const bool IsNew( void ) const { return mNew; }
            
    
};

END_NAMESPACE()
