#pragma once

BEGIN_NAMESPACE( Core )

template< typename iType >
class cMotion
{
protected:
	iType mValue;
	iType mTarget;
	float mSpeed;

public:
	cMotion() : mSpeed( 1 ) {}

	void SetSpeed( const float iSpeed )
	{
		mSpeed = iSpeed;
	}

	void Init( const iType &iValue )
	{
		mTarget = mValue = iValue;
	}

	void SetTarget( const iType &iTarget )
	{
//		mValue = mTarget;
		mTarget = iTarget;
	}

	void Update( const float iEllapsed )
	{
		iType vDiff = mTarget - mValue;
		float vAlpha = iEllapsed * mSpeed;
		if ( vAlpha > 1 ) vAlpha = 1;
		mValue += vDiff * vAlpha;
	}

	const iType &GetValue( void ) const { return mValue; }
	const iType &GetTarget( void ) const { return mTarget; }
};

END_NAMESPACE()
