/*
 *  cDebug.cpp
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <windows.h>
#include <strsafe.h>
#include <Core/Debug/cDebug.h>
using namespace Loom::Core;

/**********************************************************************/
bool cDebug::Alert( const wchar_t *iMessage, const wchar_t *iHeader )
/**********************************************************************/
{
	return ( MessageBox( NULL, iMessage, iHeader, MB_OKCANCEL ) == IDOK );
}

/**********************************************************************/
bool cDebug::Alert( const wchar_t *iMessage, const wchar_t *iFunction, const wchar_t *iLine )
/**********************************************************************/
{
	int vMaxLength = _tcslen( iFunction ) + _tcslen( iLine ) + 7;
    wchar_t *vTemp = new wchar_t[ vMaxLength ];
	StringCchPrintf( vTemp, vMaxLength, _T("%s line %s"), iFunction, iLine );
    
    bool vResult = Alert( iMessage, vTemp );
    
    delete [] vTemp;
    
    return vResult;
}

/**********************************************************************/
void cDebug::Notify( const wchar_t *iMessage, const wchar_t *iHeader )
/**********************************************************************/
{
	MessageBox( NULL, iMessage, iHeader, MB_OK );
}

