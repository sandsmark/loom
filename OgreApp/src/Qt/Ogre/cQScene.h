#pragma once
#include <OgreApp/Qt/Ogre/cQOgre.h>
#include <Ogre/OgreVector2.h>
#include <Ogre/OgreVector3.h>

BEGIN_NAMESPACE( OgreApp )

class OGREAPP_API cQScene : public cQOgre
{
protected:
	enum eKey
	{
		KEY_LEFT = 0,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_NUM,
	};

protected:
	bool mKeyStates[KEY_NUM];

	// Camera controls ( TODO: Create external class for camera handling )
	QPoint mLastMousePos;
	Ogre::Vector2 mCameraAngularVelocity;        
	Ogre::Vector2 mCameraAngle;        
	Ogre::Vector3 mCameraVelocity;
	Ogre::Vector3 mCameraTarget;
	float mZoom;
	DWORD mLastUpdate;

	void Rotate( const float iDX, const float iDY );
	void Move( const float iDX, const float iDY );
	void Zoom( const float iDelta );
	void UpdateCamera( void );

	virtual bool InitResources( void );
	virtual bool InitPost( void );

public:
	cQScene( QWidget *iParent = NULL );

	virtual void RenderFrame( void );

	// QWidget methods
	virtual void mousePressEvent( QMouseEvent *iEvent );
	virtual void mouseReleaseEvent( QMouseEvent *iEvent );
	virtual void mouseMoveEvent( QMouseEvent *iEvent );
	virtual void wheelEvent( QWheelEvent *iEvent );
	virtual void keyPressEvent( QKeyEvent *iEvent );
	virtual void keyReleaseEvent( QKeyEvent *iEvent );
};

END_NAMESPACE()