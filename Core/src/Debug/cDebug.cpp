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
bool cDebug::Alert( const TCHAR *iMessage, const TCHAR *iHeader )
/**********************************************************************/
{
	return ( MessageBox( NULL, iMessage, iHeader, MB_OKCANCEL ) == IDOK );
}

/**********************************************************************/
bool cDebug::Alert( const TCHAR *iMessage, const TCHAR *iFunction, const TCHAR *iLine )
/**********************************************************************/
{
	int vMaxLength = _tcslen( iFunction ) + _tcslen( iLine ) + 7;
    TCHAR *vTemp = new TCHAR[ vMaxLength ];
	StringCchPrintf( vTemp, vMaxLength, _T("%s line %s"), iFunction, iLine );
    
    bool vResult = Alert( iMessage, vTemp );
    
    delete [] vTemp;
    
    return vResult;
}

/**********************************************************************/
void cDebug::Notify( const TCHAR *iMessage, const TCHAR *iHeader )
/**********************************************************************/
{
	MessageBox( NULL, iMessage, iHeader, MB_OK );
}

