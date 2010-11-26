#pragma once

#ifdef CORE_EXPORTS
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#pragma warning( disable: 4251 )	// class needs to have dll-interface to be used by clients of class

#include <Core/Helper/Helper.h>
#include <Core/Debug/cDebug.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Datatype/cAutoPtr.h>
#include <Core/RTTI/cClass.h>
#include <Core/RTTI/cObject.h>
#include <Core/RTTI/cClassRegistry.h>
