#include "Camera.h"
#include <iostream>
#include "vec\vec.h"
#include "vec\mat.h"
#include "stdafx.h"

using namespace linalg;

void Camera::MoveTo(const vec3f& position) noexcept
{
	m_position = position;
}

void Camera::Move(const vec3f& direction) noexcept
{
	m_position += direction;
}

mat4f Camera::WorldToViewMatrix() const noexcept
{
	// Assuming a camera's position and rotation is defined by matrices T(p) and R,
	// the View-to-World transform is T(p)*R (for a first-person style camera).
	//
	// World-to-View then is the inverse of T(p)*R;
	//		inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
	// Since now there is no rotation, this matrix is simply T(-p)

	//mat4f rotationMatrix = mat4f::rotation(0, m_rotation);
	//mat4f R = mat4f::rotation(0, yaw, pitch);
	//mat4f rot = linalg::transpose(mat4f::rotation(1, m_rotation));
	//rotationMatrix.transpose(); //inverts camera
	// 
	//return mat4f::translation(-m_position) * rotationMatrix;
	//return rot * mat4f::translation(-m_position);
	//return rotationMatrix * mat4f::translation(-m_position);
	mat4f T, R, M;

	//R = linalg::transpose(mat4f::rotation(m_rotation.x, m_rotation.y, m_rotation.z));
	R = mat4f::rotation(m_rotation.x, m_rotation.y, m_rotation.z);
	R.transpose();
	T = mat4f::translation(-m_position);
	M = R * T;
	return M;

	//return mat4f::translation(-m_position);
}

linalg::mat4f Camera::ViewToWorldMatrix() const noexcept
{
	mat4f T, R, M;

	//R = linalg::transpose(mat4f::rotation(m_rotation.x, m_rotation.y, m_rotation.z));
	R = mat4f::rotation(m_rotation.x, m_rotation.y, m_rotation.z);
	T = mat4f::translation(m_position);
	M = T * R;
	return M;
}


mat4f Camera::ProjectionMatrix() const noexcept
{
	return mat4f::projection(m_vertical_fov, m_aspect_ratio, m_near_plane, m_far_plane);
}

void Camera::Rotate(const linalg::vec3f& rotation) noexcept
{
	m_rotation -= rotation;
	m_rotation.z = clamp(m_rotation.z, -PI / 2, PI / 2);
}

mat4f Camera::RotationMatrix(const float& roll, const float& yaw, const float& pitch) const noexcept
{
	//mat4f::rotation()
	return mat4f::rotation(roll, yaw, pitch);
}

linalg::vec3f Camera::GetPosition() const noexcept
{
	return m_position;
}

linalg::vec4f Camera::GetWorldSpacePosition() const noexcept
{
	return ViewToWorldMatrix() * vec4f(m_position,0);
}