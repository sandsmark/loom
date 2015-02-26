#pragma once

using Loom::Core::cObject;

BEGIN_NAMESPACE( Visor )

class cConfig : public cObject<cConfig>
{
public:
	wchar_t *ConfigPath;

	cConfig();
};

END_NAMESPACE()
