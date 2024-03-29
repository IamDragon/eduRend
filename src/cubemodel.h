/**
 * @file quadmodel.h
 * @brief Contains the CubeModel
 * @author 
*/

#pragma once
#include "model.h"
//#include "Model.h"

/**
* @brief Model reprsenting a Cube.
*/
class CubeModel : public Model
{


	unsigned m_number_of_indices = 0;

	void CreateCube(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);

public:
	/**
	 * @brief Create a model of a cube.
	 * @param dxdevice Valid ID3D11Device.
	 * @param dxdevice_context Valid ID3D11DeviceContext.
	*/
	CubeModel(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, ID3D11Buffer* material_buffer);

	/**
	 * @brief Render the model.
	*/
	virtual void Render() const;

	/**
	 *@brief Destructor.
	*/
	~CubeModel() { }
};

