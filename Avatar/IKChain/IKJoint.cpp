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
#include "IKJoint.h"

// Default min/max angle of a joint
double IKJoint::mDefJointMaxAngle = Ogre::Math::PI/4.0;

// Constructor
IKJoint::IKJoint(Ogre::Bone* OgreBone, IKJoint* Parent, double PosX, double PosY, double PosZ)
{
	mOgreBone = OgreBone;
	mParent = Parent;
	mPosX = PosX;
	mPosY = PosY;
	mPosZ = PosZ;
	mSavedPosX = 0;
	mSavedPosY = 0;
	mSavedPosZ = 0;
	mRotZ = 0;
	mRotY = 0;
	mRotX = 0;
	mChild = NULL;
	mRevertToggle = false;
	mLastRotX = 0;
	mLastRotY = 0;
	mLastRotZ = 0;
	mSumRotX = 0;
	mSumRotY = 0;
	mSumRotZ = 0;
	mGlobalRotX = 0;
	mGlobalRotY = 0;
	mGlobalRotZ = 0;
	mMinJointAngleX = -mDefJointMaxAngle;
	mMaxJointAngleX = mDefJointMaxAngle;
	mMinJointAngleY = -mDefJointMaxAngle;
	mMaxJointAngleY = mDefJointMaxAngle;
	mMinJointAngleZ = -mDefJointMaxAngle;
	mMaxJointAngleZ = mDefJointMaxAngle;
	mWeight = 1.0;
}

// Set the child of this joint
void IKJoint::SetChild(IKJoint* Child)
{
	mChild = Child;
}

// Save current state so we may revert to it later
void IKJoint::SaveCurrentState()
{
	mSavedPosX = mPosX;
	mSavedPosY = mPosY;
	mSavedPosZ = mPosZ;
	mSavedRotZ = mRotZ;
	mSavedRotY = mRotY;
	mSavedRotX = mRotX;
	mSavedGlobalRotZ = mGlobalRotZ;
	mSavedGlobalRotY = mGlobalRotY;
	mSavedGlobalRotX = mGlobalRotX;
	mRevertToggle = true;
	if ( mChild != NULL )
		mChild->SaveCurrentState();
}

// Revert to the stored state for the Z axis
void IKJoint::RevertPositionRotZ()
{
	if ( mRevertToggle ) 
	{
		mPosX = mSavedPosX;
		mPosY = mSavedPosY;
		mPosZ = mSavedPosZ;
		mRotZ = mSavedRotZ;
		mGlobalRotZ = mSavedGlobalRotZ;
	}
	mRevertToggle = false;
	if ( mChild != NULL )
		mChild->RevertPositionRotZ();
}

// Revert to the stored state for the Y axis
void IKJoint::RevertPositionRotY()
{
	if ( mRevertToggle ) 
	{
		mPosX = mSavedPosX;
		mPosY = mSavedPosY;
		mPosZ = mSavedPosZ;
		mRotY = mSavedRotY;
		mGlobalRotY = mSavedGlobalRotY;
	}
	mRevertToggle = false;
	if ( mChild != NULL )
		mChild->RevertPositionRotY();
}

// Revert to the stored state for the X axis
void IKJoint::RevertPositionRotX()
{
	if ( mRevertToggle ) 
	{
		mPosX = mSavedPosX;
		mPosY = mSavedPosY;
		mPosZ = mSavedPosZ;
		mRotX = mSavedRotX;
		mGlobalRotX = mSavedGlobalRotX;
	}
	mRevertToggle = false;
	if ( mChild != NULL )
		mChild->RevertPositionRotX();
}

// Rotation of a joint around the Z axis
void IKJoint::RotateZ(double Angle)
{
	SaveCurrentState();
	mRotZ += Angle;
	mGlobalRotZ += Angle;
	if ( mChild != NULL )
		mChild->RotateZ(Angle, mPosX, mPosY, mPosZ);
}

// Rotation of a joint around the Y axis
void IKJoint::RotateY(double Angle)
{
	SaveCurrentState();
	mRotY += Angle;
	mGlobalRotY += Angle;
	if ( mChild != NULL )
		mChild->RotateY(Angle, mPosX, mPosY, mPosZ);
}

// Rotation of a joint around the X axis
void IKJoint::RotateX(double Angle)
{
	SaveCurrentState();
	mRotX += Angle;
	mGlobalRotX += Angle;
	if ( mChild != NULL )
		mChild->RotateX(Angle, mPosX, mPosY, mPosZ);
}

// Effects of parent joint rotation on children (i.e. the actual rotation around the Z axis)
void IKJoint::RotateZ(double Angle, double OriginX, double OriginY, double OriginZ)
{
	// Rotate
	double dX = mPosX - OriginX;
	double dY = mPosY - OriginY;
	double dSin = sin(Angle);
	double dCos = cos(Angle);	
	mPosX = dX * dCos - dY * dSin;
	mPosY = dX * dSin + dY * dCos;
	mPosX += OriginX;
	mPosY += OriginY;
	if ( mChild != NULL )
		mChild->RotateZ(Angle, OriginX, OriginY, OriginZ);
}

// Effects of parent joint rotation on children (i.e. the actual rotation around the Y axis)
void IKJoint::RotateY(double Angle, double OriginX, double OriginY, double OriginZ)
{
	// Rotate
	double dX = mPosX - OriginX;
	double dZ = mPosZ - OriginZ;
	double dSin = sin(Angle);
	double dCos = cos(Angle);	
	mPosZ = dZ * dCos - dX * dSin;
	mPosX = dZ * dSin + dX * dCos;
	mPosX += OriginX;
	mPosZ += OriginZ;
	if ( mChild != NULL )
		mChild->RotateY(Angle, OriginX, OriginY, OriginZ);
}

// Effects of parent joint rotation on children (i.e. the actual rotation around the X axis)
void IKJoint::RotateX(double Angle, double OriginX, double OriginY, double OriginZ)
{
	// Rotate
	double dY = mPosY - OriginY;
	double dZ = mPosZ - OriginZ;
	double dSin = sin(Angle);
	double dCos = cos(Angle);	
	mPosY = dY * dCos - dZ * dSin;
	mPosZ = dY * dSin + dZ * dCos;
	mPosY += OriginY;
	mPosZ += OriginZ;
	if ( mChild != NULL )
		mChild->RotateX(Angle, OriginX, OriginY, OriginZ);
}

// Get effector joint
IKJoint* IKJoint::GetEffector()
{
	if ( this->mChild == NULL )
		return this;
	else 
		return this->mChild->GetEffector();
}

// Get joint position in world coordinates
Ogre::Vector3 IKJoint::GetPosition()
{
	return Ogre::Vector3(mPosX, mPosY, mPosZ);
}

// Perform a solver iteration in the XY plane
void IKJoint::DoSolverIterationZInv(Ogre::Vector3 Target, double Rate)
{
	double dRate = (Ogre::Math::PI / 18.0) * Rate;
	dRate = dRate * mWeight;
	// Process XY plane
	double dDistO = DistanceFromEffectorXY(Target);
	double dDistP;
	double dDistN;
	if ( mGlobalRotZ + mRotZ + dRate > mMaxJointAngleZ )
		dDistP = dDistO;
	else
	{
		RotateZ(dRate);
		dDistP = DistanceFromEffectorXY(Target);
		RevertPositionRotZ();
	}
	if ( mGlobalRotZ + mRotZ - dRate < mMinJointAngleZ )
		dDistN = dDistO;
	else
	{
		RotateZ(-dRate);
		dDistN = DistanceFromEffectorXY(Target);
		RevertPositionRotZ();	
	}
	if ( dDistP < dDistO )
		RotateZ(dRate);
	else if ( dDistN < dDistO )
		RotateZ(-dRate);
	if ( mParent != NULL && mParent->mParent != NULL)
		mParent->DoSolverIterationZInv(Target, Rate);
}

// Perform a solver iteration in the XY plane
void IKJoint::DoSolverIterationZInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll)
{
	double dRate = (Ogre::Math::PI / 18.0) * Rate;
	dRate = dRate * mWeight;
	IKJoint* oEffector = GetEffector();
	// Process XY plane
	double dDistO = DistanceFromEffectorXY(Target);
	double dDistP;
	double dDistN;
	Ogre::Vector3 oRotationsP;
	Ogre::Vector3 oRotationsN;	
	bool bAllowP = false;
	bool bAllowN = false;
	// Enforce limits
	if ( mGlobalRotZ + mRotZ + dRate > mMaxJointAngleZ )
		dDistP = dDistO;
	else
	{
		bAllowP = true;
		RotateZ(dRate);
		dDistP = DistanceFromEffectorXY(Target);
		oRotationsP.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsP.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsP.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotZ();
	}
	if ( mGlobalRotZ + mRotZ - dRate < mMinJointAngleZ )
		dDistN = dDistO;
	else
	{
		bAllowN = true;
		RotateZ(-dRate);
		dDistN = DistanceFromEffectorXY(Target);
		oRotationsN.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsN.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsN.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotZ();	
	}
	if ( bAllowP && dDistP <= dDistO &&
		 abs(oRotationsP.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsP.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsP.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll)
		 )
		RotateZ(dRate);
	else 
	if ( bAllowN && dDistN <= dDistO &&
		 abs(oRotationsN.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsN.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsN.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll)
		)
		RotateZ(-dRate);
	if ( mParent != NULL && mParent->mParent != NULL)
		mParent->DoSolverIterationZInv(Target, Rate, Pitch, Yaw, Roll);
}

// Perform a solver iteration in the XZ plane
void IKJoint::DoSolverIterationYInv(Ogre::Vector3 Target, double Rate)
{	
	double dRate = (Ogre::Math::PI / 18.0) * Rate; 
	dRate = dRate * mWeight;
	// Process XZ plane
	double dDistO = DistanceFromEffectorXZ(Target);
	double dDistP;
	double dDistN;
	if ( mGlobalRotY + mRotY + dRate > mMaxJointAngleY ) 
		dDistP = dDistO;
	else
	{
		RotateY(dRate);
		dDistP = DistanceFromEffectorXZ(Target);
		RevertPositionRotY();
	}
	if ( mGlobalRotX + mRotY - dRate < mMinJointAngleY )
		dDistN = dDistO;
	else
	{
		RotateY(-dRate);
		dDistN = DistanceFromEffectorXZ(Target);
		RevertPositionRotY();
	}
	if ( dDistP < dDistO )
		RotateY(dRate);
	else if ( dDistN < dDistO )
		RotateY(-dRate);

	if ( mParent != NULL && mParent->mParent != NULL )
		mParent->DoSolverIterationYInv(Target, Rate);
}

// Perform a solver iteration in the XZ plane
void IKJoint::DoSolverIterationYInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll)
{	
	double dRate = (Ogre::Math::PI / 18.0) * Rate; 
	dRate = dRate * mWeight;
	IKJoint* oEffector = GetEffector();
	// Process XZ plane
	double dDistO = DistanceFromEffectorXZ(Target);
	double dDistP;
	double dDistN;
	Ogre::Vector3 oRotationsP;
	Ogre::Vector3 oRotationsN;
	bool bAllowP = false;
	bool bAllowN = false;
	if ( mSumRotY + mRotY + dRate > mMaxJointAngleY ) 
		dDistP = dDistO;
	else
	{
		bAllowP = true;
		RotateY(dRate);
		dDistP = DistanceFromEffectorXZ(Target);
		oRotationsP.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsP.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsP.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotY();
	}
	if ( mSumRotX + mRotY - dRate < mMinJointAngleY )
		dDistN = dDistO;
	else
	{
		bAllowN = true;
		RotateY(-dRate);
		dDistN = DistanceFromEffectorXZ(Target);
		oRotationsN.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsN.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsN.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotY();
	}
	if ( bAllowP && dDistP <= dDistO &&
		 abs(oRotationsP.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsP.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsP.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll)
		 )
		RotateY(dRate);
	else if ( bAllowN && dDistN <= dDistO &&
		 abs(oRotationsN.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsN.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsN.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll)
		 )
		RotateY(-dRate);

	if ( mParent != NULL && mParent->mParent != NULL )
		mParent->DoSolverIterationYInv(Target, Rate, Pitch, Yaw, Roll);
}

// Perform a solver iteration in the YZ plane
void IKJoint::DoSolverIterationXInv(Ogre::Vector3 Target, double Rate)
{
	double dRate = (Ogre::Math::PI / 18.0) * Rate; 
	dRate = dRate * mWeight;
	// Process XY plane
	double dDistO = DistanceFromEffectorYZ(Target);
	double dDistP;
	double dDistN;
	if ( mSumRotX + mRotX + dRate > mMaxJointAngleX ) 
		dDistP = dDistO;
	else
	{
		RotateX(dRate);
		dDistP = DistanceFromEffectorYZ(Target);
		RevertPositionRotX();
	}
	if ( mSumRotX + mRotX - dRate < mMinJointAngleX )
		dDistN = dDistO;
	else
	{
		RotateX(-dRate);
		dDistN = DistanceFromEffectorYZ(Target);
		RevertPositionRotX();	
	}
	if ( dDistP < dDistO )
		RotateX(dRate);
	else if ( dDistN < dDistO )
		RotateX(-dRate);
	if ( mParent != NULL && mParent->mParent != NULL)
		mParent->DoSolverIterationXInv(Target, Rate);
}

// Perform a solver iteration in the YZ plane
void IKJoint::DoSolverIterationXInv(Ogre::Vector3 Target, double Rate, double Pitch, double Yaw, double Roll)
{
	double dRate = (Ogre::Math::PI / 18.0) * Rate; 
	dRate = dRate * mWeight;
	IKJoint* oEffector = GetEffector();
	// Process XY plane
	double dDistO = DistanceFromEffectorYZ(Target);
	double dDistP;
	double dDistN;
	Ogre::Vector3 oRotationsP;
	Ogre::Vector3 oRotationsN;	
	bool bAllowP = true;
	bool bAllowN = true;
	if ( mSumRotX + mRotX + dRate > mMaxJointAngleX ) 
		dDistP = dDistO;
	else
	{
		bAllowP = true;
		RotateX(dRate);
		dDistP = DistanceFromEffectorYZ(Target);
		oRotationsP.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsP.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsP.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotX();
	}
	if ( mSumRotX + mRotX - dRate < mMinJointAngleX )
		dDistN = dDistO;
	else
	{
		bAllowN = true;
		RotateX(-dRate);
		dDistN = DistanceFromEffectorYZ(Target);
		oRotationsN.x = oEffector->mSumRotX + oEffector->mRotX;
		oRotationsN.y = oEffector->mSumRotY + oEffector->mRotY;
		oRotationsN.z = oEffector->mSumRotZ + oEffector->mRotZ;
		RevertPositionRotX();	
	}
	if ( bAllowP && dDistP <= dDistO &&
		 abs(oRotationsP.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsP.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsP.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll))
		RotateX(dRate);
	else if ( bAllowN && dDistN <= dDistO &&
		 abs(oRotationsP.x - Pitch) <= abs(oEffector->mSumRotX + oEffector->mRotX - Pitch) &&
		 abs(oRotationsP.y - Yaw) <= abs(oEffector->mSumRotY + oEffector->mRotY - Yaw) &&
		 abs(oRotationsP.z - Roll) <= abs(oEffector->mSumRotZ + oEffector->mRotZ - Roll) )
		RotateX(-dRate);
	if ( mParent != NULL && mParent->mParent != NULL)
		mParent->DoSolverIterationXInv(Target, Rate, Pitch, Yaw, Roll);
}

// Get distance from effector in XY plane
double IKJoint::DistanceFromEffectorXY(Ogre::Vector3 Position)
{
	Ogre::Vector3 vecEffector = GetEffector()->GetPosition();
	double dRes = sqrt(pow(Position.x - vecEffector.x,2) + pow(Position.y - vecEffector.y,2));
	return dRes;
}

// Get distance from effector in XZ plane
double IKJoint::DistanceFromEffectorXZ(Ogre::Vector3 Position)
{
	Ogre::Vector3 vecEffector = GetEffector()->GetPosition();
	double dRes = sqrt(pow(Position.x - vecEffector.x,2) + pow(Position.z - vecEffector.z,2));
	return dRes;
}

// Get distance from effector in YZ plane
double IKJoint::DistanceFromEffectorYZ(Ogre::Vector3 Position)
{
	Ogre::Vector3 vecEffector = GetEffector()->GetPosition();
	double dRes = sqrt(pow(Position.y - vecEffector.y,2) + pow(Position.z - vecEffector.z,2));
	return dRes;
}

// Get Ogre Ogre::Bone for this joint
Ogre::Bone* IKJoint::GetOgreBone()
{
	return mOgreBone;
}

// Get the child of this joint
IKJoint* IKJoint::GetChild()
{
	return mChild;
}

// Get the parent of this joint
IKJoint* IKJoint::GetParent()
{
	return mParent;
}

double IKJoint::GetRotZ()
{
	return mRotZ;
}

double IKJoint::GetRotY()
{
	return mRotY;
}

double IKJoint::GetRotX()
{
	return mRotX;
}

void IKJoint::ClearRot()
{
	mRotZ = 0;
	mRotY = 0;
	mRotX = 0;
}

void IKJoint::ClearRotZ()
{
	mSumRotZ += mRotZ;
	mRotZ = 0;
}

void IKJoint::ClearRotY()
{
	mSumRotY += mRotY;
	mRotY = 0;
}

void IKJoint::ClearRotX()
{
	mSumRotX += mRotX;
	mRotX = 0;
}

void IKJoint::SetMinJointAngleX(double Angle) { mMinJointAngleX = Angle; }
void IKJoint::SetMaxJointAngleX(double Angle) { mMaxJointAngleX = Angle; }
void IKJoint::SetMinJointAngleY(double Angle) { mMinJointAngleY = Angle; }
void IKJoint::SetMaxJointAngleY(double Angle) { mMaxJointAngleY = Angle; }
void IKJoint::SetMinJointAngleZ(double Angle) { mMinJointAngleZ = Angle; }
void IKJoint::SetMaxJointAngleZ(double Angle) { mMaxJointAngleZ = Angle; }

void IKJoint::SetMinJointAngleXYZ(double Angle)
{
	SetMinJointAngleX(Angle);
	SetMinJointAngleY(Angle);
	SetMinJointAngleZ(Angle);
}

void IKJoint::SetMaxJointAngleXYZ(double Angle)
{
	SetMaxJointAngleX(Angle);
	SetMaxJointAngleY(Angle);
	SetMaxJointAngleZ(Angle);
}

void IKJoint::SetJointAngleSymmetric(double Angle)
{
	SetMinJointAngleXYZ(-Angle);
	SetMaxJointAngleXYZ(Angle);
}

void IKJoint::SetWeight(double Weight)
{
	mWeight = Weight;
}