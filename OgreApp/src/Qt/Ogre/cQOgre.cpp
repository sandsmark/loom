/*
 *  cQOgre.cpp
 *  QTTest
 *
 *  Created by mb on 6/6/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#pragma warning( push )
#pragma warning( disable: 4995 )	// #pragma deprecated

#include <windows.h>
#include <strsafe.h>
#include <OgreApp/Qt/Ogre/cQOgre.h>
#include <Core/Debug/Logger/cLogger.h>
#include <QtGui/qevent.h>
#include <QtGui/QTextEdit.h>
#ifdef Q_WS_MAC
	#include <OpenGL/OpenGL.h>
#else
	#include <wingdi.h>
#endif

#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable: 4189 )	// local variable is initialized but not referenced
#include <Ogre/Ogre.h>
#pragma warning( pop )

using namespace Loom::OgreApp;
using Loom::Core::cLogger;

/**********************************************************************/
cQOgre::cQOgre( QWidget *iParent )
: QGLWidget( iParent ), mWindow( NULL ), mViewport( NULL ), mDebugPanel( NULL )
/**********************************************************************/
{
    resize( 128, 128 );
    setAttribute( Qt::WA_PaintOnScreen );
    setAttribute( Qt::WA_NoBackground );
    
    InitRoot();
}

/**********************************************************************/
cQOgre::~cQOgre()
/**********************************************************************/
{
    mRoot->shutdown();
    delete mRoot;
    destroy();
}

/**********************************************************************/
bool cQOgre::InitRoot( void )
/**********************************************************************/
{
    // Basic config
#ifdef Q_WS_MAC
    Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";    
#else
    Ogre::String vPath = "resources/";    
#endif
    Ogre::String vA = vPath + "config/plugins.cfg";    
	Ogre::String vB = vPath + "config/OgreApp.cfg";    
	Ogre::String vC = vPath + "../logs/OgreApp.log";
	mRoot = OGRE_NEW Ogre::Root( vA, vB, vC );
  
    // Select rendersystem ( TODO: Select OpenGL )
    Ogre::RenderSystemList vRenderers = mRoot->getAvailableRenderers();
    Ogre::RenderSystem *vRender = *vRenderers.begin();
    mRoot->setRenderSystem( vRender );
    
    // Set size to widget size
    QString vSize = QString( "%1x%2" ).arg( width() ).arg( height() );
	std::string vFos = vSize.toStdString();
	const char *vFos2 = vFos.c_str();
    vRender->setConfigOption( "Video Mode", vFos2 );
    vRender->setConfigOption( "Full Screen", "No" );
    mRoot->saveConfig();
    
//    mRoot->showConfigDialog();
//    mRoot->saveConfig();
    
    // Init
    mRoot->initialise( false );
    
    return true;
}

/**********************************************************************/
bool cQOgre::InitWindow( void )
/**********************************************************************/
{
#ifdef Q_WS_MAC
    // Remember Qt's context
    CGLContextObj vContext = CGLGetCurrentContext();
#else
	HGLRC vContext = wglGetCurrentContext();
	HDC vDC = wglGetCurrentDC();
#endif
    
    // Use widget's window
    Ogre::NameValuePairList vParams;
	vParams[ "externalWindowHandle" ] = Ogre::StringConverter::toString( (size_t)winId() );
    vParams[ "macAPI" ] = "cocoa";
    vParams[ "macAPICocoaUseNSView" ] = "true";
    vParams[ "externalGLControl" ] = "true";
//    vParams[ "externalGLContext" ] = (unsigned long)vContext;
//    vParams[ "currentGLContext" ] = (unsigned long)vContext;
    
    mWindow = mRoot->createRenderWindow( "cQOgre", width(), height(), false, &vParams );    

	/*
    // Take over Ogre window
    WId vOgreWinId = 0;
    mWindow->getCustomAttribute( "WINDOW", &vOgreWinId );
    create( vOgreWinId );
	*/

#ifdef Q_WS_MAC
    // Force Qt context
    CGLSetCurrentContext( vContext );
#endif

    return true;
}

/**********************************************************************/
bool cQOgre::InitScene( void )
/**********************************************************************/
{
    mScene    = mRoot->createSceneManager( Ogre::ST_GENERIC );    
    mCamera   = mScene->createCamera( "MainCamera" );
    mViewport = mWindow->addViewport( mCamera );
    
    return true;
}

/**********************************************************************/
void cQOgre::initializeGL()
/**********************************************************************/
{
    InitWindow();
    InitResources();
    InitScene();
    InitPost();
}

/**********************************************************************/
void cQOgre::paintGL()
/**********************************************************************/
{
    RenderFrame();
    mRoot->renderOneFrame();
}

/**********************************************************************/
void cQOgre::resizeGL( int iWidth, int iHeight )
/**********************************************************************/
{
    mCamera->setAspectRatio( (float)width() / (float)height() );
	mWindow->resize( width(), height() );
    mWindow->windowMovedOrResized();
}

/**********************************************************************/
void cQOgre::resizeEvent( QResizeEvent *e )
/**********************************************************************/
{
    QGLWidget::resizeEvent( e );
//    resizeGL( e->size().width(), e->size().height() );
}

/**********************************************************************/
void cQOgre::paintEvent(QPaintEvent *)
/**********************************************************************/
{
    updateGL();
}

/**********************************************************************/
void cQOgre::OnSetBackgroundColour( const Ogre::ColourValue &iColour )
/**********************************************************************/
{
	if ( !mViewport ) return;
	mViewport->setBackgroundColour( iColour );
}

/************************************************************************/
void cQOgre::OnCreateBox( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize )
/************************************************************************/
{
	/*
	static int vCounter = 0;
	char vTemp[ 256 ];
	sprintf( vTemp, "CubeMaterial%d", vCounter );
	Ogre::MaterialPtr vMaterial = ((Ogre::MaterialPtr)(Ogre::MaterialManager::getSingleton().getByName( "CubeMaterial" )))->clone( vTemp );
	vMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName( vName.str() );
	sprintf( vTemp, "CubeTexture%d", vCounter );
	Ogre::TextureManager::createManual( vTemp, "", Ogre::TEX_TYPE_2D, 256, 256, 1, 0, Ogre::PF_A8R8G8B8 );
	vCounter++;
	*/

	Ogre::Entity *vCube = mScene->createEntity( iName, Ogre::SceneManager::PT_CUBE );
	vCube->setMaterialName( "CubeMaterial" );
	Ogre::SceneNode *vNode = mScene->getRootSceneNode()->createChildSceneNode( iPosition );
	vNode->attachObject( vCube );
	vNode->setScale( iSize );
}

/************************************************************************/
void Loom::OgreApp::cQOgre::OnSetPosition( const Ogre::String &iName, const Ogre::Vector3 &iPosition )
/************************************************************************/
{
	if ( mScene->hasCamera( iName ) )
	{
		mScene->getCamera( iName )->setPosition( iPosition );
		return;
	}

	Ogre::Entity *vEntity = mScene->getEntity( iName );

	if ( !vEntity )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, _T("Unknown entity: %S"), iName );	// TODO: Use %s if not in unicode
		cLogger::Get().Log( cLogger::LOG_WARNING, _T("Global"), vTemp );
	}

	vEntity->getParentNode()->setPosition( iPosition );
}

/************************************************************************/
void Loom::OgreApp::cQOgre::OnGetPosition( const Ogre::String &iName, Ogre::Vector3 &oPosition )
/************************************************************************/
{
	if ( mScene->hasCamera( iName ) )
	{
		oPosition = mScene->getCamera( iName )->getPosition();
		return;
	}

	Ogre::Entity *vEntity = mScene->getEntity( iName );

	if ( !vEntity )
	{
		TCHAR vTemp[ 256 ];
		StringCchPrintf( vTemp, 256, _T("Unknown entity: %S"), iName );	// TODO: Use %s if not in unicode
		cLogger::Get().Log( cLogger::LOG_WARNING, _T("Global"), vTemp );
	}

	oPosition = vEntity->getParentNode()->getPosition();
}

/************************************************************************/
void Loom::OgreApp::cQOgre::OnDebugLog( const cString &iText )
/************************************************************************/
{
	if ( mDebugPanel )
	{
		mDebugPanel->append( QString::fromUtf16( (const ushort*)iText.ToCString() ) );
	}


	OutputDebugString( iText.ToCString() );
	OutputDebugString( _T("\n") );
}

/************************************************************************/
void Loom::OgreApp::cQOgre::OnOutput( const cString &iText )
/************************************************************************/
{
	if ( mOutputPanel )
	{
		mOutputPanel->append( QString::fromUtf16( (const ushort*)iText.ToCString() ) );
	}
}
