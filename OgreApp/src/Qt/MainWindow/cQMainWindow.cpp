#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <OgreApp/Qt/Ogre/Event/cOgreResponders.h>
#include <Core/Serializer/cSerializerXML.h>
#include <fstream>

using namespace Loom::OgreApp;
using Loom::Core::cSerializerXML;

/************************************************************************/
cQMainWindow::cQMainWindow()
/************************************************************************/
{
	QSettings vSettings( QSettings::UserScope, "CMLabs", "OgreApp" );

	restoreGeometry( vSettings.value( "WindowGeometry" ).toByteArray() );
	restoreState   ( vSettings.value(    "WindowState" ).toByteArray() );

	mScene = new cQScene( this );
	setCentralWidget( mScene );

	mMenuBar = new QMenuBar( this );
	setMenuBar( mMenuBar );
	QMenu *vFileMenu = mMenuBar->addMenu( "&Events" );
	vFileMenu->addAction( "Change Background", this, SLOT( OnSetBgColour() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_1 ) );
	vFileMenu->addAction( "Create Box", this, SLOT( OnCreateBox() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_2 ) );
	vFileMenu->addAction( "Move Camera", this, SLOT( OnMoveCamera() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_3 ) );
	vFileMenu->addAction( "Move Box0", this, SLOT( OnMoveBox0() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_4 ) );
	vFileMenu->addAction( "IK test", this, SLOT( OnIkTest() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_5 ) );
	vFileMenu->addAction( "Test Script", this, SLOT( OnTestScript() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_6 ) );
	vFileMenu->addAction( "Test Serializer", this, SLOT( OnTestSerializer() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_7 ) );

	// Subscribe for Ogre messages
	cOgreResponderSetBGColour::Get().AddListener( *mScene );
	cOgreResponderCreateBox::Get().AddListener( *mScene );
	cOgreResponderSetPosition::Get().AddListener( *mScene );
}

/************************************************************************/
cQMainWindow::~cQMainWindow()
/************************************************************************/
{
	SAFE_DELETE( mScene );
	SAFE_DELETE( mMenuBar );
}

/**********************************************************************/
void cQMainWindow::closeEvent( QCloseEvent *iEvent )
/**********************************************************************/
{
	QSettings vSettings( QSettings::UserScope, "CMLabs", "OgreApp" );
	vSettings.setValue( "WindowGeometry", saveGeometry() );
	vSettings.setValue(    "WindowState",    saveState() );

	QMainWindow::closeEvent( iEvent );
}

/************************************************************************/
void cQMainWindow::OnSetBgColour()
/************************************************************************/
{
	cOgreResponderSetBGColour::cParam vParam;
//	vParam.Colour.setAsARGB( 0x4488aacc );
	vParam.Colour.r = Ogre::Math::RangeRandom( 0, 1 );
	vParam.Colour.g = Ogre::Math::RangeRandom( 0, 1 );
	vParam.Colour.b = Ogre::Math::RangeRandom( 0, 1 );
	vParam.Colour.a = Ogre::Math::RangeRandom( 0, 1 );
	cDispatcherHub::IParam vIParam( (void*)&vParam );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetBGColour::Get().GetEventName(), vIParam );
}

/************************************************************************/
void cQMainWindow::OnCreateBox()
/************************************************************************/
{
	struct sTemp
	{
		Ogre::Vector3 Position;
		Ogre::Vector3 Size;
		char Name[256];
	};
	sTemp vParam;
	vParam.Position.x = Ogre::Math::RangeRandom( -1, 1 );
	vParam.Position.y = Ogre::Math::RangeRandom( -1, 1 );
	vParam.Position.z = Ogre::Math::RangeRandom( -1, 1 );
	vParam.Size.x = Ogre::Math::RangeRandom( -1, 1 );
	vParam.Size.y = Ogre::Math::RangeRandom( -1, 1 );
	vParam.Size.z = Ogre::Math::RangeRandom( -1, 1 );
	static int vCount = 0;
	sprintf( vParam.Name, "Box%d", vCount++ );
	cDispatcherHub::IParam vIParam( (void*)&vParam );

	cDispatcherHub::Get().Dispatch( cOgreResponderCreateBox::Get().GetEventName(), vIParam );
}

/************************************************************************/
void cQMainWindow::OnMoveCamera()
/************************************************************************/
{
	struct sTemp
	{
		Ogre::Vector3 Pos;
		char Name[256];
	};
	sTemp vTemp;
	vTemp.Pos = Ogre::Vector3( 0, 0, -100 );
	strcpy( vTemp.Name, "MainCamera" );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetPosition::Get().GetEventName(), vIParam );
}

/************************************************************************/
void cQMainWindow::OnMoveBox0()
/************************************************************************/
{
	struct sTemp
	{
		Ogre::Vector3 Pos;
		char Name[256];
	};
	sTemp vTemp;
	vTemp.Pos.x = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.y = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.z = Ogre::Math::RangeRandom( -50, 50 );
	strcpy( vTemp.Name, "Box0" );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetPosition::Get().GetEventName(), vIParam );
}


/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnIkTest()
/************************************************************************/
{
	struct sTemp
	{
		int Effector;
		Ogre::Vector3 Pos;
	};
	sTemp vTemp;
	vTemp.Effector = 0;
	vTemp.Pos.x = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.y = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.z = Ogre::Math::RangeRandom( -50, 50 );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderSetPosition"), vIParam );

	struct sTempRot
	{
		int Effector;
		Ogre::Quaternion Rot;
	};
	sTempRot vTempRot;
	vTempRot.Effector = 2;
	Ogre::Vector3 vAxis;
	vAxis.x = Ogre::Math::RangeRandom( -1, 1 );
	vAxis.y = Ogre::Math::RangeRandom( -1, 1 );
	vAxis.z = Ogre::Math::RangeRandom( -1, 1 );
	vAxis = Ogre::Vector3( 0, 1, 0 );
	vAxis.normalise();
	vTempRot.Rot.FromAngleAxis( Ogre::Radian( Ogre::Math::RangeRandom( -1, 1 ) * Ogre::Math::PI * 2.0f ), vAxis );
	cDispatcherHub::IParam vIParamRot( (void*)&vTempRot );

	cDispatcherHub::Get().Dispatch( _T("Avatar::cAvatarResponderSetRotation"), vIParamRot );
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnTestScript()
/************************************************************************/
{
	QFileDialog vFile;
//	vFile.show();

	struct sTemp
	{
		char Name[256];
	};
	sTemp vTemp;
	strcpy( vTemp.Name, vFile.getOpenFileName().toLocal8Bit() );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

//	cDispatcherHub::Get().Dispatch( _T("Scripting::cScriptingResponderRunScript"), vIParam );
	cDispatcherHub::Get().Dispatch( _T("Scripting::cScriptingResponderRunNative"), vIParam );
//	cDispatcherHub::Get().Dispatch( _T("Scripting::cScriptingResponderRunDll"), vIParam );
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnTestSerializer()
/************************************************************************/
{
	QFileDialog vFile;
	std::wfstream *vStream = new std::wfstream( vFile.getOpenFileName().toLocal8Bit() );
	cSerializerXML *vSerializer = new cSerializerXML( *vStream );
	vSerializer->Deserialize();
}
