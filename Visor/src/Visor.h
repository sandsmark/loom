#pragma once

#ifdef VISOR_EXPORTS
	#define VISOR_API __declspec(dllexport)
#else
	#define VISOR_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
