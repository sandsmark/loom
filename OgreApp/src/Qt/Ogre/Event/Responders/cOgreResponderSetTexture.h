#pragma once

#include <OgreApp/Qt/Ogre/Event/Responders/IOgreResponder.h>
#include <Ogre/OgreColourValue.h>

BEGIN_NAMESPACE( OgreApp )

class cOgreResponderSetTexture : public IOgreResponder<cOgreResponderSetTexture>
{
public:
	class cParam
	{
	public:
		Ogre::String Name;
		void *Data;
		unsigned long Size;

	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *iInput = (char*)iParam.GetData();
			char *iName = iInput;
			char *vTemp = new char[ strlen( iName ) + 1 ];
			strcpy( vTemp, iName );
			Name = vTemp;
			iInput += strlen( iName ) + 1;
			Size = *(unsigned long*)iInput;
			iInput += 4;
			Data = new char[ Size ];
			memcpy( Data, iInput, Size );
		}
		~cParam()
		{
			delete [] Data;
			Data = NULL;
			Size = 0;
		}
	};

public:
	cOgreResponderSetTexture() : IOgreResponder( _T("Ogre::cOgreResponderSetTexture") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
//		DispatchHack<Ogre::String,void*,unsigned long>( &IOgreEvent::OnSetTexture, vParam.Name, vParam.Data, vParam.Size );
		Dispatch( &IOgreEvent::OnSetTexture, vParam.Name, vParam.Data, vParam.Size );
	}
};

END_NAMESPACE()