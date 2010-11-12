#pragma once

#ifdef SCRIPTING_EXPORTS
#define SCRIPTING_API __declspec(dllexport)
#else
#define SCRIPTING_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
