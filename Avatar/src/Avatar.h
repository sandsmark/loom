#pragma once

#ifdef AVATAR_EXPORTS
#define AVATAR_API __declspec(dllexport)
#else
#define AVATAR_API __declspec(dllimport)
#endif

// TODO: Find proper place for this
enum eEffector
{
	EFFECTOR_LEFT_HAND = 0,
	EFFECTOR_RIGHT_HAND,
	EFFECTOR_HEAD,
	EFFECTOR_LEFT_GRAB,
	EFFECTOR_RIGHT_GRAB,
	EFFECTOR_RIGHT_POINT,
	EFFECTOR_MAX,
};

BEGIN_NAMESPACE( Core )
	class IModule;
END_NAMESPACE()
