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

#ifndef IKCHAIN_H_
#define IKCHAIN_H_

#include <Ogre/Ogre.h>
#include "IKJoint.h"

using namespace std;

/*
	This class represents an inverse kinematic chain (from root to effector).
	Chains can be created directly from an Ogre skeleton or manually.
*/

class IKChain
{
public:
	// Constructor
	IKChain(Ogre::SkeletonInstance* Skeleton, Ogre::Node* MeshNode);
	IKChain(Ogre::Node* MeshNode);
	// Destructor
	~IKChain();
	// Initialize the chain
	void Initialize();
	// Add specific Ogre::Bone to chain (starting from root)
	void PushOgreBone(Ogre::Bone* OgreBone);
	// Get effector position
	Ogre::Vector3 GetEffectorPosition();
	// Solve for target position, done seperately in each dimension
	void SolveForTargetZInv(Ogre::Vector3 Target);
	void SolveForTargetZInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll);
	void SolveForTargetYInv(Ogre::Vector3 Target);
	void SolveForTargetYInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll);
	void SolveForTargetXInv(Ogre::Vector3 Target);
	void SolveForTargetXInv(Ogre::Vector3 Target, double Pitch, double Yaw, double Roll);
	// Update Ogre skeleton
	void UpdateOgreSkeleton();
	void UpdateOgreSkeletonZ();
	void UpdateOgreSkeletonY();
	void UpdateOgreSkeletonX();
	// Get effector
	IKJoint* GetEffector();
	// Get joint at depth X
	IKJoint* GetJointAtDepth(int Depth);
protected:
	Ogre::SkeletonInstance* mOgreSkeleton;
	Ogre::Node* mMeshNode;
	IKJoint* mIKRoot;
	IKJoint* mIKEffector;
	IKJoint* mLastPush;
	int mJointCount;
	double mChainLength;
};

#endif
