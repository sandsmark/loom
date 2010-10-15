#pragma once

BEGIN_NAMESPACE( Core )

template< typename iType >
class cAutoPtr
{
protected:
	iType *mData;

public:
	cAutoPtr() : mData( NULL ) {}
	cAutoPtr( iType *iData ) : mData( iData ) {}
	cAutoPtr( const cAutoPtr &iOther )
	{
		FAILTXT( _T("AutoPtr can't be copied!") );
	}
	~cAutoPtr()
	{
		SAFE_DELETE( mData );
	}

	void Set( iType *iData )
	{
		ASSERTTXT( !mData, _T("AutoPtr is already set") );

		mData = iData;
	}

	iType* operator->() const { return mData; }
	operator iType*() const { return mData; }
};

END_NAMESPACE()
