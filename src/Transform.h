#pragma once

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

class Transform
{
protected:
	Transform* m_parent = nullptr;

	linalg::vec3f m_position = { 0,0,0 }; //!< position relative to parent
	linalg::vec3f m_world_position = { 0,0,0 }; //!< position in the world

	linalg::vec3f m_rotation = { 0,0,0 }; //!< rotation in the world
	linalg::vec3f m_world_rotation = { 0,0,0 }; //!< rotation in the world
	float m_rotation_angle = 0;
	float m_world_rotation_angle = 0;

	linalg::vec3f m_scale = { 0,0,0 }; //!< scale in the world
	linalg::vec3f m_world_scale = { 0,0,0 }; //!< scale in the world

	linalg::vec3f m_pivot = { 0,0,0 }; //!< scale in the world
	linalg::vec3f m_world_pivot = { 0,0,0 }; //!< scale in the world


	mat4f m_transform;

public:
	Transform();
	Transform(Transform* parent);
	//Position
	linalg::vec3f GetPosition() const;
	linalg::vec3f GetWorldPosition() const;
	void SetPosition(linalg::vec3f position);
	void Move(linalg::vec3f direction);

	//Pivot
	linalg::vec3f GetPivotPosition() const;
	linalg::vec3f GetWorldPivotPosition() const;
	void SetPivotPosition(linalg::vec3f pivot_position);

	//rotation
	linalg::vec3f GetRotation() const;
	linalg::vec3f GetWorldRotation() const;
	float GetRotationAngle() const;
	float GetWorldRotationAngle() const;

	void SetRotation(linalg::vec3f rotation);
	void SetRotation( float rotation_angle, linalg::vec3f rotation);
	void SetRotationAngle(float rotation_angle);

	//Scale
	linalg::vec3f GetScale() const;
	linalg::vec3f GetWorldScale() const;
	void SetScale(linalg::vec3f scale);

	//Transform
	mat4f GetTransform();
	void CalculateTransform();
};