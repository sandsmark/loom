#pragma once

#include <node.h>
#include <Core/Module/IModule.h>
#include <Speech/Event/cSpeechResponders.h>
using Loom::Core::IModule;

#include <windows.h>
#include <queue>

#include <Ogre/OgreString.h>
#include <Avatar/Avatar.h>	// Needed for eEffector
#include <Avatar/Event/IAvatarListenerEvent.h>

using	namespace	core;
using	namespace	mBrane;

class HD1Loom;

#ifdef WINDOWS
	#define DllExport   __declspec( dllexport )
#else // WINDOWS
	#define DllExport
#endif // WINDOWS

// API for external code
extern "C" {
	DllExport void ProcessPayload( _Payload* iPayload );
	DllExport void SetModule( mBrane::sdk::module::_Module *iModule );
}

BEGIN_NAMESPACE( Avatar )
	class cOgreAvatar;
END_NAMESPACE();

BEGIN_NAMESPACE( MBrane )

class MBRANE_API cModulemBrane : public IModule, public Speech::ISpeechListenerEvent, public Avatar::IAvatarListenerEvent
{
public:
	enum ePayload
	{
		PAYLOAD_SPEAK = 0,
		PAYLOAD_SAMPLE_STRING255,
		PAYLOAD_MOVE_TO,
		PAYLOAD_ONTOLOGY_DEF,
		PAYLOAD_VEC3,
		PAYLOAD_UINT32,
		PAYLOAD_ONTOLOGY_COUNT,
		PAYLOAD_GRAB,
		PAYLOAD_BONES,

		PAYLOAD_MAX,
	};
	enum iOIds
	{
		OID_SELF_LEFT_HAND = 0,
		OID_SELF_RIGHT_HAND,
		OID_POSITION,
		OID_COLOR,
		OID_ESSENCE,
		OID_HUMAN1_LEFT_HAND,
		OID_HUMAN1_RIGHT_HAND,
		OID_HUMAN2_LEFT_HAND,
		OID_HUMAN2_RIGHT_HAND,
		OID_ATTACHMENT,

		OID_MAX,
	};

public:
//	static HD1Loom *mModule;
	static mBrane::sdk::module::_Module *mModule;
	static core::uint32 OIds[ OID_MAX ];
	static uint32 mMaxNumOntologies;
	static uint32 mNumOntologies;

protected:
	HANDLE mThread;
	HANDLE mThreadListener;
	static core::uint16 ClassIds[ PAYLOAD_MAX ];

	uint16 mStartId;
	uint32 mNumSentVec;

	void SendSnapshot();
	void SendObject( const Ogre::String &iObjectName, const Ogre::String &iColor, const Ogre::String &iShape, uint16 iId );
	void SendEffector( const size_t iIndex, const eEffector iEffector );

public:
	cModulemBrane();
	~cModulemBrane();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
	static DWORD WINAPI StartListenerThread( LPVOID iParam );

	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );

	// IAvatarListenerEvent
	virtual void OnNotifyEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition );
	virtual void OnNotifyEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation );
	virtual void OnNotifyGrab( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName );
	virtual void OnNotifyRelease( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName );
	virtual void OnNotifyBones( const size_t iId, const sBone *iBones, const size_t iNumBones );

	static core::uint16 GetClassId( ePayload iPayload )
	{
		return ClassIds[ iPayload ];
	}
	mBrane::Node *mNode;
};

END_NAMESPACE()
