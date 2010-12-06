#include <MoMa/Entity/cSpew.h>
#include <Ogre/OgreHardwareBufferManager.h>
#include <Ogre/OgreHardwareIndexBuffer.h>
#include <Ogre/OgreHardwareVertexBuffer.h>
#include <Ogre/OgreMeshManager.h>
#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreSubMesh.h>
#include <Ogre/OgreEntity.h>
#include <Core/Module/cModuleManager.h>
#include <OgreApp/Module/cModuleOgreApp.h>
#include <OgreApp/Qt/MainWindow/cQMainWindow.h>
#include <OgreApp/Qt/Ogre/cQScene.h>
#include <OgreApp/Qt/Ogre/Event/Responders/cOgreResponderOnRender.h>

using namespace Loom::MoMa;

using Loom::OgreApp::cModuleOgreApp;
using Loom::OgreApp::cQMainWindow;
using Loom::OgreApp::cQScene;
using Loom::Core::cModuleManager;
using Loom::OgreApp::cOgreResponderOnRender;

int cSpew::mId = 0;

/************************************************************************/
cSpew::cSpew( const Ogre::Vector3 &iOrigin, const Ogre::Vector3 &iVelocity )
: mLines( NULL ), mOrigin( iOrigin ), mVelocity( iVelocity ), mEmitting( true )
, mDecay( 1.0f ), mSpringStrength( 0.4f ), mTurbulenceStrength( 3 )
, mTurbulenceScroll( 0.001f ), mTurbulenceScale( 128.0f )
/************************************************************************/
{
	cOgreResponderOnRender::Get().AddListener( *this );
}

/************************************************************************/
void cSpew::Init( void )
/************************************************************************/
{
	CreateLines();

	sNode vNode;
	vNode.Position = mOrigin;
	vNode.Length = 0;
	vNode.Velocity = mVelocity;
	vNode.Time = -1;
	mPoints.Add( vNode );

	mLastUpdate = GetTickCount();
}

/************************************************************************/
void cSpew::CreateLines( void )
/************************************************************************/
{
	char vTemp[ 256 ];
	sprintf( vTemp, "MoMa.Spew.%d", mId );
	mLines = Ogre::MeshManager::getSingleton().createManual( vTemp, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	// Create mesh for textured lines
	Ogre::SubMesh *vSub = mLines->createSubMesh();
	vSub->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	vSub->setMaterialName( "MoMa.Spew" );
	vSub->useSharedVertices = false;
	vSub->vertexData = new Ogre::VertexData;
	vSub->indexData  = new Ogre::IndexData;
	vSub->vertexData->vertexDeclaration->addElement( 0,  0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
	vSub->vertexData->vertexDeclaration->addElement( 0, 12, Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES, 1 );
	vSub->vertexData->vertexDeclaration->addElement( 0, 24, Ogre::VET_FLOAT4, Ogre::VES_TEXTURE_COORDINATES, 0 );

	//    const int vMaxNumLines = 30000 * 5;
	const int vMaxNumLines = 30000;
	Ogre::HardwareVertexBufferSharedPtr vVertices = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vSub->vertexData->vertexDeclaration->getVertexSize( 0 ), vMaxNumLines * 2, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
	Ogre::HardwareIndexBufferSharedPtr  vIndices  = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer ( Ogre::HardwareIndexBuffer::IT_32BIT, vMaxNumLines * 2 * sizeof( Ogre::ulong ), Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );

	vSub->vertexData->vertexBufferBinding->setBinding( 0, vVertices );
	vSub->vertexData->vertexStart = 0;
	vSub->vertexData->vertexCount = 0;

	vSub->indexData->indexBuffer = vIndices;

	mLines->_setBounds( Ogre::AxisAlignedBox( -10, -10, -10, 10, 10, 10 ) );
	mLines->_setBoundingSphereRadius( 20 );

	cModuleOgreApp *vOgre = (cModuleOgreApp*)cModuleManager::Get().GetModule( _T("OgreApp") );
	cQMainWindow *vQMainWindow = vOgre->GetMainWindow();
	cQScene *vQScene = vQMainWindow->GetScene();
	Ogre::SceneManager *vScene = vQScene->GetScene();

	char vTempEntity[ 256 ];
	sprintf( vTempEntity, "MoMa.Spew.%d", mId++ );
	Ogre::Entity *vSpewsEntity = vScene->createEntity( vTempEntity, vTemp );
	Ogre::SceneNode *vNode = vScene->getRootSceneNode()->createChildSceneNode( Ogre::Vector3( 0, 0, -20 ) );
	vNode->attachObject( vSpewsEntity );
}

/************************************************************************/
void cSpew::Draw( void )
/************************************************************************/
{
	Ogre::SubMesh *vSub = mLines->getSubMesh( 0 );
	vSub->vertexData->vertexCount = 0;
	vSub->indexData->indexCount = 0;

	if ( mPoints.GetSize() < 2 ) return;

	// Fill vertices
	Ogre::HardwareVertexBufferSharedPtr vVertices = vSub->vertexData->vertexBufferBinding->getBuffer(0);

	int vVertexSize = vVertices->getVertexSize();
	int vLockSize = vVertexSize * ( mPoints.GetSize() ) * 2;
	vLockSize;
	sVertex *vVtx = (sVertex*)( vVertices->lock( vVertices->getVertexSize() * vSub->vertexData->vertexCount, vVertices->getVertexSize() * ( mPoints.GetSize() ) * 2, Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );

	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		Ogre::Vector3 vPrev = ( i == 0 ) ? mPoints[i].Position : mPoints[i-1].Position;

		float vAlpha = mPoints[i].Time;
		if ( vAlpha < 0 ) vAlpha = 0;
		if ( vAlpha > 1 ) vAlpha = 1;
		vAlpha = 1;

		float vWidth = 2;
		float vLength = mPoints[i].Length * ( 48.0f / 716.0f ) / vWidth / 2.0f;
		Ogre::Vector3 vNext = ( i == mPoints.GetSize() - 1 ) ? mPoints[i].Position : mPoints[i+1].Position;
		Ogre::Vector3 vDirection = vNext - vPrev; vDirection.normalise();
		vVtx->Pos = mPoints[i].Position;
		vVtx->Dir = vDirection;
		vVtx->Tex = Ogre::Vector4( vLength, 0, vWidth, vAlpha );
		vVtx++;
		*vVtx = vVtx[-1];
		vVtx->Tex = Ogre::Vector4( vLength, 1, vWidth, vAlpha );
		vVtx++;
	}

	vVertices->unlock();

	// Fill indices
	Ogre::HardwareIndexBufferSharedPtr vIndices = vSub->indexData->indexBuffer;
	Ogre::ulong *vIdx = (Ogre::ulong*)( vIndices->lock( vSub->indexData->indexCount * sizeof( Ogre::ulong ), sizeof( Ogre::ulong ) * ( mPoints.GetSize() * 6 ), Ogre::HardwareBuffer::HBL_NO_OVERWRITE ) );

	int vBase = 0;
	for ( size_t s=0; s<mPoints.GetSize()-1; s++ )
	{
		int vSegmentBase = vBase + s * 2;

		*vIdx++ = vSegmentBase + 1;
		*vIdx++ = vSegmentBase + 0;
		*vIdx++ = vSegmentBase + 3;

		*vIdx++ = vSegmentBase + 3;
		*vIdx++ = vSegmentBase + 2;
		*vIdx++ = vSegmentBase + 0;
	}

	vIndices->unlock();

	vSub->vertexData->vertexCount += ( mPoints.GetSize()     ) * 2;
	vSub->indexData->indexCount   += ( mPoints.GetSize() - 1 ) * 6;    
}

/************************************************************************/
void cSpew::OnRender( void )
/************************************************************************/
{
	if ( mLines.isNull() )
	{
		Init();
	}

	Update();
	Draw();

	if ( mPoints[0].Time > 10 ) StopEmitting();
}

/************************************************************************/
void cSpew::Update( void )
/************************************************************************/
{
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	if ( vEllapsed <= 0.0f ) return;

#if 0
	cArray<sNode> vResult;
	vResult = mPoints;
	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		if ( i > 0 )
		{
			float vLength = ( mPoints[i-1].InitialPosition - mPoints[i].InitialPosition ).length();
			Ogre::Vector3 vTarget = mPoints[i].Position - mPoints[i-1].Position;
			vTarget.normalise();
			vTarget = mPoints[i-1].Position + ( vTarget * vLength );
			Ogre::Vector3 vDirection = vTarget - mPoints[i].Position;
			float vAlpha = vEllapsed;
			if ( vAlpha > 1 ) vAlpha = 1;
//			vResult[i].Position += ( vDirection * vLength ) * vAlpha;
		}
		if ( i < mPoints.GetSize() - 1 )
		{
			float vLength = ( mPoints[i+1].InitialPosition - mPoints[i].InitialPosition ).length();
			Ogre::Vector3 vTarget = mPoints[i].Position - mPoints[i+1].Position;
			vTarget.normalise();
			vTarget = mPoints[i+1].Position + ( vTarget * vLength );
			Ogre::Vector3 vDirection = vTarget - mPoints[i].Position;
			float vAlpha = vEllapsed;
			if ( vAlpha > 1 ) vAlpha = 1;
			vResult[i].Position += ( vDirection * vLength ) * vAlpha;
		}
		vResult[i].Position += mPoints[i].Velocity * vEllapsed;
	}
	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		mPoints[i].Position = vResult[i].Position;
		float vDecay = 1 - ( vEllapsed / mDecay );
		if ( vDecay < 0 ) vDecay = 0;
		//		mPoints[i].Velocity *= vDecay;
	}
#else
	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		Ogre::Vector3 vDisplacement = GetTurbulence( mPoints[i].Position );
		Ogre::Vector3 vVelocityDir = mPoints[i].Velocity.normalisedCopy();
		float vDot = vVelocityDir.dotProduct( vDisplacement );
		vDisplacement -= vVelocityDir * vDot;
		vDisplacement *= mTurbulenceStrength;
		mPoints[i].Velocity += vDisplacement * vEllapsed; 
		mPoints[i].Time += vEllapsed;
	}
	cArray<sNode> vResult;
	vResult = mPoints;
	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		if ( i > 0 )
		{
			float vTargetLength = mPoints[i].PrevLength;
			float vLength = ( mPoints[i-1].Position - mPoints[i].Position ).length();
			Ogre::Vector3 vDir = mPoints[i-1].Position - mPoints[i].Position;
			float vDirLength = vDir.normalise();
			if ( vDirLength < 0.0001f ) continue;
			vTargetLength; vLength;
//			mPoints[i].Velocity += vDir * ( vLength - vTargetLength ) * mSpringStrength;
			float vStrength = ( vLength - vTargetLength ) * mSpringStrength;
			if ( vStrength >  1 ) vStrength =  1;
			if ( vStrength < -1 ) vStrength = -1;
			vResult[i].Position += vDir * vStrength;
//			mPoints[i].Velocity += vDir * vStrength;
		}
		if ( i < mPoints.GetSize() - 1 )
		{
			float vTargetLength = mPoints[i].NextLength;
			float vLength = ( mPoints[i+1].Position - mPoints[i].Position ).length();
			Ogre::Vector3 vDir = mPoints[i+1].Position - mPoints[i].Position;
			float vDirLength = vDir.normalise();
			if ( vDirLength < 0.0001f ) continue;
			vTargetLength; vLength;
//			mPoints[i].Velocity += vDir * ( vLength - vTargetLength ) * mSpringStrength;
			float vStrength = ( vLength - vTargetLength ) * mSpringStrength;
			if ( vStrength >  1 ) vStrength =  1;
			if ( vStrength < -1 ) vStrength = -1;
			vResult[i].Position += vDir * vStrength;
//			mPoints[i].Velocity += vDir * vStrength;
		}
		vResult[i].Position += mPoints[i].Velocity * vEllapsed;
	}
	for ( size_t i=0; i<mPoints.GetSize(); i++ )
	{
		mPoints[i].Position  = vResult[i].Position;
	}
//	mPoints[10].Velocity += Ogre::Vector3( 0, 1, 0 ) * vEllapsed;

#endif

	if ( !mEmitting ) return;

	float vMinDistance = 4;

	sNode &vLastNode = mPoints[ mPoints.GetSize() - 1 ];
	float vLength = ( vLastNode.Position - mOrigin ).length();
	if ( vLength > vMinDistance )
	{
		sNode vNode;
		vNode.Position = mOrigin;
		vNode.PrevLength = vLastNode.NextLength = vLength;
		vNode.Length = vLastNode.Length + vLength;
		vNode.Velocity = mVelocity;
		vNode.Time = -1;

		mPoints.Add( vNode );
	}
}

/************************************************************************/
void cSpew::StopEmitting( void )
/************************************************************************/
{
	mEmitting = false;
}

/************************************************************************/
Ogre::Vector2 cSpew::GetNoise( int iX, int iY, int iSeed ) 
/************************************************************************/
{
	int vOldSeed = rand();

	Ogre::Vector2 vResult;

	srand( iSeed + iX + ( iY << 15 ) );
	vResult.x = ( rand() % 2000 ) / 1000.0f - 1;
	srand( ( iSeed + iX + ( iY << 15 ) ) << 1 );
	vResult.y = ( rand() % 2000 ) / 1000.0f - 1;

	srand( vOldSeed );

	return vResult;
}

/************************************************************************/
Ogre::Vector3 cSpew::GetTurbulence( const Ogre::Vector3 &iPosition, float iPersistence )
/************************************************************************/
{	// Simple 2D perlin noise with 2 components
	DWORD vTime = GetTickCount();
	float vEllapsed = ( vTime - mLastUpdate ) * 0.001f;
	mLastUpdate = vTime;

	Ogre::Vector3 vResult( 0, 0, 0 );
	float vStrength = 1;
	for ( int l=7; l-->0; )
	{
		int vPower = 1<<l;
		Ogre::Vector3 vPos = iPosition * mTurbulenceScale / (float)vPower;
		vPos.x += vEllapsed * mTurbulenceScroll;
		vPos.y += vEllapsed * mTurbulenceScroll;
		int vX = (int)vPos.x;
		int vY = (int)vPos.y;
		Ogre::Vector2 tl = GetNoise( vX    , vY    , l ); 
		Ogre::Vector2 tr = GetNoise( vX + 1, vY    , l ); 
		Ogre::Vector2 bl = GetNoise( vX    , vY + 1, l ); 
		Ogre::Vector2 br = GetNoise( vX    , vY + 1, l ); 
		Ogre::Vector2 vTopD = ( tr - tl );
		Ogre::Vector2 vTop = tl + vTopD * fmodf( vPos.x, 1 );
		Ogre::Vector2 vBottomD = ( br - bl );
		Ogre::Vector2 vBottom = bl + vBottomD * fmodf( vPos.x, 1 );
		Ogre::Vector2 vDiff = ( vBottom - vTop );
		Ogre::Vector2 vRand = vTop + vDiff * fmodf( vPos.y, 1 );

		vResult.x += vRand.x * vStrength;
		vResult.y += vRand.y * vStrength;
		vStrength *= iPersistence;
	}

	return vResult;
}
