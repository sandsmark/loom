#include <Config/Module/cModuleConfig.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Core/App/IApp.h>
#include <Qt/qsettings.h>

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;
using Loom::Core::IApp;

using namespace Loom::Config;

/************************************************************************/
cModuleConfig::cModuleConfig()
: IModule( _T( "Config" ) )
/************************************************************************/
{
}

/************************************************************************/
void cModuleConfig::Init( void )
/************************************************************************/
{
	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleConfig startup" ) );

	IApp *vApp = IApp::GetInstance();
	mSettings = new QSettings( QSettings::UserScope, QString::fromUtf16( (ushort*)vApp->GetOrganization().ToCString() ), QString::fromUtf16( (ushort*)vApp->GetName().ToCString() ) );

	mInitialized = true;
}

/************************************************************************/
void cModuleConfig::Destroy( void )
/************************************************************************/
{
	delete mSettings;

	mInitialized = false;
}

/************************************************************************/
template< typename T >
void cModuleConfig::SetValue( const cString &iKey, const T iValue, const cString *iGroup )
/************************************************************************/
{
	if ( iGroup ) mSettings->beginGroup( QString::fromUtf16( (ushort*)iGroup->ToCString() ) );

	mSettings->setValue( QString::fromUtf16( (ushort*)iKey.ToCString() ), iValue );

	if ( iGroup ) mSettings->endGroup();
}

/************************************************************************/
template< typename T >
const void *cModuleConfig::GetValue( const cString &iKey, const T iDefault, const cString *iGroup )
/************************************************************************/
{
	const void *vResult = iDefault;

	if ( iGroup ) mSettings->beginGroup( QString::fromUtf16( (ushort*)iGroup->ToCString() ) );

	vResult = mSettings->value( QString::fromUtf16( (ushort*)iKey.ToCString() ), iDefault ).value<void*>();

	if ( iGroup ) mSettings->endGroup();

	return vResult;
}
