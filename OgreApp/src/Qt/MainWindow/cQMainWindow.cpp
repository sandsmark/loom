#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <OgreApp/Qt/Ogre/Event/cOgreResponders.h>
#include <OgreApp/Logger/cLogWriterOgreApp.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Serializer/cSerializerXML.h>
#include <fstream>
#include <io.h>

#include <Ogre/OgreAxisAlignedBox.h>
#include <Strsafe.h>

using namespace Loom::OgreApp;
using Loom::Core::cSerializerXML;
using Loom::Core::cLogger;

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
	vFileMenu->addAction( "Set Material", this, SLOT( OnSetMaterial() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_8 ) );
	vFileMenu->addAction( "List Entities", this, SLOT( OnListEntities() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_9 ) );
	vFileMenu->addAction( "Get Bounds Test", this, SLOT( OnGetBounds() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_A ) );
	vFileMenu->addAction( "Add Line", this, SLOT( OnAddLine() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_B ) );
	vFileMenu->addAction( "Scale Avatar", this, SLOT( OnScaleAvatar() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_C ) );
	vFileMenu->addAction( "Test", this, SLOT( OnTest() ) );

	mDebugWindow = new QTextEdit();
	mDebugWindow->setDisabled( true );
	mDebugDock = new QDockWidget( this );
	mDebugDock->setWidget( mDebugWindow );
//	mDebugDock->show();
	mDebugDock->hide();
	mScene->SetDebugPanel( mDebugWindow );

	mOutputWindow = new QTextEdit();
	mOutputWindow->setDisabled( true );
	mOutputDock = new QDockWidget( this );
	mOutputDock->setWidget( mOutputWindow );
//	mOutputDock->show();
	mOutputDock->hide();
	mScene->SetOutputPanel( mOutputWindow );

	// Subscribe for Ogre messages
	cOgreResponderSetBGColour::Get().AddListener( *mScene );
	cOgreResponderCreateBox::Get().AddListener( *mScene );
	cOgreResponderSetPosition::Get().AddListener( *mScene );
	cOgreResponderDebugLog::Get().AddListener( *mScene );
	cOgreResponderOutput::Get().AddListener( *mScene );
	cOgreResponderSetTexture::Get().AddListener( *mScene );
	cOgreResponderMoveTo::Get().AddListener( *mScene );
	cOgreResponderGetEntities::Get().AddListener( *mScene );
	cOgreResponderGetBoundingBox::Get().AddListener( *mScene );
	cOgreResponderClearLines::Get().AddListener( *mScene );
	cOgreResponderAddLine::Get().AddListener( *mScene );
	cOgreResponderStopMove::Get().AddListener( *mScene );
	cOgreResponderSetScale::Get().AddListener( *mScene );
	cOgreResponderGetScale::Get().AddListener( *mScene );

	cLogger::Get().AddWriter( _T( "OgreAppLog" ), new cLogWriterOgreApp( cLogger::LOG_DEBUG ) );
	cLogger::Get().Log( cLogger::LOG_DEBUG, _T( "OgreAppLog" ), _T( "OgreApp log test" ) );

	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderOutput"), _T("OgreApp output test" ) );
}

/************************************************************************/
cQMainWindow::~cQMainWindow()
/************************************************************************/
{
	SAFE_DELETE( mOutputDock );
//	SAFE_DELETE( mOutputWindow );
	SAFE_DELETE( mDebugDock );
//	SAFE_DELETE( mDebugWindow );
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
	sprintf_s( vParam.Name, 256, "Box%d", vCount++ );
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
	strcpy_s( vTemp.Name, 256, "MainCamera" );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetPosition::Get().GetEventName(), vIParam );
}

/************************************************************************/
void cQMainWindow::OnMoveBox0()
/************************************************************************/
{
	struct sTemp
	{
		float Speed;
		Ogre::Vector3 Pos;
		char Name[256];
	};
	sTemp vTemp;
	vTemp.Speed = 10;
	vTemp.Pos.x = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.y = Ogre::Math::RangeRandom( -50, 50 );
	vTemp.Pos.z = Ogre::Math::RangeRandom( -50, 50 );
	strcpy_s( vTemp.Name, 256, "Box0" );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderMoveTo::Get().GetEventName(), vIParam );
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
	strcpy_s( vTemp.Name, 256, vFile.getOpenFileName().toLocal8Bit() );
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

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnSetMaterial()
/************************************************************************/
{
	FILE *f = fopen( "resources/materials/texture_test.jpg", "rb" );
	size_t vSize = filelength( f->_file );
	char *vTemp = new char[ 5 + 4 + vSize ];
	strcpy_s( vTemp, 256, "Box0" );
	*(unsigned long*)( vTemp + 5 ) = vSize;
	fread( vTemp + 9, vSize, 1, f );
	fclose( f );
	cDispatcherHub::IParam vIParam( (void*)vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetTexture::Get().GetEventName(), vIParam );
	delete [] vTemp;
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnTest()
/************************************************************************/
{
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnListEntities()
/************************************************************************/
{
	cArray<Ogre::String> vEntities;
	cOgreResponderGetEntities::Get().Dispatch( &IOgreEvent::OnGetEntities, vEntities );

	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderOutput"), TEXT( "List of entities:" ) );
	for ( size_t i=0; i<vEntities.GetSize(); i++ )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, L"\t%S", vEntities[i].c_str() );
		cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderOutput"), vTemp );
	}
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnGetBounds()
/************************************************************************/
{
	Ogre::AxisAlignedBox vBounds;
	cOgreResponderGetBoundingBox::Get().Dispatch( &IOgreEvent::OnGetBoundingBox, Ogre::String( "Avatar1" ), vBounds );

	TCHAR vTemp[ 256 ];
	StringCchPrintf( vTemp, 256, L"Avatar1 bounds: [%.2f,%.2f,%.2f]-[%.2f,%.2f,%.2f]", vBounds.getMinimum().x, vBounds.getMinimum().y, vBounds.getMinimum().z, vBounds.getMaximum().x, vBounds.getMaximum().y, vBounds.getMaximum().z );
	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderOutput"), vTemp );
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnAddLine()
/************************************************************************/
{
	Ogre::Vector3 vFrom;
	vFrom.x = Ogre::Math::RangeRandom( -50, 50 );
	vFrom.y = Ogre::Math::RangeRandom( -50, 50 );
	vFrom.z = Ogre::Math::RangeRandom( -50, 50 );

	Ogre::Vector3 vTo;
	vTo.x = Ogre::Math::RangeRandom( -50, 50 );
	vTo.y = Ogre::Math::RangeRandom( -50, 50 );
	vTo.z = Ogre::Math::RangeRandom( -50, 50 );

	Ogre::ColourValue vColour;
	vColour.r = Ogre::Math::UnitRandom();
	vColour.g = Ogre::Math::UnitRandom();
	vColour.b = Ogre::Math::UnitRandom();
	vColour.a = 1;

	cOgreResponderAddLine::Get().Dispatch( &IOgreEvent::OnAddLine, vFrom, vTo, vColour );
}

/************************************************************************/
void Loom::OgreApp::cQMainWindow::OnScaleAvatar()
/************************************************************************/
{
	cOgreResponderSetScale::Get().Dispatch( &IOgreEvent::OnSetScale, Ogre::String( "Avatar1" ), Ogre::Vector3( 7, 0.5, 1 ) );
}
