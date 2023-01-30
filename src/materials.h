#pragma once
#include "vec/vec.h"
namespace materials
{
	struct Material
	{
	public:
		vec4f Ambient = vec4f(0, 0, 0, 0);
		vec4f Diffuse = vec4f(0, 0, 0, 0);
		vec4f Specular = vec4f(0, 0, 0, 0);
		float Alpha = 0;
	};

	struct Materials
	{
	public:
		Material Red;
		Material Green;
		Material Blue;
		Material Yellow;
		Materials()
		{
			Red = Material();

			Red.Ambient = normalize(vec4f(1, 0, 0, 0));
			Red.Diffuse = normalize(vec4f(1, 1, 1, 0));
			Red.Specular = normalize(vec4f(1, 1, 1, 0));
			Red.Alpha = 50;

			Green = Material();

			Green.Ambient = normalize(vec4f(0, 1, 0, 0));
			Green.Diffuse = normalize(vec4f(0, 255, 0, 0));
			Green.Specular = normalize(vec4f(1, 1, 1, 0));
			Green.Alpha = 20;

			Blue = Material();

			Blue.Ambient = normalize(vec4f(0, 0, 1, 0));
			Blue.Diffuse = normalize(vec4f(1, 1, 1, 0));
			Blue.Specular = normalize(vec4f(1, 1, 1, 0));
			Blue.Alpha = 50;

			Yellow = Material();

			Yellow.Ambient = normalize(vec4f(255, 255, 0, 0));
			Yellow.Diffuse = normalize(vec4f(1, 1, 1, 0));
			Yellow.Specular = normalize(vec4f(1, 1, 1, 0));
			Yellow.Alpha = 80;
		}

	};
}


