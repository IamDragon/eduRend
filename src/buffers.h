/**
 * @file buffers.h
 * @brief Contains constant buffers
*/

#pragma once

#include "vec/mat.h"

/**
 * @brief Contains transformation matrices.
*/
struct TransformationBuffer
{
	linalg::mat4f ModelToWorldMatrix; //!< Matrix for converting from object space to world space.
	linalg::mat4f WorldToViewMatrix; //!< Matrix for converting from world space to view space.
	linalg::mat4f ProjectionMatrix; //!< Matrix for converting from view space to clip cpace.
};

struct LightCamBuffer 
{
	vec4f LightPos;
	vec4f CamPos;
};

struct MaterialBuffer 
{
	vec4f Ambient;
	vec4f Diffuse;
	vec4f Specular;
	float Alpha;
	vec3f pad;
};