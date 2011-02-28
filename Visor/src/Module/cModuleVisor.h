#pragma once

#include <Core/Module/IModule.h>
using Loom::Core::IModule;

#include <windows.h>
#include <set>
#include <hash_map>

namespace r_comp
{
	class Image;
};

BEGIN_NAMESPACE( Visor )

class cConfig;
class cVisualizerReplicode;

class VISOR_API cModuleVisor : public IModule
{
protected:
	cConfig *mConfig;
	cVisualizerReplicode *mVisualizer;

	typedef stdext::hash_map<const r_comp::Image *, std::set<int> > tImageViewHash;
	tImageViewHash mImageViews;

public:
	cModuleVisor();

	virtual void Init( void );
	virtual void Destroy( void );
	void SetImage( r_comp::Image *iImage );

	bool HasView( const r_comp::Image *iImage, int iOID );
};

END_NAMESPACE()
