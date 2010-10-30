#pragma once

#ifdef SPEECH_EXPORTS
	#define SPEECH_API __declspec(dllexport)
#else
	#define SPEECH_API __declspec(dllimport)
#endif

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
