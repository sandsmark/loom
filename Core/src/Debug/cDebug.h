#pragma once

#include <tchar.h>

#ifdef _WIN32
//	#define __PRETTY_FUNCTION__ __FUNCSIG__
	#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define ASSERT( iExpression )   \
	if ( !iExpression ) abort();

#define ASSERT_STRINGIFY( iLine ) #iLine
#define ASSERT_TOSTRING( iLine ) ASSERT_STRINGIFY( iLine )
#define ASSERT_LINE ASSERT_TOSTRING( __LINE__ )

#define ASSERTTXT_L( iLogger, iExpression, iMessage )   \
	if ( !( iExpression ) ) \
	{   \
		cLogger::Get().Log( cLogger::LOG_ERROR, iLogger, ( _T("[") _T(__PRETTY_FUNCTION__) _T("] ") iMessage _T(" at line ")  _T(ASSERT_LINE) ) );    \
		if ( cDebug::Alert( iMessage, _T(__PRETTY_FUNCTION__), _T(ASSERT_LINE) ) )    \
		{   \
			abort();    \
		}   \
	}

#define ASSERTTXT( iExpression, iMessage )   \
	ASSERTTXT_L( _T("Global"), iExpression, iMessage )

#define FAILTXT( iMessage )   \
	cLogger::Get().Log( cLogger::LOG_ERROR, _T("Global"), ( _T("[") _T(__PRETTY_FUNCTION__) _T("] ") iMessage _T(" at line ")  _T(ASSERT_LINE) ) );    \
	if ( cDebug::Alert( iMessage, __PRETTY_FUNCTION__, ASSERT_LINE ) )    \
	{   \
		abort();    \
	}

//if ( !iExpression ) { cDebug::Alert( iMessage, "Assertion failed" ); abort(); }

#define SAFE_DELETE( iObject ) { delete iObject; iObject = NULL; }

BEGIN_NAMESPACE( Core )

class cDebug
{
public:
	static bool Alert( const TCHAR *iMessage, const TCHAR *iHeader = _T("Alert") );
	static bool Alert( const TCHAR *iMessage, const TCHAR *iFunction, const TCHAR *iLine );

	static void Notify( const TCHAR *iMessage, const TCHAR *iHeader = _T("Notification") );
};

END_NAMESPACE()
