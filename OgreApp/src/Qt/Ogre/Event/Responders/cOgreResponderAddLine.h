#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderAddLine : public IOgreResponder<cOgreResponderAddLine>
{
public:
	class cParam
	{
	public:
		Ogre::Vector3 From;
		Ogre::Vector3 To;
		Ogre::ColourValue Colour;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{	// TODO: Smarter implementation
			*this = *((cParam*)iParam.GetData());
		}
	};

public:
	cOgreResponderAddLine() : IOgreResponder( _T("Ogre::cOgreResponderAddLine") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IOgreEvent::OnAddLine, vParam.From, vParam.To, vParam.Colour );
	}
};

END_NAMESPACE()