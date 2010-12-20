#pragma once

#ifdef TRACKSTAR_EXPORTS
	#define TRACKSTAR_API __declspec(dllexport)
#else
	#define TRACKSTAR_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
