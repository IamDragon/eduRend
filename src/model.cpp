#include "model.h"
#include "materials.h"
#include "buffers.h"

void Model::compute_TB(Vertex& v0, Vertex& v1, Vertex& v2)
{
	vec3f tangent, binormal;
	// TODO: compute the 'tangent' and 'binormal' vectors
	//       using Lengyel’s method, as given in lecture
	// Now assign the newly computed vectors to the vertices

	vec3f D, E;
	D = v1.Position - v0.Position;
	E = v2.Position - v0.Position;
	vec2f F, G;
	F = v1.TexCoord - v0.TexCoord;
	G = v2.TexCoord - v0.TexCoord;

	float multiplier = 1 / (F.x * G.y - F.y * G.x);
	mat2f GF(G.y, -F.y, -G.x, F.x);

	tangent.x = multiplier * (G.y * D.x + -F.y * E.x);
	tangent.y = multiplier * (G.y * D.y + -F.y * E.y);
	tangent.z = multiplier * (G.y * D.z + -F.y * E.z);

	binormal.x = multiplier * (-G.x * D.x + F.x * E.x);
	binormal.y = multiplier * (-G.x * D.y + F.x * E.y);
	binormal.z = multiplier * (-G.x * D.z + F.x * E.z);

	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Binormal = v1.Binormal = v2.Binormal = binormal;
}

void Model::UpdateMaterialBuffer() const
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_material_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialBuffer* materialBuffer = (MaterialBuffer*)resource.pData;
	materialBuffer->Ambient = vec4f(m_material.AmbientColour, 0);
	materialBuffer->Diffuse = vec4f(m_material.DiffuseColour, 0);
	materialBuffer->Specular = vec4f(m_material.SpecularColour, 0);
	materialBuffer->Alpha = m_material.Alpha;
	m_dxdevice_context->Unmap(m_material_buffer, 0);
}

/// <summary>
/// Sets  Ambient, Diffuse, specular and alpha values with no texture
/// </summary>
/// <param name="material"></param>
void Model::SetMaterial(materials::Material material)
{
	m_material.AmbientColour = material.Ambient.xyz();
	m_material.DiffuseColour = material.Diffuse.xyz();
	m_material.SpecularColour = material.Specular.xyz();
	m_material.Alpha = material.Alpha;
}


/// <summary>
/// Assumes materials contains texture files
/// </summary>
/// <param name="material"></param>
void Model::SetMaterial(Material* material)
{
	m_material = *material;

}

/// <summary>
/// Sets  Ambient, Diffuse, specular and alpha values with no texture
/// </summary>
/// <param name="ambient"></param>
/// <param name="diffuse"></param>
/// <param name="specular"></param>
/// <param name="alpha"></param>
void Model::SetMaterial(vec4f ambient, vec4f diffuse, vec4f specular, float alpha)
{
	m_material.AmbientColour = ambient.xyz();
	m_material.DiffuseColour = diffuse.xyz();
	m_material.SpecularColour = specular.xyz();
	m_material.Alpha = alpha;
}
