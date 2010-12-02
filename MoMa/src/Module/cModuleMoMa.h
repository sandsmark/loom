#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;
using Loom::Core::cObject;

BEGIN_NAMESPACE( MoMa )

class cController;

class MOMA_API cModuleMoMa : public IModule
{
protected:
	cController *mController;

public:
	cModuleMoMa();

	virtual void Init( void );
	virtual void Destroy( void );
};

END_NAMESPACE()
