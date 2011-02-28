/*
 *  cVisualizerReplicodeSettings.cpp
 *  Visor
 *
 *  Created by mb on 9/21/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include "cVisualizerReplicodeSettings.h"
using namespace Loom::Visor;

cVisualizerReplicodeSettings cVisualizerReplicodeSettings::mInstance;

/**********************************************************************/
cVisualizerReplicodeSettings::cVisualizerReplicodeSettings()
/**********************************************************************/
{
    mColors[ COLOR_NODE_DEFAULT           ] = Ogre::ColourValue(    1,    1,    1 );
    mColors[ COLOR_LINE_DEFAULT           ] = Ogre::ColourValue(    1,    1,    1 );
    mColors[ COLOR_NODE_REDUCTION         ] = Ogre::ColourValue(    1,    0,    1 );
    mColors[ COLOR_LINE_REDUCTION_MEMBER  ] = Ogre::ColourValue(    0,    0,    1 );
    mColors[ COLOR_LINE_REDUCTION_INPUT   ] = Ogre::ColourValue(    0,    1,    0 );
    mColors[ COLOR_LINE_REDUCTION_PRODUCT ] = Ogre::ColourValue(    1,    1,    0 );
    mColors[ COLOR_LINE_CODE_INJECTION    ] = Ogre::ColourValue(    1,    0,    1 );
    mColors[ COLOR_GROUP_VISIBILITY       ] = Ogre::ColourValue(    0, 0.5f,    1 );
    mColors[ COLOR_SALIENCY               ] = Ogre::ColourValue(    0,    1,    1 );
    mColors[ COLOR_ACTIVATION             ] = Ogre::ColourValue(    1, 0.5f,    0 );
    mColors[ COLOR_VISOR_BACKGROUND       ] = Ogre::ColourValue( 0.5f, 0.5f, 0.5f );
}

/**********************************************************************/
void cVisualizerReplicodeSettings::BeginGroup( QSettings &iSettings )
/**********************************************************************/
{
    iSettings.beginGroup( "VisualizerReplicodeSettings" );
}

/**********************************************************************/
void cVisualizerReplicodeSettings::EndGroup( QSettings &iSettings )
/**********************************************************************/
{
    iSettings.endGroup();
}

/**********************************************************************/
void cVisualizerReplicodeSettings::Init( QSettings &iSettings )
/**********************************************************************/
{
    BeginGroup( iSettings );
    
    int vVersion = iSettings.value( "Version", cVisualizerReplicodeSettings::VERSION ).toInt();
    ASSERT( cVisualizerReplicodeSettings::VERSION >= vVersion ); //, "Unknown version" );
    
    if ( vVersion > 2 )
    {
        InitColorArray( iSettings, "Colors", mColors, COLOR_MAX );
    }
    
    EndGroup( iSettings );
}

/**********************************************************************/
void cVisualizerReplicodeSettings::Save( QSettings &iSettings )
/**********************************************************************/
{
    BeginGroup( iSettings );
    
    iSettings.setValue( "Version", cVisualizerReplicodeSettings::VERSION );
    SaveColorArray( iSettings, "Colors", mColors, COLOR_MAX );
    
    EndGroup( iSettings );
}

/**********************************************************************/
void cVisualizerReplicodeSettings::InitColorArray( QSettings &iSettings, const char *iName, Ogre::ColourValue *iArray, int iNumElements )
/**********************************************************************/
{
    for ( size_t i=0; i<iNumElements; i++ )
    {
        char Temp[ 256 ];
        sprintf( Temp, "%s[%d].r", iName, (int)i );
        iArray[i].r = iSettings.value( Temp, iArray[i].r ).toFloat();
        sprintf( Temp, "%s[%d].g", iName, (int)i );
        iArray[i].g = iSettings.value( Temp, iArray[i].g ).toFloat();
        sprintf( Temp, "%s[%d].b", iName, (int)i );
        iArray[i].b = iSettings.value( Temp, iArray[i].b ).toFloat();
    }}

/**********************************************************************/
void cVisualizerReplicodeSettings::SaveColorArray( QSettings &iSettings, const char *iName, Ogre::ColourValue *iArray, int iNumElements )
/**********************************************************************/
{
    for ( size_t i=0; i<iNumElements; i++ )
    {
        char Temp[ 256 ];
        sprintf( Temp, "%s[%d].r", iName, (int)i );
        iSettings.setValue( Temp, iArray[i].r );
        sprintf( Temp, "%s[%d].g", iName, (int)i );
        iSettings.setValue( Temp, iArray[i].g );
        sprintf( Temp, "%s[%d].b", iName, (int)i );
        iSettings.setValue( Temp, iArray[i].b );
    }
}
