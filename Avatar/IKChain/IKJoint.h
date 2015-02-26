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

#ifndef IKJOINT_H_
#define IKJOINT_H_

#include <math.h>
#include <Ogre/Ogre.h>

using namespace std;

/*
	This class represents an inverse kinematic joint, i.e. the joint of two bones.
*/

class IKJoint
{
public:
	IKJoint(Ogre::Bone* OgreBone, IKJoint* Parent, double PosX, double PosY, double PosZ);
	void SetChild(IKJoint* Child);
	void RotateZ(double Angle);
	void RotateZ(double Angle, double OriginX, double OriginY, double OriginZ);
	void RotateY(double Angle);
	void RotateY(double Angle, double OriginX, double OriginY, double OriginZ);
	void RotateX(double Angle);
	void RotateX(double Angle, double OriginX, double OriginY, double OriginZ);
	Ogre::Vector3 GetPosition();
	IKJoint* GetEffector();
	void DoSolverIterationZInv(Ogre::Vector3 Target, double Rate);
	void DoSolverIterationYInv(Ogre::Vector3 Target, double Rate);
	void DoSolverIterationXInv(Ogre::Vector3 Target, double Rate);
	void DoSolverIterationZInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll);
	void DoSolverIterationYInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll);
	void DoSolverIterationXInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll);

	double DistanceFromEffectorXY(Ogre::Vector3 Position);
	double DistanceFromEffectorXZ(Ogre::Vector3 Position);
	double DistanceFromEffectorYZ(Ogre::Vector3 Position);
	Ogre::Bone* GetOgreBone();
	IKJoint* GetChild();
	IKJoint* GetParent();
	double GetRotZ();
	double GetRotY();
	double GetRotX();
	void ClearRot();
	void ClearRotZ();
	void ClearRotY();
	void ClearRotX();

	void SetMinJointAngleX(double Angle);
	void SetMaxJointAngleX(double Angle);
	void SetMinJointAngleY(double Angle);
	void SetMaxJointAngleY(double Angle);
	void SetMinJointAngleZ(double Angle);
	void SetMaxJointAngleZ(double Angle);
	void SetMinJointAngleXYZ(double Angle);
	void SetMaxJointAngleXYZ(double Angle);
	void SetJointAngleSymmetric(double Angle);

	void SetWeight(double Weight);
	

protected:
	void SaveCurrentState();
	void RevertPositionRotZ();
	void RevertPositionRotY();
	void RevertPositionRotX();
	IKJoint* mParent;
	IKJoint* mChild;
	double mMinJointAngleZ;
	double mMaxJointAngleZ;
	double mMinJointAngleY;
	double mMaxJointAngleY;
	double mMinJointAngleX;
	double mMaxJointAngleX;
	double mPosX;
	double mPosY;
	double mPosZ;
	double mRotZ;
	double mRotY;
	double mRotX;
	double mLastRotX;
	double mLastRotY;
	double mLastRotZ;
	double mSumRotX;
	double mSumRotY;
	double mSumRotZ;

	double mGlobalRotX;
	double mGlobalRotY;
	double mGlobalRotZ;

	double mWeight;

	Ogre::Bone* mOgreBone;
	bool mRevertToggle;

	// Saved values for enabling revertion
	double mSavedPosX;
	double mSavedPosY;
	double mSavedPosZ;
	double mSavedRotZ;
	double mSavedRotY;
	double mSavedRotX;
	double mSavedGlobalRotX;
	double mSavedGlobalRotY;
	double mSavedGlobalRotZ;

	static double mDefJointMaxAngle;

};

#endif
