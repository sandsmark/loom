#include <TrackStar/TrackStar.h>
#include <TrackStar/Module/cModuleTrackStar.h>

using namespace Loom::TrackStar;

extern "C"
{

TRACKSTAR_API IModule *CreateModule( void )
{
	return new cModuleTrackStar();
}

}
