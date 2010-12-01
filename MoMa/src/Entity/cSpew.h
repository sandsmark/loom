#pragma once

#include <OgreApp/Qt/Ogre/Event/IOgreListenerEvent.h>
#include <Ogre/OgreMesh.h>
#include <Ogre/OgreVector2.h>
#include <Ogre/OgreVector3.h>
#include <Ogre/OgreVector4.h>

using Loom::OgreApp::IOgreListenerEvent;
using Loom::Core::cObject;

BEGIN_NAMESPACE( MoMa )

class cSpew : public IOgreListenerEvent
{
public:
	struct sNode
	{
		Ogre::Vector3 Position;
		Ogre::Vector3 Velocity;
		float PrevLength;
		float NextLength;
		float Length;
		float Time;
	};

protected:
	struct sVertex
	{
		Ogre::Vector3 Pos;
		Ogre::Vector3 Dir;
		Ogre::Vector4 Tex;
	};

protected:
	Ogre::MeshPtr mLines;
	Ogre::Vector3 mOrigin;
	Ogre::Vector3 mVelocity;
	bool mEmitting;

	DWORD mLastUpdate;

	static int mId;

	void CreateLines( void );
	void Draw( void );
	Ogre::Vector3 GetTurbulence( const Ogre::Vector3 &iPosition, float iPersistence = 0.6f );
	Ogre::Vector2 GetNoise( int iX, int iY, int iSeed );

public:	// TODO: For test only
	cArray<sNode> mPoints;

public:
	cSpew( const Ogre::Vector3 &iOrigin, const Ogre::Vector3 &iVelocity );

	void Update( void );
	void StopEmitting( void );

	// IOgreListenerEvent methods
	virtual void OnRender( void );

	// Tweak these!
	float mDecay;
	float mSpringStrength;
	float mTurbulenceStrength;
	float mTurbulenceScroll;
	float mTurbulenceScale;
};

END_NAMESPACE()
