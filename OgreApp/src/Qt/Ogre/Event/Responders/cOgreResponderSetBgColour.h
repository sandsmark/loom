#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderSetBGColour : public IOgreResponder<cOgreResponderSetBGColour>
{
public:
	class cParam
	{
	public:
		Ogre::ColourValue Colour;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{	// TODO: Smarter implementation
			*this = *((cParam*)iParam.GetData());
		}
	};

public:
	cOgreResponderSetBGColour() : IOgreResponder( _T("Ogre::cOgreResponderSetBGColour") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnSetBackgroundColour, vParam.Colour );
	}
};

END_NAMESPACE()