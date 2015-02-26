#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>
#include <Core/Datatype/cString.h>

using Loom::Core::cString;

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderDebugLog : public IOgreResponder<cOgreResponderDebugLog>
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
			wchar_t *iText = ((wchar_t*)iParam.GetData());
			wchar_t *vTemp = new wchar_t[ _tcslen( iText ) + 1 ];
			_tcscpy( vTemp, iText );
			Text = new cString( vTemp );
		}
		~cParam()
		{
			delete Text;
		}
	};

public:
	cOgreResponderDebugLog() : IOgreResponder( _T("Ogre::cOgreResponderDebugLog") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnDebugLog, *vParam.Text );
	}
};

END_NAMESPACE()