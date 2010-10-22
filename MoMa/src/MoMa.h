#pragma once

#ifdef MOMA_EXPORTS
#define MOMA_API __declspec(dllexport)
#else
#define MOMA_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Weaver )
	class IModule;
END_NAMESPACE()

extern "C"
{
	MOMA_API Loom::Weaver::IModule *CreateModule( void );
}