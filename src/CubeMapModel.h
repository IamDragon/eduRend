#pragma once
#include "cubemodel.h"
//#include "src/shader.h"
//#include <d3d11.h>

 class CubeMapModel : public CubeModel
{
private:

	void LoadCubeMapTextures();
public:
	CubeMapModel(
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context, ID3D11Buffer* material_buffer);
};

