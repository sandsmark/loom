#pragma once


#ifdef _WIN32
#include <tchar.h>
//	#define __PRETTY_FUNCTION__ __FUNCSIG__
	#define __PRETTY_FUNCTION__ __FUNCTION__
#else
    #define tchar_t wchar_t
#endif

#define ASSERT( iExpression )   \
	if ( !( iExpression ) ) abort();

#define ASSERT_STRINGIFY( iLine ) #iLine
#define ASSERT_TOSTRING( iLine ) ASSERT_STRINGIFY( iLine )
#define ASSERT_LINE ASSERT_TOSTRING( __LINE__ )

#define ASSERTTXT_L( iLogger, iExpression, iMessage )   \
	if ( !( iExpression ) ) \
	{   \
		Loom::Core::cLogger::Get().Log( Loom::Core::cLogger::LOG_ERROR, iLogger, ( _T("[") _T(__PRETTY_FUNCTION__) _T("] ") iMessage _T(" at line ")  _T(ASSERT_LINE) ) );    \
		if ( Loom::Core::cDebug::Alert( iMessage, _T(__PRETTY_FUNCTION__), _T(ASSERT_LINE) ) )    \
		{   \
			abort();    \
		}   \
	}

#define ASSERTTXT( iExpression, iMessage )   \
	ASSERTTXT_L( _T("Global"), iExpression, iMessage )

#define FAILTXT( iMessage )   \
	Loom::Core::cLogger::Get().Log( Loom::Core::cLogger::LOG_ERROR, _T("Global"), ( _T("[") _T(__PRETTY_FUNCTION__) _T("] ") iMessage _T(" at line ")  _T(ASSERT_LINE) ) );    \
	if ( Loom::Core::cDebug::Alert( iMessage, _T(__PRETTY_FUNCTION__), _T(ASSERT_LINE) ) )    \
	{   \
		abort();    \
	}

//if ( !iExpression ) { Loom::Core::cDebug::Alert( iMessage, "Assertion failed" ); abort(); }

#define SAFE_DELETE( iObject ) { delete iObject; iObject = NULL; }

BEGIN_NAMESPACE( Core )

class CORE_API cDebug
{
public:
	static bool Alert( const wchar_t *iMessage, const wchar_t *iHeader = _T("Alert") );
	static bool Alert( const wchar_t *iMessage, const wchar_t *iFunction, const wchar_t *iLine );

	static void Notify( const wchar_t *iMessage, const wchar_t *iHeader = _T("Notification") );
};

END_NAMESPACE()
