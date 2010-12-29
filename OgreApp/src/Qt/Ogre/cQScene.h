#pragma once
#include <OgreApp/Qt/Ogre/cQOgre.h>
#include <Ogre/OgreVector2.h>
#include <Ogre/OgreVector3.h>
#include <Core/Spline/cMotion.h>

using Loom::Core::cMotion;

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
	struct sAnim
	{
		Ogre::Entity *Entity;
		Ogre::Camera *Camera;
		cMotion<Ogre::Vector3> *Motion;

		sAnim() : Entity( NULL ), Camera( NULL ), Motion( NULL ) {}
		~sAnim() { SAFE_DELETE( Motion ); }
	};

protected:
	bool mKeyStates[KEY_NUM];
	cArray< sAnim* > mAnims;


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
	void UpdateCamera( const float iEllapsed );
	void UpdateAnims( const float iEllapsed );

	virtual bool InitResources( void );
	virtual bool InitPost( void );

public:
	cQScene( QWidget *iParent = NULL );

	virtual void RenderFrame( void );

	virtual void OnMoveTo( const Ogre::String &iName, const Ogre::Vector3 &iPosition, float iSpeed );

	// QWidget methods
	virtual void mousePressEvent( QMouseEvent *iEvent );
	virtual void mouseReleaseEvent( QMouseEvent *iEvent );
	virtual void mouseMoveEvent( QMouseEvent *iEvent );
	virtual void wheelEvent( QWheelEvent *iEvent );
	virtual void keyPressEvent( QKeyEvent *iEvent );
	virtual void keyReleaseEvent( QKeyEvent *iEvent );
};

END_NAMESPACE()