#pragma once

using Loom::Core::cObject;

BEGIN_NAMESPACE( Visor )

class cConfig : public cObject<cConfig>
{
public:
	TCHAR *ConfigPath;

	cConfig();
};

END_NAMESPACE()
