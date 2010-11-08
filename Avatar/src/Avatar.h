#pragma once

#ifdef AVATAR_EXPORTS
#define AVATAR_API __declspec(dllexport)
#else
#define AVATAR_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
