#include "CubeMapModel.h"

void CubeMapModel::LoadCubeMapTextures()
{
	// Array of paths to cube map images
	const char* cube_filenames[6] =
	{
	"assets/cubemaps/brightday/posx.png",
	"assets/cubemaps/brightday/negx.png",
	"assets/cubemaps/brightday/negy.png",
	"assets/cubemaps/brightday/posy.png",
	"assets/cubemaps/brightday/posz.png",
	"assets/cubemaps/brightday/negz.png",
	};
	Texture cube_texture;


		HRESULT hr = LoadCubeTextureFromFile(m_dxdevice, cube_filenames, &cube_texture);
		std::cout << "\t" << "Cubemap files"
			<< (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;

	const unsigned cube_slot = 2;
	m_dxdevice_context->PSSetShaderResources(cube_slot, 1, &cube_texture.TextureView);
}

CubeMapModel::CubeMapModel(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context, ID3D11Buffer* material_buffer)
	: CubeModel(dxdevice, dxdevice_context, material_buffer)
{
	LoadCubeMapTextures();
};
