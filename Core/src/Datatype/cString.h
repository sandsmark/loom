#pragma once

#include <string>
#include <xhash>

#include <tchar.h>

BEGIN_NAMESPACE( Core )

class cString
{
protected:
	wchar_t *mData;

public:
	cString( const wchar_t *iString )
	{
		size_t vSize = wcslen( iString ) + 1;
		mData = new wchar_t[ vSize ];
		wcscpy_s( mData, vSize, iString );
	}

//	operator const wchar_t*() const { return mData; }

	const wchar_t *ToCString( void ) const { return mData; }
	const size_t GetLength( void ) const { return wcslen( mData ); }

	cString &operator +=( const cString &iOther )
	{
		size_t vSize = GetLength() + iOther.GetLength() + 1;
		wchar_t *vNewData = new wchar_t[ vSize ];
		wcscpy_s( vNewData, vSize, mData );
		wcscat_s( vNewData, vSize, iOther.mData );
		delete [] mData;
		mData = vNewData;

		return *this;
	}

	bool operator ==( const cString &iOther ) const
	{
		return ( wcscmp( mData, iOther.mData ) == 0 );
	}

	bool operator !=( const cString &iOther ) const
	{
		return ( wcscmp( mData, iOther.mData ) != 0 );
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
		return ( wcscmp(_Left.ToCString(), _Right.ToCString()) < 0 );
	}
};
