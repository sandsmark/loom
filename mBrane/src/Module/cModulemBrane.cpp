#include <mBrane/Module/cModulemBrane.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <HumanobsD1/r_mem_class.h>
// #include	"../Core/payload_utils.h"

#include <Speech/Event/Responders/cSpeechResponderSay.h>
//#include <HumanobsD1/loom_module.h>

#include <OgreApp/OgreApp.h>
#include <Avatar/Event/Responders/cAvatarResponderSetEffectorPosition.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Avatar/Module/cModuleAvatar.h>
#include <Avatar/Ogre/cOgreAvatar.h>
#include <Avatar/Event/Responders/cAvatarResponderListenerGrab.h>
#include <Avatar/Event/Responders/cAvatarResponderListenerRelease.h>
#include <Ogre/OgreSceneNode.h>
#include <Ogre/OgreEntity.h>
#include <Avatar/Event/Responders/cAvatarResponderNotifyPosition.h>
#include <Avatar/Event/IAvatarListenerEvent.h>
#include <Avatar/Event/Responders/cAvatarResponderNotifyBones.h>
#include <Ogre/OgreSkeletonInstance.h>
#include <Ogre/OgreBone.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::Speech::cSpeechResponderSay;
using Loom::Speech::ISpeechEvent;
using Loom::Avatar::cAvatarResponderSetEffectorPosition;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Core::cModuleManager;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Avatar::cModuleAvatar;
using Loom::Avatar::cOgreAvatar;
using Loom::Avatar::cAvatarResponderListenerGrab;
using Loom::Avatar::cAvatarResponderListenerRelease;
using Loom::Avatar::cAvatarResponderListenerNotifyPosition;
using Loom::Avatar::IAvatarListenerEvent;
using Loom::Avatar::cAvatarResponderListenerNotifyBones;

SharedLibrary	SL;	//	will be initialized with the library loaded by the node; this ensures SL is deleted last, and thus that all user-defined adresses (e.g. __vfptr) are still valid until the very end

using namespace Loom::MBrane;

core::uint16 cModulemBrane::ClassIds[ PAYLOAD_MAX ];
core::uint32 cModulemBrane::OIds[ OID_MAX ] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
//HD1Loom *cModulemBrane::mModule = NULL;
mBrane::sdk::module::_Module *cModulemBrane::mModule = NULL;
core::uint32 cModulemBrane::mMaxNumOntologies = 0xffff;
core::uint32 cModulemBrane::mNumOntologies = 0;

void ProcessOntologyDef( _Payload* iPayload );
void ProcessVec3Payload( _Payload* iPayload );

/************************************************************************/
void ProcessBonesPayload( _Payload* iPayload ) 
/************************************************************************/
{
	Bones *vInput = (Bones*)iPayload;

	IAvatarListenerEvent::sBone *vBones = new IAvatarListenerEvent::sBone[ vInput->NumBones ];
	for ( size_t i=0; i<vInput->NumBones; i++ )
	{
		vBones[i].Position.x    = vInput->Positions[i][0];
		vBones[i].Position.y    = vInput->Positions[i][1];
		vBones[i].Position.z    = vInput->Positions[i][2];
		vBones[i].Orientation.x = vInput->Orientations[i][0];
		vBones[i].Orientation.y = vInput->Orientations[i][1];
		vBones[i].Orientation.z = vInput->Orientations[i][2];
		vBones[i].Orientation.w = vInput->Orientations[i][3];
	}

	cAvatarResponderListenerNotifyBones::cParam vParam;
	vParam.Bones = vBones;
	vParam.Id = vInput->Id;
	vParam.NumBones = vInput->NumBones;

	cDispatcherHub::IParam vReceivedParam( (void*)&vParam );
	cDispatcherHub::Get().Dispatch( cAvatarResponderListenerNotifyBones::Get().GetEventName(), vReceivedParam );
}

/************************************************************************/
DllExport void ProcessPayload( _Payload* iPayload )
/************************************************************************/
{
	int a = 0;
	if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_ONTOLOGY_COUNT ) )
	{
		OntologyCount *vCount = (OntologyCount*)iPayload;
		cModulemBrane::mMaxNumOntologies = vCount->count;

	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_ONTOLOGY_DEF ) )
	{
		ProcessOntologyDef(iPayload);
	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_SPEAK ) )
	{
		Speak *vSpeak = (Speak*)iPayload;
		wchar_t vTemp[ 256 ];
		mbstowcs_s( NULL, vTemp, 256, vSpeak->word, 256 );
//		cSpeechResponderSay::Get().Dispatch( &ISpeechEvent::OnSay, std::wstring( vTemp ) );

		cDispatcherHub::IParam vReceivedParam( (void*)vTemp );
		cDispatcherHub::Get().Dispatch( _T("Speech::Say"), vReceivedParam );
	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_MOVE_TO ) )
	{
		MoveTo *vMoveTo = (MoveTo*)iPayload;
		
		eEffector vEffector;
		if ( vMoveTo->OID == cModulemBrane::OIds[ cModulemBrane::OID_SELF_LEFT_HAND ] )
		{
			vEffector = EFFECTOR_LEFT_HAND;
		}
		else if ( vMoveTo->OID == cModulemBrane::OIds[ cModulemBrane::OID_SELF_RIGHT_HAND ] )
		{
			vEffector = EFFECTOR_RIGHT_HAND;
		}

		cAvatarResponderSetEffectorPosition::cParam vSetPosParam;
		vSetPosParam.Id = 2;
		vSetPosParam.Effector = vEffector;
		vSetPosParam.Position.x = vMoveTo->target_position[0];
		vSetPosParam.Position.y = vMoveTo->target_position[1];
		vSetPosParam.Position.z = vMoveTo->target_position[2];

		cDispatcherHub::IParam vReceivedParam( (void*)&vSetPosParam );
		cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderSetPosition" ), vReceivedParam );
	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_GRAB ) )
	{
		Grab *vGrab = (Grab*)iPayload;

		eEffector vEffector;
		if ( vGrab->OID == cModulemBrane::OIds[ cModulemBrane::OID_SELF_LEFT_HAND ] )
		{
			vEffector = EFFECTOR_LEFT_HAND;
		}
		else if ( vGrab->OID == cModulemBrane::OIds[ cModulemBrane::OID_SELF_RIGHT_HAND ] )
		{
			vEffector = EFFECTOR_RIGHT_HAND;
		}

		cAvatarResponderSetEffectorPosition::cParam vGrabParam;
		vGrabParam.Id = 2;
		vGrabParam.Effector = vEffector;

		cDispatcherHub::IParam vReceivedParam( (void*)&vGrabParam );
		cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderGrab" ), vReceivedParam );
	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_VEC3 ) )
	{
		ProcessVec3Payload(iPayload);
	}
	else if ( iPayload->cid() == cModulemBrane::GetClassId( cModulemBrane::PAYLOAD_BONES ) )
	{
		ProcessBonesPayload(iPayload);
	}
}

/************************************************************************/
//DllExport void SetModule( HD1Loom *iModule )
DllExport void SetModule( mBrane::sdk::module::_Module *iModule )
/************************************************************************/
{
	cModulemBrane::mModule = iModule;
}

/************************************************************************/
void ProcessOntologyDef( _Payload* iPayload )
/************************************************************************/
{
	OntologyDef *vOntology = (OntologyDef*)iPayload;
	if ( strcmp( vOntology->name, "self_left_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_SELF_LEFT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "self_right_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_SELF_RIGHT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "human1_left_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_HUMAN1_LEFT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "human1_right_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_HUMAN1_RIGHT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "human2_left_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_HUMAN2_LEFT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "human2_right_hand" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_HUMAN2_RIGHT_HAND ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "position" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_POSITION ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "color" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_COLOR ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "essence" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_ESSENCE ] = vOntology->OID;
	}
	else if ( strcmp( vOntology->name, "attachment" ) == 0 )
	{
		cModulemBrane::OIds[ cModulemBrane::OID_ATTACHMENT ] = vOntology->OID;
	}
	cModulemBrane::mNumOntologies++;
}

/************************************************************************/
void ProcessVec3Payload( _Payload* iPayload )
/************************************************************************/
{
	Sample_Vec3 *vVector = (Sample_Vec3*)iPayload;

	size_t vId;
	eEffector vEffector;
	if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_HUMAN1_LEFT_HAND ] )
	{
		vId = 0;
		vEffector = EFFECTOR_LEFT_HAND;
	}
	else if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_HUMAN1_RIGHT_HAND ] )
	{
		vId = 0;
		vEffector = EFFECTOR_RIGHT_HAND;
	}
	else if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_HUMAN2_LEFT_HAND ] )
	{
		vId = 1;
		vEffector = EFFECTOR_LEFT_HAND;
	}
	else if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_HUMAN2_RIGHT_HAND ] )
	{
		vId = 1;
		vEffector = EFFECTOR_RIGHT_HAND;
	}
	else if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_SELF_LEFT_HAND ] )
	{
		vId = 2;
		vEffector = EFFECTOR_LEFT_HAND;
	}
	else if ( vVector->object == cModulemBrane::OIds[ cModulemBrane::OID_SELF_RIGHT_HAND ] )
	{
		vId = 2;
		vEffector = EFFECTOR_RIGHT_HAND;
	}

	cAvatarResponderListenerNotifyPosition::cParam vSetPosParam;
	vSetPosParam.Id = vId;
	vSetPosParam.Effector = vEffector;
	vSetPosParam.Position.x = vVector->value[0];
	vSetPosParam.Position.y = vVector->value[1];
	vSetPosParam.Position.z = vVector->value[2];

	cDispatcherHub::IParam vReceivedParam( (void*)&vSetPosParam );
	cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderNotifyPosition" ), vReceivedParam );
//	cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderSetPosition" ), vReceivedParam );
}

/************************************************************************/
cModulemBrane::cModulemBrane()
: IModule( _T( "mBrane" ) ), mStartId( 0x7fff )
/************************************************************************/
{
	mDependencies.Add( _T( "Avatar" ) );
	mNode = NULL;
}

/************************************************************************/
cModulemBrane::~cModulemBrane()
/************************************************************************/
{
	if ( mNode ) {
		mNode->shutdown();
		delete mNode;
		mNode = NULL;
	}
}

/************************************************************************/
void cModulemBrane::Init( void )
/************************************************************************/
{
	Sleep( 1000 );
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModulemBrane startup" ) );

	Speech::cSpeechResponderHeard::Get().AddListener( *this );
	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	mThreadListener = CreateThread( NULL, 0, StartListenerThread, this, 0, NULL );
	
	cAvatarResponderListenerGrab::Get().AddListener( *this );
	cAvatarResponderListenerRelease::Get().AddListener( *this );

	mNumSentVec = 0;

	mInitialized = true;
}

/************************************************************************/
void cModulemBrane::OnSay( const std::wstring& )
/************************************************************************/
{
}

/************************************************************************/
void cModulemBrane::OnHeard( const std::wstring &text )
/************************************************************************/
{
	if ( !mModule ) return;

	/*
	char vTemp[ 256 ];
	wcstombs_s( NULL, vTemp, 255, text.c_str(), 256 );
	mModule->on_heard( vTemp );
	delete vTemp;
	*/

	OutputDebugStringA( "Sending text\n" );
	Sample_String255 *vText = new Sample_String255;
	vText->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_SAMPLE_STRING255 ] << 16 );
	wcstombs_s( NULL, vText->value, 255, text.c_str(), 256 );
//	vText->_Metadata = cModulemBrane::ClassIds[ PAYLOAD_SAMPLE_STRING255 ];
	mNode->send( mModule, vText, module::Node::PRIMARY );
}


/************************************************************************/
DWORD cModulemBrane::StartThread( LPVOID arg )
/************************************************************************/
{
	if (!arg)
		return 0;

	cModulemBrane *_this = (cModulemBrane*) arg;

	LoadControlMessageMetaData();
	Time::Init(1000);

	char configfile[] = "mBrane.xml";
	int verboseLevel = 7;

	_this->mNode = mBrane::Node::New(configfile,SL,verboseLevel);
	uint16 vId = ClassRegister::GetCID( "Speak" );
	cModulemBrane::ClassIds[ PAYLOAD_SPEAK ] = ClassRegister::GetCID( "Speak" );
	cModulemBrane::ClassIds[ PAYLOAD_SAMPLE_STRING255 ] = ClassRegister::GetCID( "Sample_String255" );
	cModulemBrane::ClassIds[ PAYLOAD_MOVE_TO ] = ClassRegister::GetCID( "MoveTo" );
	cModulemBrane::ClassIds[ PAYLOAD_ONTOLOGY_DEF ] = ClassRegister::GetCID( "OntologyDef" );
	cModulemBrane::ClassIds[ PAYLOAD_VEC3 ] = ClassRegister::GetCID( "Sample_Vec3" );
	cModulemBrane::ClassIds[ PAYLOAD_UINT32 ] = ClassRegister::GetCID( "Sample_uint32" );
	cModulemBrane::ClassIds[ PAYLOAD_ONTOLOGY_COUNT ] = ClassRegister::GetCID( "OntologyCount" );
	cModulemBrane::ClassIds[ PAYLOAD_GRAB ] = ClassRegister::GetCID( "Grab" );
	cModulemBrane::ClassIds[ PAYLOAD_BONES ] = ClassRegister::GetCID( "Bones" );

	Speak::_MetaData            = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_SPEAK ] << 16 );
	Sample_String255::_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_SAMPLE_STRING255 ] << 16 );
	MoveTo::_MetaData           = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_MOVE_TO ] << 16 );
	OntologyDef::_MetaData      = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_ONTOLOGY_DEF ] << 16 );
	Sample_Vec3::_MetaData      = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_VEC3 ] << 16 );
	Sample_uint32::_MetaData    = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_UINT32 ] << 16 );
	OntologyCount::_MetaData    = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_ONTOLOGY_COUNT ] << 16 );
	Grab::_MetaData             = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_GRAB ] << 16 );

	//_this->node=mBrane::Node::NewXML(configXML,SL,verboseLevel);

	// We could not initialise everything, bailing out
	if ( !_this->mNode )
	{
		return 1;
	}

	std::wstring text = _T("mBrane is running...");
	cDispatcherHub::IParam vReceivedParam( (void*)text.c_str() );
	const TCHAR *vReceivedMessageName = _T("Speech::Say");
	cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );

	// std::cout<<"Running\n";

	_this->mNode->run();

	_this->mNode->shutdown();
	SAFE_DELETE( _this->mNode );

	return 0;
}

/************************************************************************/
void cModulemBrane::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );
	TerminateThread( mThreadListener, 0 );

	mInitialized = false;
}

/************************************************************************/
DWORD WINAPI Loom::MBrane::cModulemBrane::StartListenerThread( LPVOID arg )
/************************************************************************/
{
	if (!arg)
		return 0;

	cModulemBrane *_this = (cModulemBrane*) arg;

#if 0
	while ( mNumOntologies < mMaxNumOntologies )
	{
		Sleep( 100 );
	}
#else
	Sleep( 15 * 1000 );
#endif

	printf( "START!" );

	while ( _this->mNode->isRunning() )
	{
		Sleep( 500 );
//		Sleep( 50 );	// 20 fps
		_this->SendSnapshot();
	}
}

/************************************************************************/
void Loom::MBrane::cModulemBrane::SendSnapshot()
/************************************************************************/
{
	if ( !mModule ) return;

	printf( "Sending world snapshot\n" );
	SendObject( Ogre::String( "Blue cube 1" ), Ogre::String( "blue" ), Ogre::String( "cube" ), 0 );
	SendObject( Ogre::String( "Blue cube 2" ), Ogre::String( "blue" ), Ogre::String( "cube" ), 1 );
	SendObject( Ogre::String( "Red cube" ), Ogre::String( "red" ), Ogre::String( "cube" ), 2 );
	SendObject( Ogre::String( "Blue sphere" ), Ogre::String( "blue" ), Ogre::String( "sphere" ), 3 );
	SendObject( Ogre::String( "Red sphere" ), Ogre::String( "red" ), Ogre::String( "sphere" ), 4 );

	SendEffector( 0, EFFECTOR_LEFT_HAND );
	SendEffector( 0, EFFECTOR_RIGHT_HAND );
	SendEffector( 1, EFFECTOR_LEFT_HAND );
	SendEffector( 1, EFFECTOR_RIGHT_HAND );

	cModuleAvatar *vAvatarModule = (cModuleAvatar*)cModuleManager::Get().GetModule( _T("Avatar") );
	for ( size_t i=0; i<2; i++ )
	{
		cOgreAvatar *vAvatar = vAvatarModule->GetAvatar( i );
		if ( !vAvatar ) continue;
		if ( !vAvatar->IsInteractive() ) continue;

		Ogre::SkeletonInstance *vSkeleton = vAvatar->GetSkeleton();

		Bones *vBones = new Bones;
		vBones->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_BONES ] << 16 );
		vBones->Id = vAvatar->GetId();
		vBones->NumBones = vSkeleton->getNumBones();
		for ( size_t b=0; b<vBones->NumBones; b++ )
		{
			Ogre::Bone *vBone = vSkeleton->getBone( b );
			vBones->Positions[b][0] = vBone->getPosition().x;
			vBones->Positions[b][1] = vBone->getPosition().y;
			vBones->Positions[b][2] = vBone->getPosition().z;
			vBones->Orientations[b][0] = vBone->getOrientation().x;
			vBones->Orientations[b][1] = vBone->getOrientation().y;
			vBones->Orientations[b][2] = vBone->getOrientation().z;
			vBones->Orientations[b][3] = vBone->getOrientation().w;
		}
		mNode->send( mModule, vBones, module::Node::PRIMARY );
	}
}

/************************************************************************/
void Loom::MBrane::cModulemBrane::SendObject( const Ogre::String &iObjectName, const Ogre::String &iColor, const Ogre::String &iShape, uint16 iId )
/************************************************************************/
{
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();

	Ogre::Vector3 vPos;
	vQScene->OnGetPosition( iObjectName, vPos );

	Sample_Vec3 *vSamplePos = new Sample_Vec3;
	vSamplePos->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_VEC3 ] << 16 );
	vSamplePos->object = mStartId + iId;
	vSamplePos->attribute = OIds[ OID_POSITION ];
	vSamplePos->value[0] = vPos.x;
	vSamplePos->value[1] = vPos.y;
	vSamplePos->value[2] = vPos.z;
	mNode->send( mModule, vSamplePos, module::Node::PRIMARY );
	mNumSentVec++;

	Sample_String255 *vSampleText = new Sample_String255;
	vSampleText->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_SAMPLE_STRING255 ] << 16 );
	vSampleText->object = mStartId + 0;
	vSampleText->attribute = OIds[ OID_COLOR ];
	strcpy_s( vSampleText->value, 255, iColor.c_str() );
	mNode->send( mModule, vSampleText, module::Node::PRIMARY );

	vSampleText->object = mStartId + 0;
	vSampleText->attribute = OIds[ OID_ESSENCE ];
	strcpy_s( vSampleText->value, 255, iShape.c_str() );
	mNode->send( mModule, vSampleText, module::Node::PRIMARY );
}

/************************************************************************/
void Loom::MBrane::cModulemBrane::SendEffector( const size_t iIndex, const eEffector iEffector )
/************************************************************************/
{
	cModuleAvatar *vAvatarModule = (cModuleAvatar*)cModuleManager::Get().GetModule( _T("Avatar") );
	/*
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	*/

	cOgreAvatar *vAvatar = vAvatarModule->GetAvatar( iIndex );
	Ogre::Vector3 vPos = vAvatar->GetWorldEffectorPosition( iEffector );
	size_t vId = vAvatar->GetId();

	Sample_Vec3 *vSamplePos = new Sample_Vec3;
	vSamplePos->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_VEC3 ] << 16 );
	switch ( iIndex )
	{
	case 0:	// Human
		switch ( iEffector )
		{
		case EFFECTOR_LEFT_HAND:
			vSamplePos->object = OIds[ OID_HUMAN1_LEFT_HAND ];
			break;
		case EFFECTOR_RIGHT_HAND:
			vSamplePos->object = OIds[ OID_HUMAN1_RIGHT_HAND ];
			break;
		}
		break;
	case 1:	// Human 2
		switch ( iEffector )
		{
		case EFFECTOR_LEFT_HAND:
			vSamplePos->object = OIds[ OID_HUMAN2_LEFT_HAND ];
			break;
		case EFFECTOR_RIGHT_HAND:
			vSamplePos->object = OIds[ OID_HUMAN2_RIGHT_HAND ];
			break;
		}
		break;
	case 2:	// Self
		switch ( iEffector )
		{
		case EFFECTOR_LEFT_HAND:
			vSamplePos->object = OIds[ OID_SELF_LEFT_HAND ];
			break;
		case EFFECTOR_RIGHT_HAND:
			vSamplePos->object = OIds[ OID_SELF_RIGHT_HAND ];
			break;
		}
		break;
	}
	vSamplePos->attribute = OIds[ OID_POSITION ];
	vSamplePos->value[0] = vPos.x;
	vSamplePos->value[1] = vPos.y;
	vSamplePos->value[2] = vPos.z;
	mNode->send( mModule, vSamplePos, module::Node::PRIMARY );
	mNumSentVec++;
}

/************************************************************************/
void Loom::MBrane::cModulemBrane::OnNotifyGrab( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName )
/************************************************************************/
{
	if ( !mModule ) return;

	{
		Sample_uint32 *vSampleGrab = new Sample_uint32;
		vSampleGrab->value = 0;
		vSampleGrab->attribute = 11;
		vSampleGrab->object = 0;
		mNode->send( mModule, vSampleGrab, module::Node::PRIMARY );
	}	

	cModuleAvatar *vAvatarModule = (cModuleAvatar*)cModuleManager::Get().GetModule( _T("Avatar") );
	cOgreAvatar *vAvatar = vAvatarModule->GetAvatarById( iId );
	if ( !vAvatar )
	{
		return;
	}

	Sample_uint32 *vSampleGrab = new Sample_uint32;
	vSampleGrab->_MetaData = 0x00FFFFFF00000000 + ( cModulemBrane::ClassIds[ PAYLOAD_UINT32 ] << 16 );
	int iIndex = -1;
	if ( vAvatar->GetEntity()->getName() == "Avatar1" )
	{
		iIndex = 0;
	}
	else if ( vAvatar->GetEntity()->getName() == "Avatar2" )
	{
		iIndex = 1;
	}
	switch ( iIndex )
	{
	case 0:	// Human
		switch ( iEffector )
		{
		case EFFECTOR_LEFT_HAND:
			vSampleGrab->object = OIds[ OID_HUMAN1_LEFT_HAND ];
			break;
		case EFFECTOR_RIGHT_HAND:
			vSampleGrab->object = OIds[ OID_HUMAN1_RIGHT_HAND ];
			break;
		}
		break;
	case 1:	// Self
		switch ( iEffector )
		{
		case EFFECTOR_LEFT_HAND:
			vSampleGrab->object = OIds[ OID_SELF_LEFT_HAND ];
			break;
		case EFFECTOR_LEFT_GRAB:
			vSampleGrab->object = OIds[ OID_SELF_LEFT_HAND ];
			break;
		case EFFECTOR_RIGHT_HAND:
			vSampleGrab->object = OIds[ OID_SELF_RIGHT_HAND ];
			break;
		case EFFECTOR_RIGHT_GRAB:
			vSampleGrab->object = OIds[ OID_SELF_RIGHT_HAND ];
			break;
		}
		break;
	}
	vSampleGrab->attribute = OIds[ OID_ATTACHMENT ];
	if ( iEntityName == "Blue cube 1" ) vSampleGrab->value = mStartId + 0;
	if ( iEntityName == "Blue cube 2" ) vSampleGrab->value = mStartId + 1;
	if ( iEntityName == "Red cube" ) vSampleGrab->value = mStartId + 2;
	if ( iEntityName == "Blue sphere" ) vSampleGrab->value = mStartId + 3;
	if ( iEntityName == "Red sphere" ) vSampleGrab->value = mStartId + 4;
	mNode->send( mModule, vSampleGrab, module::Node::PRIMARY );
}

/************************************************************************/
void Loom::MBrane::cModulemBrane::OnNotifyRelease( const size_t iId, const eEffector iEffector, const Ogre::String &iEntityName )
/************************************************************************/
{

}

/************************************************************************/
void Loom::MBrane::cModulemBrane::OnNotifyEffectorPosition( const size_t iId, const eEffector iEffector, const Ogre::Vector3 &iPosition )
/************************************************************************/
{

}

/************************************************************************/
void Loom::MBrane::cModulemBrane::OnNotifyEffectorRotation( const size_t iId, const eEffector iEffector, const Ogre::Quaternion &iRotation )
/************************************************************************/
{

}

/************************************************************************/
void Loom::MBrane::cModulemBrane::OnNotifyBones( const size_t iId, const sBone *iBones, const size_t iNumBones )
/************************************************************************/
{

}
