#pragma once

#pragma warning( push )
#pragma warning( disable: 4512 )	// assignment operator could not be generated
#pragma warning( disable: 4510 )	// default constructor could not be generated
#include <QtGui/QtGui>
#pragma warning( push )

using Loom::Core::cAutoPtr;

BEGIN_NAMESPACE( Weaver )

class cQScene;

class cQMainWindow : public QMainWindow
{
	Q_OBJECT

protected:
	// QMainWindow methods
	virtual void closeEvent( QCloseEvent *iEvent );

protected:
	cAutoPtr<cQScene>    mScene;
	cAutoPtr<QMenuBar> mMenuBar;

public:
	cQMainWindow();

	public slots:
		void OnSetBgColour();
		void OnCreateBox();
		void OnMoveCamera();
		void OnMoveBox0();
};

END_NAMESPACE()

// Needed for Qt metadata
using Loom::Weaver::cQMainWindow;
