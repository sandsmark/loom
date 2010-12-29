#pragma warning( disable: 4244 )

#include <Core/Serializer/cSerializerXML.h>
#include <Ogre/OgreVector3.h>

using Loom::Core::cSerializerXML;

/************************************************************************/
template<>
OGREAPP_API void Loom::Core::ISerializer::Write<Ogre::Vector3>( const Ogre::Vector3 *iValue )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	vSerializer->mStream<<(iValue->x)<<','<<(iValue->y)<<','<<(iValue->z)<<"\n";
}
/************************************************************************/
template<>
OGREAPP_API void Loom::Core::ISerializer::Read<Ogre::Vector3>( Ogre::Vector3 &iTarget )
/************************************************************************/
{
	cSerializerXML *vSerializer = (cSerializerXML*)this;
	wchar_t vLine[ 1024 ];
	vSerializer->mStream.getline( vLine, 1023 );
	swscanf( vLine, L"%f,%f,%f", &iTarget.x, &iTarget.y, &iTarget.z );
}


