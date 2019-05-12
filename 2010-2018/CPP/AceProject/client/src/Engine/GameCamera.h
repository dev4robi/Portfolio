#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__

#include "Library/dhpoware/mathlib.h"
#include "Engine/GameObject.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : GameCamera (v2)
*	- Last Update : 2016.06.30
*	- Library by : http://www.dhpoware.com/source/mathlib.html
*
*	- Introduction : 게임의 특정 위치를 보여주기 위해 사용하는 카메라.
*					 카메라가 바라보는 방향으로 화면에 그리기 작업이 수행된다.
*					 dhpoware 수학 라이브러리 및 카메라 라이브러리를 사용.
*
*	- Dependency : [StdHeader.h], camera.h
*
*   - Version(v1): 게임 카메라 기초 설계 (2016.04.13)
*	- Version(v2): 기존 카메라 클래스를 dhpoware 카메라 라이브러리로 교체 (2016.06.30)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//-----------------------------------------------------------------------------
// Copyright (c) 2007-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// A general purpose 6DoF (six degrees of freedom) quaternion based camera.
//
// This camera class supports 4 different behaviors:
// first person mode, spectator mode, flight mode, and orbit mode.
//
// First person mode only allows 5DOF (x axis movement, y axis movement, z axis
// movement, yaw, and pitch) and movement is always parallel to the world x-z
// (ground) plane.
//
// Spectator mode is similar to first person mode only movement is along the
// direction the camera is pointing.
// 
// Flight mode supports 6DoF. This is the camera class' default behavior.
//
// Orbit mode rotates the camera around a target position. This mode can be
// used to simulate a third person camera. Orbit mode supports 2 modes of
// operation: orbiting about the target's Y axis, and free orbiting. The former
// mode only allows pitch and yaw. All yaw changes are relative to the target's
// local Y axis. This means that camera yaw changes won't be affected by any
// rolling. The latter mode allows the camera to freely orbit the target. The
// camera is free to pitch, yaw, and roll. All yaw changes are relative to the
// camera's orientation (in space orbiting the target).
//
// This camera class allows the camera to be moved in 2 ways: using fixed
// step world units, and using a supplied velocity and acceleration. The former
// simply moves the camera by the specified amount. To move the camera in this
// way call one of the move() methods. The other way to move the camera
// calculates the camera's displacement based on the supplied velocity,
// acceleration, and elapsed time. To move the camera in this way call the
// updatePosition() method.
//-----------------------------------------------------------------------------

class GameCamera : public GameObject {
public:
	enum CameraBehavior
	{
		CAMERA_BEHAVIOR_FIRST_PERSON,
		CAMERA_BEHAVIOR_SPECTATOR,
		CAMERA_BEHAVIOR_FLIGHT,
		CAMERA_BEHAVIOR_ORBIT
	};

	GameCamera();
	~GameCamera();

	void lookAt(const Vector3 &target);
	void lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);
	void move(float dx, float dy, float dz);
	void move(const Vector3 &direction, const Vector3 &amount);
	void perspective(float fovx, float aspect, float znear, float zfar);
	void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);
	void rotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees);
	void undoRoll();
	void updatePosition(const Vector3 &direction, float elapsedTimeSec);
	void zoom(float zoom, float minZoom, float maxZoom);

	// Getter methods.

	const Vector3 &getAcceleration() const;
	CameraBehavior getBehavior() const;
	const Vector3 &getCurrentVelocity() const;
	const Vector3 &getPosition() const;
	float getOrbitMinZoom() const;
	float getOrbitMaxZoom() const;
	float getOrbitOffsetDistance() const;
	float getOrbitPitchMaxDegrees() const;
	float getOrbitPitchMinDegrees() const;
	const Quaternion &getOrientation() const;
	float getRotationSpeed() const;
	const Matrix4 &getProjectionMatrix() const;
	const Vector3 &getVelocity() const;
	const Vector3 &getViewDirection() const;
	const Matrix4 &getViewMatrix() const;
	const Matrix4 &getViewProjectionMatrix() const;
	const Vector3 &getXAxis() const;
	const Vector3 &getYAxis() const;
	const Vector3 &getZAxis() const;
	bool preferTargetYAxisOrbiting() const;

	// Setter methods.

	void setAcceleration(const Vector3 &acceleration);
	void setBehavior(CameraBehavior newBehavior);
	void setCurrentVelocity(const Vector3 &currentVelocity);
	void setCurrentVelocity(float x, float y, float z);
	void setOrbitMaxZoom(float orbitMaxZoom);
	void setOrbitMinZoom(float orbitMinZoom);
	void setOrbitOffsetDistance(float orbitOffsetDistance);
	void setOrbitPitchMaxDegrees(float orbitPitchMaxDegrees);
	void setOrbitPitchMinDegrees(float orbitPitchMinDegrees);
	void setOrientation(const Quaternion &newOrientation);
	void setPosition(const Vector3 &newEye);
	void setPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
	void setRotationSpeed(float rotationSpeed);
	void setVelocity(const Vector3 &velocity);
	void setVelocity(float x, float y, float z);

private:
	void rotateFirstPerson(float headingDegrees, float pitchDegrees);
	void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
	void rotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees);
	void updateVelocity(const Vector3 &direction, float elapsedTimeSec);
	void updateViewMatrix();

	static const float DEFAULT_ROTATION_SPEED;
	static const float DEFAULT_FOVX;
	static const float DEFAULT_ZNEAR;
	static const float DEFAULT_ZFAR;
	static const float DEFAULT_ORBIT_MIN_ZOOM;
	static const float DEFAULT_ORBIT_MAX_ZOOM;
	static const float DEFAULT_ORBIT_OFFSET_DISTANCE;
	static const Vector3 WORLD_XAXIS;
	static const Vector3 WORLD_YAXIS;
	static const Vector3 WORLD_ZAXIS;

	CameraBehavior m_behavior;
	bool m_preferTargetYAxisOrbiting;
	float m_accumPitchDegrees;
	float m_savedAccumPitchDegrees;
	float m_rotationSpeed;
	float m_fovx;
	float m_aspectRatio;
	float m_znear;
	float m_zfar;
	float m_orbitMinZoom;
	float m_orbitMaxZoom;
	float m_orbitOffsetDistance;
	float m_firstPersonYOffset;
	Vector3 m_eye;
	Vector3 m_savedEye;
	Vector3 m_target;
	Vector3 m_targetYAxis;
	Vector3 m_xAxis;
	Vector3 m_yAxis;
	Vector3 m_zAxis;
	Vector3 m_viewDir;
	Vector3 m_acceleration;
	Vector3 m_currentVelocity;
	Vector3 m_velocity;
	Quaternion m_orientation;
	Quaternion m_savedOrientation;
	Matrix4 m_viewMatrix;
	Matrix4 m_projMatrix;
	Matrix4 m_viewProjMatrix;
};

//-----------------------------------------------------------------------------

inline const Vector3 &GameCamera::getAcceleration() const {
	return m_acceleration;
}

inline GameCamera::CameraBehavior GameCamera::getBehavior() const {
	return m_behavior;
}

inline const Vector3 &GameCamera::getCurrentVelocity() const {
	return m_currentVelocity;
}

inline const Vector3 &GameCamera::getPosition() const {
	return m_eye;
}

inline float GameCamera::getOrbitMinZoom() const {
	return m_orbitMinZoom;
}

inline float GameCamera::getOrbitMaxZoom() const {
	return m_orbitMaxZoom;
}

inline float GameCamera::getOrbitOffsetDistance() const {
	return m_orbitOffsetDistance;
}

inline const Quaternion &GameCamera::getOrientation() const {
	return m_orientation;
}

inline float GameCamera::getRotationSpeed() const {
	return m_rotationSpeed;
}

inline const Matrix4 &GameCamera::getProjectionMatrix() const {
	return m_projMatrix;
}

inline const Vector3 &GameCamera::getVelocity() const {
	return m_velocity;
}

inline const Vector3 &GameCamera::getViewDirection() const {
	return m_viewDir;
}

inline const Matrix4 &GameCamera::getViewMatrix() const {
	return m_viewMatrix;
}

inline const Matrix4 &GameCamera::getViewProjectionMatrix() const {
	return m_viewProjMatrix;
}

inline const Vector3 &GameCamera::getXAxis() const {
	return m_xAxis;
}

inline const Vector3 &GameCamera::getYAxis() const {
	return m_yAxis;
}

inline const Vector3 &GameCamera::getZAxis() const {
	return m_zAxis;
}

inline bool GameCamera::preferTargetYAxisOrbiting() const {
	return m_preferTargetYAxisOrbiting;
}

#endif