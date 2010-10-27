#pragma once

#ifdef MOMA_EXPORTS
#define MOMA_API __declspec(dllexport)
#else
#define MOMA_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()

/*
extern "C"
{
	MOMA_API Loom::Core::IModule *CreateModule( void );
}
*/