#include <Core/Core.h>
#include <Core/Event/cDispatcherHub.h>
#include <windows.h>

using Loom::Core::cDispatcherHub;

extern "C"
{

__declspec( dllexport ) int main( int argc, char *argv[] )
{
	OutputDebugStringA( "TEST SCRIPT" );
	struct sColour
	{
		float r;
		float g;
		float b;
		float a;
	};
	sColour vTestColour;
	vTestColour.r = 1;
	vTestColour.g = 0;
	vTestColour.b = 0;
	vTestColour.a = 1;

	cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderSetBGColour"), &vTestColour );
	OutputDebugStringA( "TEST SCRIPT DONE" );
}

}
