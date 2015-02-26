#pragma once

#include <vector>

BEGIN_NAMESPACE( Core )

template<typename cType>
class cArray : public std::vector<cType>
{
public:
	const size_t GetSize( void ) const { return size(); }
	void Add( const cType &iElement ) { push_back( iElement ); }
};

END_NAMESPACE()
