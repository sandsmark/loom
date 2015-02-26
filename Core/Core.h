#pragma once

#ifdef WIN32
    #pragma warning( disable: 4251 )	// class needs to have dll-interface to be used by clients of class
    #ifdef CORE_EXPORTS
    	#define CORE_API __declspec(dllexport)
    #else
    	#define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API
#endif



#include <Core/Helper/Helper.h>
#include <Core/Debug/cDebug.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Datatype/cAutoPtr.h>
#include <Core/RTTI/cClass.h>
#include <Core/RTTI/cObject.h>
#include <Core/RTTI/cClassRegistry.h>
