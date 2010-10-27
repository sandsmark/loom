#pragma once

#ifdef OGREAPP_EXPORTS
#define OGREAPP_API __declspec(dllexport)
#else
#define OGREAPP_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()

/*
extern "C"
{
	OGREAPP_API Loom::Core::IModule *CreateModule( void );
}
*/