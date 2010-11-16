#pragma once

#ifdef MBRANE_EXPORTS
	#define MBRANE_API __declspec(dllexport)
#else
	#define MBRANE_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
