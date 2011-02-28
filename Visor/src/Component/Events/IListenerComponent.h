/*
 *  IListenerComponent.h
 *  Visor
 *
 *  Created by mb on 5/31/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Core/Event/IEventListener.h>

BEGIN_NAMESPACE( Visor )

class IComponent;

class IListenerComponent : public Loom::Core::IEventListener
{
public:
    virtual void OnComponentCreated    ( IComponent *iComponent ) = 0;
    virtual void OnComponentDestroyed  ( IComponent *iComponent ) = 0;
    virtual void OnComponentInitialized( IComponent *iComponent ) = 0;
};

END_NAMESPACE()
