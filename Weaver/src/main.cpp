#include <windows.h>

#include <Weaver/App/cApp.h>
using namespace Loom::Weaver;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	return cApp::Get().Run( __argc, __argv );
}