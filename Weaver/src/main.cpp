#include <windows.h>

#include <Weaver/App/cApp.h>
using namespace Loom::Weaver;

#if 0
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
#else
int main( int, char ** )
#endif
{
	return cApp::Get().Run( __argc, __argv );
}