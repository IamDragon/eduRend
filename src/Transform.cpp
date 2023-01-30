#include "Transform.h"
#include "stdafx.h"
#include "vec\vec.h"
#include "vec\mat.h"
#include <iostream>

using namespace linalg;

Transform::Transform() {}

Transform::Transform(Transform* parent)
{
	m_parent = parent;
}

linalg::vec3f Transform::GetPosition() const
{
	return m_position;
}

linalg::vec3f Transform::GetWorldPosition() const
{
	return m_world_position;
}

void Transform::SetPosition(linalg::vec3f position) {
	if (m_parent != nullptr) {
		m_position = position;
		m_world_position = position + m_parent->GetPosition();
	}
	else
	{
		m_position = position;
		m_world_position = position;
	}
}

void Transform::Move(linalg::vec3f direction) 
{
	m_position += direction;
	m_world_position += direction;
}

linalg::vec3f Transform::GetPivotPosition() const
{
	return m_pivot;
}

linalg::vec3f Transform::GetWorldPivotPosition() const
{
	return m_world_pivot;
}

void Transform::SetPivotPosition(linalg::vec3f pivot_position) {
	if (m_parent != nullptr) {
		m_pivot = pivot_position;
		m_world_pivot = pivot_position + m_parent->GetPivotPosition();
	}
	else
	{
		m_pivot = pivot_position;
		m_world_pivot = pivot_position;
	}
}

linalg::vec3f Transform::GetRotation() const
{
	return m_rotation;
}

linalg::vec3f Transform::GetWorldRotation() const
{
	return m_world_rotation;
}

float Transform::GetRotationAngle() const
{
	return m_rotation_angle;
}

float Transform::GetWorldRotationAngle() const
{
	return m_world_rotation_angle;
}

void Transform::SetRotation(linalg::vec3f rotation) {
	if (m_parent != nullptr) {
		m_rotation = rotation;
		m_world_rotation = rotation + m_parent->GetPosition();
	}
	else
	{
		m_rotation = rotation;
		m_world_rotation = rotation;
	}
}
void Transform::SetRotation( float rotation_angle, linalg::vec3f rotation) {
	if (m_parent != nullptr) {
		m_rotation = rotation;
		m_world_rotation = rotation + m_parent->GetRotation();
		m_rotation_angle = rotation_angle;
		m_world_rotation_angle = rotation_angle + m_parent->GetRotationAngle();
	}
	else
	{
		m_rotation = rotation;
		m_world_rotation = rotation;
		m_rotation_angle = rotation_angle;
		m_world_rotation_angle = rotation_angle;
	}
}
void Transform::SetRotationAngle(float rotation_angle) {
	if (m_parent != nullptr) {
		m_rotation_angle = rotation_angle;
		m_world_rotation_angle = rotation_angle + m_parent->GetRotationAngle();
	}
	else
	{
		m_rotation_angle = rotation_angle;
		m_world_rotation_angle = rotation_angle;
	}
}

linalg::vec3f Transform::GetScale() const
{
	return m_scale;
}

linalg::vec3f Transform::GetWorldScale() const
{
	return m_world_scale;
}

void Transform::SetScale(linalg::vec3f scale) {
	if (m_parent != nullptr) {
		m_scale = scale;
		m_world_scale = scale + m_parent->GetScale();
	}
	else
	{
		m_scale = scale;
		m_world_scale = scale;
	}
}

mat4f Transform::GetTransform()
{
	CalculateTransform();
	return m_transform;
}

void Transform::CalculateTransform()
{
	//rotation around pivot
	mat4f pivot_rotation_mat = mat4f::translation(m_pivot) *
		mat4f::rotation(m_rotation_angle, m_rotation) * mat4f::translation(-m_pivot);


	mat4f local_transform = mat4f::translation(m_position) *
		pivot_rotation_mat *
		mat4f::scaling(m_scale);

	if (m_parent != nullptr)
		m_transform = m_parent->GetTransform() * local_transform;
	else
		m_transform = local_transform;
}