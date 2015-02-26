/*
 *  cListenerCreation.h
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <Event/IEventListener.h>

namespace Visor
{
    class IComponent;
    
    class cListenerCreation : public IEventListener
    {
    public:
        void OnComponentCreated( IComponent *iComponent );
        void OnComponentDestroyed( IComponent *iComponent );
    };
}
