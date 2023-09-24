// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#include "Camera.h"
#include <ctype.h>
#include "foundation/PxMat33.h"

using namespace physx;

namespace Snippets
{

Camera::Camera(const PxVec3& eye, const PxVec3& dir)
{
	mEye = eye;
	bodyEye = &mEye;
	mDir = dir.getNormalized();
	mMouseX = 0;
	mMouseY = 0;
}

void Camera::handleMouse(int button, int state, int x, int y)
{
	PX_UNUSED(state);
	PX_UNUSED(button);
	mMouseX = x;
	mMouseY = y;
}

bool Camera::handleKey(unsigned char key, int x, int y, float speed)
{
	if (lockedCam) return false;

	PX_UNUSED(x);
	PX_UNUSED(y);

	PxVec3 viewX = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
	PxVec3 viewY = mDir.cross(PxVec3(1, 0, 0)).getNormalized();
	PxVec3 viewZ = mDir.getNormalized();
	/*switch(toupper(key))
	{
	case 'W':	mEye += mDir*0.5f*speed;		break;
	case 'S':	mEye -= mDir*0.5f*speed;		break;
	case 'A':	mEye -= viewY*0.5f*speed;		break;
	case 'D':	mEye += viewY*0.5f*speed;		break;
	default:							return false;
	}*/
	float speedModifier = 0.5f;
	//if (toupper(key) == 'W')
		//mEye += viewZ * speedModifier * speed;
	//else if (toupper(key) == 'S')
		//mEye -= viewZ * speedModifier * speed;
	//else if (toupper(key) == 'A')
		//mEye -= viewX * speedModifier * speed;
	//else if (toupper(key) == 'D')
		//mEye += viewX * speedModifier * speed;
	/*else */
	if (toupper(key) == ' ')
		mEye -= viewY * speedModifier * speed; 
	else if (toupper(key) == 'S')
		mEye += viewY * speedModifier * speed;
	else
		return false;
	return true;
}

void Camera::handleAnalogMove(float x, float y)
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();
	mEye += mDir*y;
	mEye += viewY*x;
}

void Camera::lockCam(physx::PxVec3* p)
{
	lockedCam = true;
	bodyEye = p;
}

void Camera::freeCam()
{
	lockedCam = false;
}

void Camera::updatePos()
{
	if (lockedCam) {
		//mEye = *bodyEye;
	}
}


void Camera::handleMotion(int x, int y)
{
	int dx = mMouseX - x;
	int dy = mMouseY - y;

	float speedRot = 0.5;

	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();

	PxQuat qx(PxPi * dx / 180.0f, PxVec3(0,1,0));
	mDir = qx.rotate(mDir) * speedRot;
	PxQuat qy(PxPi * dy / 180.0f, viewY);
	mDir = qy.rotate(mDir) * speedRot;

	mDir.normalize();

	mMouseX = x;
	mMouseY = y;
}

PxTransform Camera::getTransform() const
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0));

	if(viewY.normalize()<1e-6f) 
		return PxTransform(mEye);

	PxMat33 m(mDir.cross(viewY), viewY, -mDir);
	return PxTransform(mEye, PxQuat(m));
}

PxVec3 Camera::getEye() const
{ 
	return mEye; 
}

PxVec3 Camera::getDir() const
{ 
	return mDir; 
}


}

