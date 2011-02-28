/*
 *  cComponentRegistry.h
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Core/Event/IEventDispatcher.h>

BEGIN_NAMESPACE( Visor )

class IComponent;
class IListenerComponent;

class cComponentRegistry : public Loom::Core::IEventDispatcher< IListenerComponent >
{
public:
    void      RegisterComponent( IComponent *iComponent );
    void    DeregisterComponent( IComponent *iComponent );
    void OnComponentInitialized( IComponent *iComponent );
};    

END_NAMESPACE()
