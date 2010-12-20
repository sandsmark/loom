#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>
#include <Core/Datatype/cString.h>

using Loom::Core::cString;

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderOutput : public IOgreResponder<cOgreResponderOutput>
{
public:
	class cParam
	{
	public:
		cString *Text;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			TCHAR *iText = ((TCHAR*)iParam.GetData());
			TCHAR *vTemp = new TCHAR[ _tcslen( iText ) + 1 ];
			_tcscpy( vTemp, iText );
			Text = new cString( vTemp );
		}
		~cParam()
		{
			delete Text;
		}
	};

public:
	cOgreResponderOutput() : IOgreResponder( _T("Ogre::cOgreResponderOutput") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnOutput, *vParam.Text );
	}
};

END_NAMESPACE()