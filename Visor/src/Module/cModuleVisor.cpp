#include <Visor/Module/cModuleVisor.h>
#include <Core/Debug/Logger/cLogger.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <Visor/Config/cConfig.h>
#include <Core/Serializer/cSerializerXML.h>
#include <Visor/Replicode/Component/cComponentReplicode.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <Visor/Replicode/Hypertree/cVisualizerReplicode.h>
#include <Visor/Component/Events/IListenerComponent.h>
#include <Ogre/OgreResourceGroupManager.h>

using Loom::Core::cLogger;
using Loom::OgreApp::cModuleOgreApp;
using Loom::Core::cSerializerXML;
using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;

using namespace Loom::Visor;

/************************************************************************/
cModuleVisor::cModuleVisor()
: IModule( L"Visor" ), mConfig( NULL )
/************************************************************************/
{
	mDependencies.Add( cModuleOgreApp::GetName() );
}

#include <CoreLibrary/trunk/CoreLibrary/types.h>
#include <CoreLibrary/trunk/CoreLibrary/utils.h>
#include <Replicode/Test/settings.h>
#include <Replicode/r_exec/init.h>
#include <Replicode/r_exec/mem.h>
#include <Replicode/r_comp/decompiler.h>
#include <Replicode/r_comp/segments.h>
#include <Replicode/r_code/object.h>

/************************************************************************/
void cModuleVisor::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
//	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleVisor startup" ) );

	// Load config
	/*
	std::wfstream *vStream = new std::wfstream();
	vStream->open( "resources/config/visor.cfg", std::ios_base::in | std::ios_base::binary );
	if ( vStream->is_open() )
	{	// Open existing config
		cSerializerXML *vSerializer = new cSerializerXML( *vStream );
		mConfig = (cConfig*)vSerializer->Deserialize();
	}
	else
	{	// Create default config
		delete vStream;
		vStream = new std::wfstream();
		vStream->open( "resources/config/visor.cfg", std::ios_base::out | std::ios_base::binary );
		cSerializerXML *vSerializer = new cSerializerXML( *vStream );
		mConfig = new cConfig();
		vSerializer->Serialize( mConfig );
	}
	vStream->close();
	delete vStream;
	*/

	mConfig = new cConfig();

	// Add fonts
#ifdef Q_WS_MAC
	Ogre::String vPath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
	Ogre::String vPath = "resources/";
#endif
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation( vPath + "fonts/", "FileSystem" ); 

	// Get Ogre scene
	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( L"OgreApp" );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	// Create hypertree visualizer
	mVisualizer = new cVisualizerReplicode( vScene );
	IComponent::GetRegistry().AddListener( *mVisualizer ); 

	core::Time::Init(1000);

	Settings settings;
	ASSERT( settings.load( mConfig->ConfigPath ) );

#if 0
	ASSERT( r_exec::Init(settings.usr_operator_path.c_str(),Time::Get,settings.usr_class_path.c_str() ) )

	srand(r_exec::Now());
	Random::Init();

	std::string	error;
	ASSERT( r_exec::Compile(settings.source_file_name.c_str(),error) );

	r_exec::Mem<r_exec::LObject> *mem = new r_exec::Mem<r_exec::LObject>();

	r_code::vector<r_code::Code	*>	ram_objects;
	r_exec::Seed.getObjects(mem,ram_objects);

	mem->init(	settings.base_period,
		settings.reduction_core_count,
		settings.time_core_count,
		settings.probe_level,
		settings.notification_resilience,
		settings.goal_resilience,
		settings.assumption_resilience,
		settings.simulation_resilience);

	ASSERT( mem->load( r_exec::CreateCodeVector( &ram_objects[0], ram_objects.size() ) ) );
	uint64	starting_time=mem->start();
#endif

	r_exec::Mem<r_exec::LObject> *mem = r_exec::Run( settings.usr_operator_path.c_str(),Time::Get,settings.usr_class_path.c_str(), settings.source_file_name.c_str() );
	uint64	starting_time=mem->start();

//	Thread::Sleep(settings.run_time);
	Thread::Sleep(1000);

	mem->stop();

	r_comp::Image *vImage = mem->getImage();
	SetImage( vImage );

	delete	mem;

	mInitialized = true;
}

/************************************************************************/
void cModuleVisor::Destroy( void )
/************************************************************************/
{
	mInitialized = false;
}

/**********************************************************************/
bool cModuleVisor::HasView( const r_comp::Image *iImage, int iOID )
/**********************************************************************/
{
	return ( mImageViews[ iImage ].find( iOID ) != mImageViews[ iImage ].end() );
} 
/************************************************************************/
void cModuleVisor::SetImage( r_comp::Image * iImage )
/************************************************************************/
{
#if 1
    // We have to delete all current components
//    GetVisor()->Clean();
    
    r_code::vector< r_code::Code* > vObjects;
    iImage->getObjects< r_exec::LObject >( vObjects );
    
	r_comp::Decompiler	vDecompiler;
	vDecompiler.init( &r_exec::Metadata);
	vDecompiler.decompile_references( iImage );

    typedef stdext::hash_map<r_code::Code*,cComponentReplicode*> tComponentMap;
	tComponentMap vComponentMap;
    
    typedef stdext::hash_map<r_code::View*,cComponentReplicode*> tViewMap;
	tViewMap vViewMap;
    
    // Create components
    for ( size_t i=0; i<vObjects.size(); i++ )
    {
        std::ostringstream	vSourceCode;
        r_code::Code *vCode = vObjects[i];
        
        vDecompiler.decompile_object( i, &vSourceCode, 0 );
        
        cComponentReplicode *vComponent = new cComponentReplicode( vCode, vSourceCode.str() );
        vComponentMap[ vCode ] = vComponent;
        
        for ( UNORDERED_SET<r_code::View*,r_code::View::Hash,r_code::View::Equal>::const_iterator it = vCode->views.begin(); it != vCode->views.end(); it++ )
        {
            r_code::View *vView = *it;
            int vOID = vView->code( VIEW_OID ).asOpcode();
			r_comp::Image *iPrevImage = NULL;
            cComponentReplicode *vViewComponent = new cComponentReplicode( vView, !HasView( iPrevImage, vOID ) );        
            mImageViews[ iImage ].insert( vOID );
            vViewMap[ vView ] = vViewComponent;
            vViewComponent->AddChild( vComponent );
        }
    }
    
    // Link components to views
    for ( tViewMap::iterator it = vViewMap.begin(); it != vViewMap.end(); it++ )
    {
        cComponentReplicode *vComponent = it->second;
        r_code::View *vView = vComponent->GetView();
        
        { // Link from owner group
            r_code::Code *vOwner = vView->references[0];
            tComponentMap::iterator vRefIt = vComponentMap.find( vOwner );
            if ( vRefIt == vComponentMap.end() ) continue;
            
            ASSERT( vRefIt != vComponentMap.end() ); //, "Referenced node should be in the image" );                        
            
            vRefIt->second->AddChild( vComponent );
        }
        
        // Link to view's origin
        r_code::Code *vOrigin = vView->references[1];
        if ( vOrigin )
        {   // Link to origin
            tComponentMap::iterator vRefIt = vComponentMap.find( vOrigin );
            ASSERT( vRefIt != vComponentMap.end() ); //, "Referenced node should be in the image" );            
            
            vComponent->AddChild( vRefIt->second );
        }
        
        { // Link to view's object
            r_code::Code *vCode = vView->object;
            tComponentMap::iterator vRefIt = vComponentMap.find( vCode );
            ASSERT( vRefIt != vComponentMap.end() ); //, "Referenced node should be in the image" );            
            
            vComponent->AddChild( vRefIt->second );
        }
    }
    
    // Link components to other components
    for ( tComponentMap::iterator it = vComponentMap.begin(); it != vComponentMap.end(); it++ )
    {
        cComponentReplicode *vComponent = it->second;
        r_code::Code *vCode = vComponent->GetCode();
        
        for ( size_t r=0; r<vCode->references_size(); r++ )
        {
            r_code::Code *vRef = vCode->get_reference( r );
            
            tComponentMap::iterator vRefIt = vComponentMap.find( vRef );
            ASSERT( vRefIt != vComponentMap.end() ); //, "Referenced node should be in the image" );
            
            vComponent->AddChild( vRefIt->second );
        }
    }

//	mVisor->TrackNode( NULL );

//    OnLayout(); 
#endif
}
