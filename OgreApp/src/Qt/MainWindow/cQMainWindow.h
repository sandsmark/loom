#pragma once

#pragma warning( push )
#pragma warning( disable: 4512 )	// assignment operator could not be generated
#pragma warning( disable: 4510 )	// default constructor could not be generated
#include <QtGui/QtGui>
#pragma warning( push )

using Loom::Core::cAutoPtr;

BEGIN_NAMESPACE( OgreApp )

class cQScene;

class OGREAPP_API cQMainWindow : public QMainWindow
{
	Q_OBJECT

protected:
	// QMainWindow methods
	virtual void closeEvent( QCloseEvent *iEvent );

protected:
	cQScene    *mScene;
	QMenuBar *mMenuBar;
	QDockWidget *mDebugDock;
	QTextEdit *mDebugWindow;
	QDockWidget *mOutputDock;
	QTextEdit *mOutputWindow;

public:
	cQMainWindow();
	~cQMainWindow();

	cQScene *GetScene( void ) const { return mScene; }

	public slots:
		void OnSetBgColour();
		void OnCreateBox();
		void OnMoveCamera();
		void OnMoveBox0();
		void OnIkTest();
		void OnTestScript();
		void OnTestSerializer();
		void OnSetMaterial();
		void OnListEntities();
		void OnGetBounds();
		void OnAddLine();
		void OnTest();
};

END_NAMESPACE()

// Needed for Qt metadata
using Loom::OgreApp::cQMainWindow;
