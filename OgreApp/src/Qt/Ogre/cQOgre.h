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
//    Q_OBJECT
protected:
    // Ogre stuff
    Ogre::Root *mRoot;
    Ogre::RenderWindow *mWindow;
    Ogre::Camera *mCamera;
    Ogre::Viewport *mViewport;
    Ogre::SceneManager *mScene;
	QTextEdit *mDebugPanel;
	QTextEdit *mOutputPanel;

    bool InitRoot( void );
    bool InitWindow( void );
    virtual bool InitResources( void ) { return true; }
    bool InitScene( void );
    virtual bool InitPost( void ) { return true; }
    
public:
    cQOgre( QWidget *iParent = NULL );
    ~cQOgre();
    
    virtual void RenderFrame( void ) {}
	Ogre::SceneManager *GetScene( void ) const { return mScene; }
    
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

	// IOgreEvent methods
	virtual void OnSetBackgroundColour( const Ogre::ColourValue &iColour );
	virtual void OnCreateBox( const Ogre::String &iName, const Ogre::Vector3 &iPosition, const Ogre::Vector3 &iSize );
	virtual void OnSetPosition( const Ogre::String &iName, const Ogre::Vector3 &iPosition );
	virtual void OnGetPosition( const Ogre::String &iName, Ogre::Vector3 &oPosition );
	virtual void OnDebugLog( const cString &iText );
	virtual void OnOutput( const cString &iText );
	virtual void OnSetTexture( const Ogre::String &iName, void *iTextureData, unsigned long iSize );
	virtual void OnMoveTo( const Ogre::String &iName, const Ogre::Vector3 &iPosition, float iSpeed );
};

END_NAMESPACE()
