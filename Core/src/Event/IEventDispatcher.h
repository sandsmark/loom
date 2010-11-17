/*
 *  IEventDispatcher.h
 *  Visor
 *
 *  Created by mb on 5/30/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma once

#include <Core/Container/cArray.h>

BEGIN_NAMESPACE( Core )

template< typename iType >
class IEventDispatcher
{
public:
	class cTypeWrapper0
	{
	public:
		typedef void(iType::*tFunction)();
	};

	template< typename iParamType1 >
	class cTypeWrapper1
	{
	public:
		typedef void(iType::*tFunction)( iParamType1 );
	};

	template< typename iParamType1, typename iParamType2 >
	class cTypeWrapper2
	{
	public:
		typedef void(iType::*tFunction)( iParamType1, iParamType2 );
	};

	template< typename iParamType1, typename iParamType2, typename iParamType3 >
	class cTypeWrapper3
	{
	public:
		typedef void(iType::*tFunction)( iParamType1, iParamType2, iParamType3 );
	};

protected:
    cArray<iType*> mListeners;
        
public:
    void AddListener( iType &iListener ) { mListeners.Add( &iListener ); }

	void Dispatch( typename cTypeWrapper0::tFunction iFunction )
	{
		for ( size_t i=0; i<mListeners.GetSize(); i++ )
		{
			iType *vListener = mListeners[i];
			(vListener->*iFunction)();
		}
	}

    template< typename iParamType1 >
    void Dispatch( typename cTypeWrapper1<iParamType1>::tFunction iFunction, iParamType1 vParam1 )
    {
        for ( size_t i=0; i<mListeners.GetSize(); i++ )
        {
            iType *vListener = mListeners[i];
            (vListener->*iFunction)( vParam1 );
        }
    }

	template< typename iParamType1 >
	void Dispatch( typename cTypeWrapper1<const iParamType1&>::tFunction iFunction, const iParamType1 &vParam1 )
	{
		for ( size_t i=0; i<mListeners.GetSize(); i++ )
		{
			iType *vListener = mListeners[i];
			(vListener->*iFunction)( vParam1 );
		}
	}

	template< typename iParamType1, typename iParamType2 >
	void Dispatch( typename cTypeWrapper2<const iParamType1,const iParamType2&>::tFunction iFunction, const iParamType1 vParam1, const iParamType2 &vParam2 )
	{
		for ( size_t i=0; i<mListeners.GetSize(); i++ )
		{
			iType *vListener = mListeners[i];
			(vListener->*iFunction)( vParam1, vParam2 );
		}
	}

	template< typename iParamType1, typename iParamType2 >
	void Dispatch( typename cTypeWrapper2<const iParamType1&,const iParamType2&>::tFunction iFunction, const iParamType1 &vParam1, const iParamType2 &vParam2 )
	{
		for ( size_t i=0; i<mListeners.GetSize(); i++ )
		{
			iType *vListener = mListeners[i];
			(vListener->*iFunction)( vParam1, vParam2 );
		}
	}

	template< typename iParamType1, typename iParamType2, typename iParamType3 >
	void Dispatch( typename cTypeWrapper3<const iParamType1&,const iParamType2&,const iParamType3&>::tFunction iFunction, const iParamType1 &vParam1, const iParamType2 &vParam2, const iParamType3 &vParam3 )
	{
		for ( size_t i=0; i<mListeners.GetSize(); i++ )
		{
			iType *vListener = mListeners[i];
			(vListener->*iFunction)( vParam1, vParam2, vParam3 );
		}
	}
};

END_NAMESPACE()

