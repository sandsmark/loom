#include <Core/Serializer/cSerializerXML.h>
#include <strsafe.h>
#include <Core/Debug/Logger/cLogger.h>

using namespace Loom::Core;

using Loom::Core::cLogger;

/************************************************************************/
void cSerializerXML::Serialize( const IObject *iObject )
/************************************************************************/
{
	const cClass &iClass = iObject->GetRTTI();
	mStream<<iClass.GetName().ToCString()<<":\n";
	for ( size_t i=0; i<iClass.GetNumProperties(); i++ )
	{
		const cProperty &vProperty = iClass.GetProperty(i);
		mStream<<"\t"<<vProperty.GetName().ToCString()<<"=";
		vProperty.Write( this, iObject );
	}
	mStream<<"\n";
}

/************************************************************************/
IObject *cSerializerXML::Deserialize( void )
/************************************************************************/
{
	std::wstring vLine;	
	mStream>>vLine;
	vLine = vLine.substr( 0, vLine.length() - 1 );
	const cClass *vClass = cClassRegistry::Get().FindClass( vLine.c_str() );
	if ( !vClass )
	{
		cLogger &vLogger = cLogger::Get();
		TCHAR vTemp[ 1024 ];
		StringCchPrintf( vTemp, 1024, _T("Can't find class: '%s'"), vLine );
		vLogger.Log( cLogger::LOG_WARNING, _T( "Global" ), vTemp );
		return NULL;
	}
	IObject *vResult = vClass->Create();
	for ( size_t i=0; i<vClass->GetNumProperties(); i++ )
	{
		const cProperty &vProperty = vClass->GetProperty(i);
		TCHAR vTemp[ 1024 ];
		mStream.getline( vTemp, 1024, '=' );
		vLine = vTemp;
		size_t vIdx = vLine.find_last_of( _T(" \n\r\t") ) + 1;
		vLine = vLine.substr( vIdx );
		cString vLineString( vLine.c_str() );
		if ( vProperty.GetName() != vLineString )
		{
			cLogger &vLogger = cLogger::Get();
			TCHAR vTemp[ 1024 ];
			StringCchPrintf( vTemp, 1024, _T("Class '%s' is in invalid format."), vLine );
			vLogger.Log( cLogger::LOG_WARNING, _T( "Global" ), vTemp );
			delete vResult;
			return NULL;
		}

		vProperty.Read( this, vResult );
	}

	return vResult;
}

// Implementation
/************************************************************************/
template<>
void CORE_API Loom::Core::ISerializer::Write<int>( const int *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream<<(*iValue)<<"\n";
}

/************************************************************************/
template<>
void CORE_API Loom::Core::ISerializer::Write<float>( const float *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream<<(*iValue)<<"\n";
}

/************************************************************************/
template<>
void CORE_API Loom::Core::ISerializer::Write<bool>( const bool *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	boolalpha( vSerializer->mStream );
	vSerializer->mStream<<(*iValue)<<"\n";
}

typedef char* tStringBaszA;
typedef wchar_t* tStringBaszW;

/************************************************************************/
template<>
void CORE_API Loom::Core::ISerializer::Write<tStringBaszA>( const tStringBaszA *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream<<(*iValue)<<"\n";
}

/************************************************************************/
template<>
void CORE_API Loom::Core::ISerializer::Write<tStringBaszW>( const tStringBaszW *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream<<(*iValue)<<"\n";
}

/************************************************************************/
template<>
CORE_API void Loom::Core::ISerializer::Read<int>( int &iTarget )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream>>iTarget;
}

/************************************************************************/
template<>
CORE_API void Loom::Core::ISerializer::Read<float>( float &iTarget )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream>>iTarget;
}

/************************************************************************/
template<>
CORE_API void Loom::Core::ISerializer::Read<bool>( bool &iTarget )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	boolalpha( vSerializer->mStream );
	vSerializer->mStream>>iTarget;
}

/************************************************************************/
template<>
CORE_API void Loom::Core::ISerializer::Read<wchar_t *>( wchar_t * &iTarget )
/************************************************************************/
{
	TCHAR vTemp[ 1024 ];
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream.getline( vTemp, 1024 );
	size_t vLength = wcslen( vTemp ) + 1;
	iTarget = new wchar_t[ vLength ];
	memcpy( iTarget, vTemp, vLength * sizeof( wchar_t ) );
}

/************************************************************************/
template<>
CORE_API void Loom::Core::ISerializer::Read<char *>( char * &iTarget )
/************************************************************************/
{
	TCHAR vTemp[ 1024 ];
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream.getline( vTemp, 1024 );
	size_t vLength = wcslen( vTemp ) + 1;
	iTarget = new char[ vLength ];
	size_t vConverted;
	wcstombs_s( &vConverted, iTarget, vLength, vTemp, vLength );
}

