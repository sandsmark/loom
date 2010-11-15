#pragma once

#ifdef CONFIG_EXPORTS
#define CONFIG_API __declspec(dllexport)
#else
#define CONFIG_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
