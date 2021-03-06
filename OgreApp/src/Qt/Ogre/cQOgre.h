/*
 *  cQOgre.h
 *  QTTest
 *
 *  Created by mb on 6/6/10.
 *  Copyright 2010 Communicative Machines. All rights reserved.
 *
 */

#include <QtOpenGL/QGLWidget>
#include <OgreApp/Qt/Ogre/Event/IOgreEvent.h>
#include <Ogre/OgreString.h>
#include <QtGui/QTextEdit.h>
#include <Ogre/OgreMesh.h>

namespace Ogre
{
	class Root;
	class RenderWindow;
	class Camera;
	class Viewport;
	class SceneManager;
};

BEGIN_NAMESPACE( OgreApp )
    
class OGREAPP_API cQOgre : public QGLWidget, public IOgreEvent
{
public:
	typedef const Ogre::String* LPOGRESTR;

//    Q_OBJECT
protected:
	struct sLineVertex
	{
		Ogre::Vector3     Pos;
		DWORD             Col;
	};

protected:
    // Ogre stuff
    Ogre::Root *mRoot;
    Ogre::RenderWindow *mWindow;
    Ogre::Camera *mCamera;
    Ogre::Viewport *mViewport;
    Ogre::SceneManager *mScene;
	QTextEdit *mDebugPanel;
	QTextEdit *mOutputPanel;
	Ogre::MeshPtr mLines;

    bool InitRoot( void );
    bool InitWindow( void );
    virtual bool InitResources( void ) { return true; }
    bool InitScene( void );
	void InitLines( void );
    virtual bool InitPost( void ) { return true; }
    
public:
    cQOgre( QWidget *iParent = NULL );
    ~cQOgre();
    
    virtual void RenderFrame( void ) {}
	Ogre::SceneManager *GetScene( void ) const { return mScene; }
	Ogre::RenderWindow *GetWindow( void ) const { return mWindow; }
    
    // QGLWidget methods
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL( int iWidth, int iHeight );
    
    // QWidget methods
    virtual void paintEvent(QPaintEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    
    virtual QSize minimumSizeHint(void) const { return QSize( 128, 128 ); }    

	void SetDebugPanel( QTextEdit *iDebugPanel ) { mDebugPanel = iDebugPanel; }
	void SetOutputPanel( QTextEdit *iOutputPanel ) { mOutputPanel = iOutputPanel; }

	void ClearLines( void );
	void AddLine( const Ogre::Vector3 &iFrom, const Ogre::Vector3 &iTo, const Ogre::ColourValue &iColour = Ogre::ColourValue::White );

	void OnGetEntitiesNoStd( LPOGRESTR *oNames, size_t &oNumNames );

	// IOgreEvent methods
	virtual void OnSetBackgroundColour( const Ogre::ColourValue &iColour );
	virtual void OnCreateBox( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize );
	virtual void OnCreateSphere( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize );
	virtual void OnSetPosition( const Ogre::String &iName, const Ogre::Vector3 &iPosition );
	virtual void OnGetPosition( const Ogre::String &iName, Ogre::Vector3 &oPosition );
	virtual void OnDebugLog( const cString &iText );
	virtual void OnOutput( const cString &iText );
	virtual void OnSetTexture( const Ogre::String &iName, void *iTextureData, unsigned long iSize );
	virtual void OnSetTextureFile( const Ogre::String &iName, const Ogre::String &iTextureName );
	virtual void OnMoveTo( const Ogre::String &iName, const Ogre::Vector3 &iPosition, float iSpeed );
	virtual void OnGetEntities( cArray<Ogre::String> &oNames );
	virtual void OnGetBoundingBox( const Ogre::String &iName, Ogre::AxisAlignedBox &oBounds );
	virtual void OnClearLines( void );
	virtual void OnAddLine( const Ogre::Vector3 &iFrom, const Ogre::Vector3 &iTo, const Ogre::ColourValue &iColour = Ogre::ColourValue::White );
	virtual void OnStopMove( const Ogre::String &iName );
	virtual void OnSetScale( const Ogre::String &iName, const Ogre::Vector3 &iScale );
	virtual void OnGetScale( const Ogre::String &iName, Ogre::Vector3 &oScale );
};

END_NAMESPACE()
