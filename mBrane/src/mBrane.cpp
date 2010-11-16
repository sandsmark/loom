#include <mBrane/mBrane.h>
#include <mBrane/Module/cModulemBrane.h>

using namespace Loom::MBrane;

extern "C"
{

MBRANE_API IModule *CreateModule( void )
{
	return new cModulemBrane();
}

}
