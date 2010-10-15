#pragma once

#include <Core/Singleton/ISingleton.h>
#include <Core/Datatype/cString.h>
#include <Core/Container/cArray.h>
#include <Core/Container/cHashMap.h>
#include <tchar.h>

using Loom::Core::cString;
using Loom::Core::cArray;
using Loom::Core::cHashMap;

BEGIN_NAMESPACE( Weaver )

class cDispatcherHub : public Loom::Core::ISingleton<cDispatcherHub>
{
public:
	class IParam
	{
	protected:
		void *mData;

	public:
		IParam( void *iData ) : mData( iData ) {}
		const void *GetData( void ) const { return mData; }
	};

public:
	class IResponder
	{
	public:
		cString mEventName;

	public:
		IResponder( const cString &iEventName )
		: mEventName( iEventName )
		{
			cDispatcherHub::Get().Register( mEventName, this );
		}

		virtual void Respond( const IParam &iParam ) = 0;
		const cString &GetEventName( void ) const { return mEventName; }
	};

protected:
	cHashMap<const cString, cArray<IResponder*> > mResponders;

public:
	void Register( const cString &iName, IResponder *iResponder );
	void Dispatch( const cString &iName, const IParam &iParam );

	static const cString &GetLoggerName()
	{
		static const cString vLogger( _T("DispatcherHub") );
		return vLogger;
	}

	void Init( void );
	void Destroy( void );
};

END_NAMESPACE()