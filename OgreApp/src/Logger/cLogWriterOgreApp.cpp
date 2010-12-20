#include <OgreApp/Logger/cLogWriterOgreApp.h>
#include <windows.h>

#include <Core/Datatype/cString.h>
#include <Core/Event/cDispatcherHub.h>

using namespace Loom::OgreApp;
using Loom::Core::cDispatcherHub;

/************************************************************************/
void cLogWriterOgreApp::Log( const Loom::Core::cString &iMessage )
/************************************************************************/
{
	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderDebugLog"), cDispatcherHub::IParam( (void*)iMessage.ToCString() ) );
}
