#include <Weaver/Qt/MainWindow/cQMainWindow.h>
#include <Weaver/Qt/Ogre/cQScene.h>
#include <Weaver/Qt/Ogre/Event/cOgreResponders.h>

using namespace Loom::Weaver;

/************************************************************************/
cQMainWindow::cQMainWindow()
/************************************************************************/
{
	QSettings vSettings( QSettings::UserScope, "CMLabs", "Weaver" );

	restoreGeometry( vSettings.value( "WindowGeometry" ).toByteArray() );
	restoreState   ( vSettings.value(    "WindowState" ).toByteArray() );

	mScene.Set( new cQScene( this ) );
	setCentralWidget( mScene );

	mMenuBar.Set( new QMenuBar( this ) );
	setMenuBar( mMenuBar );
	QMenu *vFileMenu = mMenuBar->addMenu( "&Events" );
	vFileMenu->addAction( "Change Background", this, SLOT( OnSetBgColour() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_1 ) );
	vFileMenu->addAction( "Create Box", this, SLOT( OnCreateBox() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_2 ) );
	vFileMenu->addAction( "Move Camera", this, SLOT( OnMoveCamera() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_3 ) );
	vFileMenu->addAction( "Move Box0", this, SLOT( OnMoveBox0() ), QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_4 ) );

	// Subscribe for Ogre messages
	cOgreResponderSetBGColour::Get().AddListener( *mScene );
	cOgreResponderCreateBox::Get().AddListener( *mScene );
	cOgreResponderSetPosition::Get().AddListener( *mScene );
}

/**********************************************************************/
void cQMainWindow::closeEvent( QCloseEvent *iEvent )
/**********************************************************************/
{
	QSettings vSettings( QSettings::UserScope, "CMLabs", "Weaver" );
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
	vTemp.Pos.x = Ogre::Math::RangeRandom( -10, 10 );
	vTemp.Pos.y = Ogre::Math::RangeRandom( -10, 10 );
	vTemp.Pos.z = Ogre::Math::RangeRandom( -10, 10 );
	strcpy( vTemp.Name, "Box0" );
	cDispatcherHub::IParam vIParam( (void*)&vTemp );

	cDispatcherHub::Get().Dispatch( cOgreResponderSetPosition::Get().GetEventName(), vIParam );
}

