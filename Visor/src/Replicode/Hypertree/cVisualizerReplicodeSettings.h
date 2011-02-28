/*
 *  cVisualizerReplicodeSettings.h
 *  Visor
 *
 *  Created by mb on 9/21/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Ogre/OgreColourValue.h>
#include <Qt/qsettings.h>

BEGIN_NAMESPACE( Visor )

class cVisualizerReplicodeSettings
{
public:
    enum eColor
    {
        COLOR_NODE_DEFAULT = 0,
        COLOR_LINE_DEFAULT,
        COLOR_NODE_REDUCTION,
        COLOR_LINE_REDUCTION_MEMBER,
        COLOR_LINE_REDUCTION_INPUT,
        COLOR_LINE_REDUCTION_PRODUCT,
        COLOR_LINE_CODE_INJECTION,
        COLOR_GROUP_VISIBILITY,
        COLOR_SALIENCY,
        COLOR_ACTIVATION,
        COLOR_VISOR_BACKGROUND,
        
        COLOR_MAX,
    };
    static const int VERSION = 3;
    
protected:
    static cVisualizerReplicodeSettings mInstance;
    
    Ogre::ColourValue mColors[ COLOR_MAX ];
    
    void BeginGroup( QSettings &iSettings );
    void EndGroup( QSettings &iSettings );
    
    void InitColorArray( QSettings &iSettings, const char *iName, Ogre::ColourValue *iArray, int iNumElements );
    void SaveColorArray( QSettings &iSettings, const char *iName, Ogre::ColourValue *iArray, int iNumElements );
    
public:
    cVisualizerReplicodeSettings();
    
    void Init( QSettings &iSettings );
    void Save( QSettings &iSettings );
    
    static cVisualizerReplicodeSettings &Get( void ) { return mInstance; }
    
    const Ogre::ColourValue &GetColor( eColor iType ) const { return mColors[ (int)iType ]; }
};

END_NAMESPACE()

