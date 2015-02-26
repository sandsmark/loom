#pragma once

#include <hash_map>

BEGIN_NAMESPACE( Core )

template<typename cKey, typename cValue>
class cHashMap : public stdext::hash_map<cKey,cValue>
{
public:
	// WARNING: Costy
	const bool HasKey( const cKey &iKey ) const
	{
		return ( find( iKey ) != end() );
	}

	cValue *Find( const cKey &iKey )
	{
		iterator it = find( iKey );

		if ( it == end() ) return NULL;
		return &(it->second);
	}
};

END_NAMESPACE()
