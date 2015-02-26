//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/  CADIA-IK
//_/_/  Helgi Páll Helgason
//_/_/
//_/_/  Copyright(c)2009 Center for Analysis and Design of Intelligent Agents
//_/_/                   Reykjavik University
//_/_/                   All rights reserved
//_/_/
//_/_/                   http://cadia.ru.is/
//_/_/
//_/_/  Redistribution and use in source and binary forms, with or without
//_/_/  modification, is permitted provided that the following conditions 
//_/_/  are met:
//_/_/
//_/_/  - Redistributions of source code must retain the above copyright notice,
//_/_/    this list of conditions and the following disclaimer.
//_/_/
//_/_/  - Redistributions in binary form must reproduce the above copyright 
//_/_/    notice, this list of conditions and the following disclaimer in the 
//_/_/    documentation and/or other materials provided with the distribution.
//_/_/
//_/_/  - Neither the name of its copyright holders nor the names of its 
//_/_/    contributors may be used to endorse or promote products derived from 
//_/_/    this software without specific prior written permission.
//_/_/
//_/_/  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//_/_/  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//_/_/  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
//_/_/  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
//_/_/  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//_/_/  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
//_/_/  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
//_/_/  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//_/_/  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//_/_/  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#include "StdAfx.h"
#include "IKChain.h"
#include <Ogre/OgreSkeletonInstance.h>

IKChain::IKChain(Ogre::SkeletonInstance* Skeleton, Ogre::Node* MeshNode)
{
	mOgreSkeleton = Skeleton;
	mMeshNode = MeshNode;
	mChainLength = 0;
	mIKRoot = NULL;
	Initialize();
}

// For manual creation of IK chain
IKChain::IKChain(Ogre::Node* MeshNode)
{
	mMeshNode = MeshNode;
	mChainLength = 0;
	mIKRoot = NULL;
	mLastPush = NULL;
	// Initialized later
}

IKChain::~IKChain()
{
	IKJoint* oJoint = mIKEffector;
	do 
	{
		IKJoint* oJointToDelete = oJoint;
		oJoint = oJoint->GetParent();
		delete oJointToDelete;
	} while ( oJoint == NULL );

}

void IKChain::Initialize()
{
	// Get skeleton root Ogre::Bone
	Ogre::Bone* oBoneRoot = mOgreSkeleton->getRootBone();
	Ogre::Vector3 vecBonePos = oBoneRoot->getPosition();
	oBoneRoot->setManuallyControlled(true);
	// Create IK root
	mIKRoot = new IKJoint(oBoneRoot, NULL, oBoneRoot->getPosition().x + mMeshNode->getPosition().x, oBoneRoot->getPosition().y + mMeshNode->getPosition().y, oBoneRoot->getPosition().z + mMeshNode->getPosition().z);
	// Keep track of previously processed Ogre::Bone
	IKJoint* oLastIKJoint = mIKRoot;
	// Current Ogre::Bone
	Ogre::Bone* oCurrentBone = oBoneRoot;
	cout << "Current Ogre::Bone: " << oCurrentBone->getName() << endl;
	mJointCount = 1;
	// Ogre::Bone iterator
	Ogre::Node::ChildNodeIterator oIterator = oCurrentBone->getChildIterator();
	Ogre::Vector3 vecParent = oBoneRoot->_getDerivedPosition();
	while ( oIterator.hasMoreElements() ) 
	{
		oCurrentBone = (Ogre::Bone*)oIterator.getNext();
		oCurrentBone->setManuallyControlled(true);
		cout << "Current Ogre::Bone: " << oCurrentBone->getName() << endl;
		Ogre::Vector3 vecDerived = oCurrentBone->_getDerivedPosition();
		Ogre::Vector3 vecJointPos = oCurrentBone->_getDerivedPosition() + mMeshNode->getPosition();
		vecBonePos = oCurrentBone->getPosition();
		IKJoint* oNewJoint = new IKJoint(oCurrentBone, oLastIKJoint, vecJointPos.x, vecJointPos.y, vecJointPos.z);
		oLastIKJoint->SetChild(oNewJoint);
		oLastIKJoint = oNewJoint;
		oIterator = oCurrentBone->getChildIterator();
		mJointCount++;
		mChainLength += vecParent.distance(oCurrentBone->_getDerivedPosition());
		vecParent = oCurrentBone->_getDerivedPosition();
	}
	mChainLength = mChainLength;
	mIKEffector = oLastIKJoint;
	cout << "Ogre::Bone count is " << mJointCount << endl;
}

void IKChain::PushOgreBone(Ogre::Bone* OgreBone)
{
	Ogre::Bone* oBone = OgreBone;
	Ogre::Vector3 vecBonePos = oBone->getPosition();
	oBone->setManuallyControlled(true);
	IKJoint* oJoint = NULL;
	if ( mLastPush == NULL ) 
	{
		// Root
		oJoint = new IKJoint(oBone, NULL, oBone->getPosition().x + mMeshNode->getPosition().x, oBone->getPosition().y + mMeshNode->getPosition().y, oBone->getPosition().z + mMeshNode->getPosition().z);
		mIKRoot = oJoint;
		mJointCount = 1;
	}
	else
	{
		// Not root
		Ogre::Vector3 vecParent = mLastPush->GetOgreBone()->_getDerivedPosition();
		Ogre::Vector3 vecDerived = oBone->_getDerivedPosition();
		Ogre::Vector3 vecJointPos = oBone->_getDerivedPosition() + mMeshNode->getPosition();
		oJoint = new IKJoint(oBone, mLastPush, vecJointPos.x, vecJointPos.y, vecJointPos.z);
		mLastPush->SetChild(oJoint);
		mJointCount++;
		mChainLength += vecParent.distance(oBone->_getDerivedPosition());
	}
	mLastPush = oJoint;
	mIKEffector = oJoint;
	cout << "Joint count " << mJointCount << endl;
}

IKJoint* IKChain::GetEffector()
{
	return mIKRoot->GetEffector();
}

Ogre::Vector3 IKChain::GetEffectorPosition()
{
	return GetEffector()->GetPosition();
}


void IKChain::SolveForTargetZInv(Ogre::Vector3 Target)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationZInv(Target, dRate);
		dRate = dRate / 1.5;
	}
}

void IKChain::SolveForTargetZInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationZInv(Target, dRate, Pitch, Yaw, Roll);
		dRate = dRate / 1.5;
	}
}

void IKChain::SolveForTargetYInv(Ogre::Vector3 Target)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationYInv(Target, dRate);
		dRate = dRate / 1.5;
	}
}

void IKChain::SolveForTargetYInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationYInv(Target, dRate, Pitch, Yaw, Roll);
		dRate = dRate / 1.5;
	}
}

void IKChain::SolveForTargetXInv(Ogre::Vector3 Target)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationXInv(Target, dRate);
		dRate = dRate / 1.5;
	}
}

void IKChain::SolveForTargetXInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll)
{
	double dRate = Target.distance(GetEffectorPosition()) / mChainLength;
	for ( int i = 0; i < 5; i++ )
	{
		mIKRoot->GetEffector()->DoSolverIterationXInv(Target, dRate, Pitch, Yaw, Roll);
		dRate = dRate / 1.5;
	}
}

void IKChain::UpdateOgreSkeleton()
{
	IKJoint* oJoint = mIKRoot->GetChild();
	do
	{
		oJoint->GetOgreBone()->roll(Ogre::Radian(oJoint->GetRotZ()));
		oJoint->GetOgreBone()->yaw(Ogre::Radian(oJoint->GetRotY()));
		//oJoint->GetOgreBone()->yaw(Ogre::Radian(oJoint->GetRotX()));
		oJoint->ClearRot();
		oJoint = oJoint->GetChild();
	} while ( oJoint != NULL );
}

void IKChain::UpdateOgreSkeletonZ()
{
	IKJoint* oJoint = mIKRoot->GetChild();
	do
	{
		oJoint->GetOgreBone()->roll(Ogre::Radian(oJoint->GetRotZ()), Ogre::Node::TS_WORLD);
		oJoint->ClearRotZ();
		oJoint = oJoint->GetChild();
	} while ( oJoint != NULL );
}

void IKChain::UpdateOgreSkeletonY()
{
	IKJoint* oJoint = mIKRoot->GetChild();
	do
	{
		oJoint->GetOgreBone()->yaw(Ogre::Radian(oJoint->GetRotY()), Ogre::Node::TS_WORLD);
		oJoint->ClearRotY();
		oJoint = oJoint->GetChild();
	} while ( oJoint != NULL );
}

void IKChain::UpdateOgreSkeletonX()
{
	IKJoint* oJoint = mIKRoot->GetChild();
	do
	{
		oJoint->GetOgreBone()->pitch(Ogre::Radian(oJoint->GetRotX()), Ogre::Node::TS_WORLD);
		oJoint->ClearRotX();
		oJoint = oJoint->GetChild();
	} while ( oJoint != NULL );
}

IKJoint* IKChain::GetJointAtDepth(int Depth)
{
	int n = Depth;
	IKJoint* oJoint = mIKRoot;
	while ( n > 0 )
	{
		oJoint = oJoint->GetChild();
		n--;
	}
	return oJoint;
}