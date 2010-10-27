#pragma once

#ifdef NETWORK_EXPORTS
	#define NETWORK_API __declspec(dllexport)
#else
	#define NETWORK_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
