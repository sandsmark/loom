#include <MoMa/Module/cModuleMoMa.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Ogre/OgreMath.h>
#include <Ogre/OgreColourValue.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::MoMa;

/************************************************************************/
void cModuleMoMa::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleMoMa startup" ) );

	CreateThread( NULL, 0, StartThread, this, 0, NULL );
}

/************************************************************************/
DWORD cModuleMoMa::StartThread( LPVOID iParam )
/************************************************************************/
{
	while ( true )
	{
		Sleep( 1000 );

		// Test
		struct sTemp
		{
			Ogre::ColourValue Colour;
		};
		sTemp vParam;
		vParam.Colour.r = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.g = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.b = Ogre::Math::RangeRandom( 0, 1 );
		vParam.Colour.a = Ogre::Math::RangeRandom( 0, 1 );
		cDispatcherHub::IParam vIParam( (void*)&vParam );

		cDispatcherHub::Get().Dispatch( _T("Ogre::cOgreResponderSetBGColour"), vIParam );
	}
}
