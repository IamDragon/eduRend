
#include "Scene.h"
#include "QuadModel.h"
#include "OBJModel.h"
#include "CubeModel.h"
#include "Transform.h"
#include "CubeMapModel.h"

Scene::Scene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	m_dxdevice(dxdevice),
	m_dxdevice_context(dxdevice_context),
	m_window_width(window_width),
	m_window_height(window_height)
{
}

void Scene::OnWindowResized(
	int new_width,
	int new_height)
{
	m_window_width = new_width;
	m_window_height = new_height;
}

OurTestScene::OurTestScene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	Scene(dxdevice, dxdevice_context, window_width, window_height)
{
	InitTransformationBuffer();
	InitLightCameraBuffer();
	InitMaterialBuffer();
	// + init other CBuffers
	InitSampler();
	InitCubeMapSampler();
}

//
// Called once at initialization
//
void OurTestScene::Init()
{
	m_dxdevice_context->PSSetSamplers(0, 1, &m_sampler);
	m_dxdevice_context->PSSetSamplers(1, 1, &m_cubemap_sampler);

	m_camera = new Camera(
		45.0f * fTO_RAD,		// field-of-view (radians)
		(float)m_window_width / m_window_height,	// aspect ratio
		1.0f,					// z-near plane (everything closer will be clipped/removed)
		500.0f);				// z-far plane (everything further will be clipped/removed)

	m_materials = materials::Materials();

	// Move camera to (0,0,5)
	m_camera->MoveTo({ 0, 0, -40 });

	// Create objects
	m_quad = new QuadModel(m_dxdevice, m_dxdevice_context, m_material_buffer);
	//m_cube_parent->SetMaterial(m_materials.Red);

	m_sponza = new OBJModel("assets/crytek-sponza/sponza.obj", m_dxdevice, m_dxdevice_context, m_material_buffer);

	m_cube_parent = new CubeModel(m_dxdevice, m_dxdevice_context, m_material_buffer);
	//m_cube_parent->SetMaterial(m_materials.Red);

	m_cube_child = new CubeModel(m_dxdevice, m_dxdevice_context, m_material_buffer);
	//m_cube_parent->SetMaterial(m_materials.Green);

	m_cube_child_child = new CubeModel(m_dxdevice, m_dxdevice_context, m_material_buffer);
	//m_cube_parent->SetMaterial(m_materials.Yellow);

	m_sphere = new OBJModel("assets/sphere/sphere.obj", m_dxdevice, m_dxdevice_context, m_material_buffer);

	m_cubemap = new CubeMapModel(m_dxdevice, m_dxdevice_context, m_material_buffer);

	m_sponza_transform = new Transform();
	m_quad_transform = new Transform();
	m_cube_parent_transform = new Transform();
	m_cube_child_transform = new Transform(m_cube_parent_transform);
	m_cube_child_child_transform = new Transform(m_cube_child_transform);
	m_light_transform = new Transform();
	m_cubemap_transform = new Transform();


	m_sponza_transform->SetPosition(vec3f(0, -5, 0));					// Move down 5 units
	m_sponza_transform->SetPivotPosition(vec3f(0, -5, 0));
	m_sponza_transform->SetRotation(fPI / 2, vec3f(0, 1, 0));					// Rotate pi/2 radians (90 degrees) around y
	m_sponza_transform->SetScale(vec3f(0.05f, 0.05f, 0.05f));

	m_quad_transform->SetPosition(vec3f(0, -1, 0));
	m_quad_transform->SetRotation(fPI / 2, vec3f(-1, 0, 0));
	m_quad_transform->SetScale(vec3f(15, 50, 1));

	m_light_transform->SetPosition(vec3f(0, 1, -45));
	m_light_transform->SetScale(vec3f(0.2f, 0.2f, 0.2f));

	m_cubemap_transform->SetPosition(vec3f(0, 0, 0));
	m_cubemap_transform->SetScale(vec3f(-300, -300, -300));
	//m_cubemap_transform->SetScale(vec3f(20, 20, 20));

}

//
// Called every frame
// dt (seconds) is time elapsed since the previous frame
//
void OurTestScene::Update(
	float dt,
	const InputHandler& input_handler)
{
	vec4f camera_foward_vec = m_camera->ViewToWorldMatrix() * m_camera->forward;
	vec4f camera_right_vec = m_camera->ViewToWorldMatrix() * m_camera->right;
	vec3f camera_foward = { camera_foward_vec.x,camera_foward_vec.y,camera_foward_vec.z };
	vec3f camera_right = { camera_right_vec.x,camera_right_vec.y,camera_right_vec.z };


	// Basic camera control
	if (/*input_handler.IsKeyPressed(Keys::Up) ||*/ input_handler.IsKeyPressed(Keys::W))
		m_camera->Move(camera_foward * m_camera_velocity * dt);
	if (/*input_handler.IsKeyPressed(Keys::Down) || */input_handler.IsKeyPressed(Keys::S))
		m_camera->Move(-camera_foward * m_camera_velocity * dt);
	if (/*input_handler.IsKeyPressed(Keys::Right) ||*/ input_handler.IsKeyPressed(Keys::D))
		m_camera->Move(camera_right * m_camera_velocity * dt);
	if (/*input_handler.IsKeyPressed(Keys::Left) ||*/ input_handler.IsKeyPressed(Keys::A))
		m_camera->Move(-camera_right * m_camera_velocity * dt);

	long mousedx = input_handler.GetMouseDeltaX();
	long mousedy = input_handler.GetMouseDeltaY();

	//if (input_handler.IsKeyPressed(Keys::Space))
	{
		m_camera->Rotate({ 0, mousedx * dt , mousedy * dt });

		//m_camera->Rotate({ 0, input_handler.GetMouseDeltaX() * dt , input_handler.GetMouseDeltaY() * dt });
	}

	vec3f up = vec3f(0.0f, 1.0f, 0.0f);
	vec3f right = vec3f(1.0f, 0.0f, 0.0f);
	vec3f forward = vec3f(0.0f, 0.0f, 1.0f);

	if (input_handler.IsKeyPressed(Keys::Up) && input_handler.IsKeyPressed(Keys::LShift))
		m_light_transform->Move(-forward * dt);
	else if (input_handler.IsKeyPressed(Keys::Up))
		m_light_transform->Move(up * dt);

	if (input_handler.IsKeyPressed(Keys::Down) && input_handler.IsKeyPressed(Keys::LShift))
		m_light_transform->Move(forward * dt);
	else if (input_handler.IsKeyPressed(Keys::Down))
		m_light_transform->Move(-up * dt);

	if (input_handler.IsKeyPressed(Keys::Right))
		m_light_transform->Move(right * dt);
	if (input_handler.IsKeyPressed(Keys::Left))
		m_light_transform->Move(-right * dt);



	// Now set/update object transformations
	// This can be done using any sequence of transformation matrices,
	// but the T*R*S order is most common; i.e. scale, then rotate, and then translate.
	// If no transformation is desired, an identity matrix can be obtained 
	// via e.g. Mquad = linalg::mat4f_identity; 

	m_cube_parent_transform->SetPosition({ 0,0,0 });
	m_cube_parent_transform->SetPivotPosition({ 0,0,0 });
	m_cube_parent_transform->SetRotation(0, { 0,0,0 });
	m_cube_parent_transform->SetScale({ 1,1,1 });

	m_cube_child_transform->SetPosition({ 2,0,0 });
	m_cube_child_transform->SetPivotPosition({ -2,0,0 });
	m_cube_child_transform->SetRotation(-m_angle, { 0,1,0 });
	m_cube_child_transform->SetScale({ 0.75f,0.75f,0.75f });

	m_cube_child_child_transform->SetPosition({ 1,1,0 });
	m_cube_child_child_transform->SetPivotPosition({ -1,-1,0 });
	m_cube_child_child_transform->SetRotation(-m_angle, { 0,1,0 });
	m_cube_child_child_transform->SetScale({ 0.5f,0.5f,0.5f });

	// Increment the rotation angle.
	m_angle += m_angular_velocity * dt;

	// Print fps
	m_fps_cooldown -= dt;
	if (m_fps_cooldown < 0.0)
	{
		std::cout << "fps " << (int)(1.0f / dt) << std::endl;
		//		printf("fps %i\n", (int)(1.0f / dt));
		m_fps_cooldown = 2.0;
	}
}

//
// Called every frame, after update
//
void OurTestScene::Render()
{
	// Bind transformation_buffer to slot b0 of the VS
	m_dxdevice_context->VSSetConstantBuffers(0, 1, &m_transformation_buffer);
	m_dxdevice_context->PSSetConstantBuffers(0, 1, &m_lightCam_buffer);
	m_dxdevice_context->PSSetConstantBuffers(1, 1, &m_material_buffer);

	//vec4f camPos = vec4f(m_camera->GetPosition(), 0);
	vec4f camPos = m_camera->GetWorldSpacePosition();
	vec4f lightPos = vec4f(m_light_transform->GetPosition(), 0);
	//vec4f lightPos = m_light_transform->GetTransform() * vec4f(m_light_transform->GetPosition(),0);
	UpdateLightCameraBuffer(lightPos, camPos);

	// Obtain the matrices needed for rendering from the camera
	m_view_matrix = m_camera->WorldToViewMatrix();
	m_projection_matrix = m_camera->ProjectionMatrix();

	//Load matrices + the Quad's transformation to the device and render it
	UpdateTransformationBuffer(m_quad_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	//m_quad->Render();

	//Load matrices + Sponza's transformation to the device and render it
	UpdateTransformationBuffer(m_sponza_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	m_sponza->Render();

	// Load matrices + Cube's transformation to the device and render it
	UpdateTransformationBuffer(m_cube_parent_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	//m_cube_parent->Render();

	// Load matrices + Cube's transformation to the device and render it
	UpdateTransformationBuffer(m_cube_child_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	//m_cube_child->Render();

	//Load matrices + Cube's transformation to the device and render it
	UpdateTransformationBuffer(m_cube_child_child_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	//m_cube_child_child->Render();

	UpdateTransformationBuffer(m_light_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	m_sphere->Render();

	UpdateTransformationBuffer(m_cubemap_transform->GetTransform(), m_view_matrix, m_projection_matrix);
	m_cubemap->Render();

}

void OurTestScene::Release()
{
	SAFE_DELETE(m_quad);
	SAFE_DELETE(m_sponza);
	SAFE_DELETE(m_cube_parent);
	SAFE_DELETE(m_cube_child);
	SAFE_DELETE(m_cube_child_child);
	SAFE_DELETE(m_sphere);
	SAFE_DELETE(m_camera);

	SAFE_DELETE(m_sponza_transform);
	SAFE_DELETE(m_cube_parent_transform);
	SAFE_DELETE(m_cube_child_transform);
	SAFE_DELETE(m_cube_child_child_transform);
	SAFE_DELETE(m_light_transform);

	SAFE_RELEASE(m_transformation_buffer);
	SAFE_RELEASE(m_lightCam_buffer);
	SAFE_RELEASE(m_material_buffer);
	// + release other CBuffers
	SAFE_RELEASE(m_sampler);
}

void OurTestScene::OnWindowResized(
	int new_width,
	int new_height)
{
	if (m_camera)
		m_camera->SetAspect(float(new_width) / new_height);

	Scene::OnWindowResized(new_width, new_height);
}

void OurTestScene::InitTransformationBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC matrixBufferDesc = { 0 };
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(TransformationBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&matrixBufferDesc, nullptr, &m_transformation_buffer));
}

void OurTestScene::UpdateTransformationBuffer(
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_transformation_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TransformationBuffer* matrixBuffer = (TransformationBuffer*)resource.pData;
	matrixBuffer->ModelToWorldMatrix = ModelToWorldMatrix;
	matrixBuffer->WorldToViewMatrix = WorldToViewMatrix;
	matrixBuffer->ProjectionMatrix = ProjectionMatrix;
	m_dxdevice_context->Unmap(m_transformation_buffer, 0);
}

void OurTestScene::InitLightCameraBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC lightCamBufferDesc = { 0 };
	lightCamBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightCamBufferDesc.ByteWidth = sizeof(LightCamBuffer);
	lightCamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightCamBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightCamBufferDesc.MiscFlags = 0;
	lightCamBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&lightCamBufferDesc, nullptr, &m_lightCam_buffer));
}

void OurTestScene::UpdateLightCameraBuffer(vec4f light_position, vec4f camera_position)

{
	// Map the resource buffer, obtain a pointer and then write our positions to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_lightCam_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	LightCamBuffer* lightCamBuffer = (LightCamBuffer*)resource.pData;
	lightCamBuffer->LightPos = light_position;
	lightCamBuffer->CamPos = camera_position;
	m_dxdevice_context->Unmap(m_lightCam_buffer, 0);
}

void OurTestScene::InitMaterialBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC materialBufferDesc = { 0 };
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialBuffer);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&materialBufferDesc, nullptr, &m_material_buffer));
}

void OurTestScene::InitSampler()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc =
	{
		//D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		//D3D11_TEXTURE_ADDRESS_MIRROR,
		//D3D11_TEXTURE_ADDRESS_MIRROR,
		//D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		//D3D11_TEXTURE_ADDRESS_CLAMP,
		//D3D11_TEXTURE_ADDRESS_CLAMP,
		//D3D11_TEXTURE_ADDRESS_CLAMP,
		0.0f,
		1,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f},
		-FLT_MAX,
		FLT_MAX
	};
	ASSERT(hr = m_dxdevice->CreateSamplerState(&samplerDesc, &m_sampler));
}

void OurTestScene::InitCubeMapSampler()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc =
	{
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		1,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f},
		-FLT_MAX,
		FLT_MAX
	};
	ASSERT(hr = m_dxdevice->CreateSamplerState(&samplerDesc, &m_cubemap_sampler));
}
