#pragma once

BEGIN_NAMESPACE( Core )

template<class cType>
class ISingleton
{
protected:
	ISingleton() {}	// Singletons shouldn't be created by hand!

public:
	~ISingleton()
	{
//		ISingleton::Destroy( *(cType*)this );
	}

	// Have to implement this in the actual class!
//	static void Destroy( cType &iObject ) { iObject.Destroy(); }

	static cType &Get( void )
	{
		static cType vInstance;

		return vInstance;
	}
};


END_NAMESPACE()