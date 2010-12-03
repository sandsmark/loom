#pragma once

#ifdef PSYCLONE_EXPORTS
	#define PSYCLONE_API __declspec(dllexport)
#else
	#define PSYCLONE_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
