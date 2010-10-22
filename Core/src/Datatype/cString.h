#pragma once

#include <string>
#include <xhash>

#include <tchar.h>

BEGIN_NAMESPACE( Core )

class cString
{
protected:
	TCHAR *mData;

public:
	cString( const TCHAR *iString )
	{
		size_t vSize = _tcslen( iString ) + 1;
		mData = new TCHAR[ vSize ];
		_tcscpy_s( mData, vSize, iString );
	}

//	operator const TCHAR*() const { return mData; }

	const TCHAR *ToCString( void ) const { return mData; }
	const size_t GetLength( void ) const { return _tcslen( mData ); }

	cString &operator +=( const cString &iOther )
	{
		size_t vSize = GetLength() + iOther.GetLength() + 1;
		TCHAR *vNewData = new TCHAR[ vSize ];
		_tcscpy_s( vNewData, vSize, mData );
		_tcscat_s( vNewData, vSize, iOther.mData );
		delete [] mData;
		mData = vNewData;

		return *this;
	}
};

END_NAMESPACE()


// Define hashing
template<>
inline size_t stdext::hash_value<Loom::Core::cString>( const Loom::Core::cString &iString )
{	// hash NTBS to size_t value
	return (_Hash_value( iString.ToCString(), iString.ToCString() + iString.GetLength() ) );
}

// Define less operator
template<>
struct std::less<const Loom::Core::cString>
	: public binary_function<const Loom::Core::cString, const Loom::Core::cString, bool>
{	// functor for operator<
	bool operator()(const Loom::Core::cString& _Left, const Loom::Core::cString& _Right) const
	{	// apply operator< to operands
		return ( _tcscmp(_Left.ToCString(), _Right.ToCString()) < 0 );
	}
};
