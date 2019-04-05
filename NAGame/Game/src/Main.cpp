#include <cstdlib>

#include <Windows.h>

#include "Core/Engine/Engine.h"

#include "Renderer/Scene/Scene.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/InputLayout.h"
#include "Renderer/Material.h"
#include "Renderer/Shader.h"
#include "Renderer/Scene/Renderables/MeshInstance.h"

int main(int argc, char *argv[])
{
	// Initialize
	if (na::InitializeEngine() == false) {
		return EXIT_FAILURE;
	}

	na::Scene *scene = na::Scene::GetInstance();

	na::VertexBuffer vb;
	float verts[] = { 0.0f, 0.5f, 0.0f,     0.5f, 0.0f, 0.0f,      -0.5f, 0.0f, 0.0f, };
	vb.Initialize(verts, 3, sizeof(float) * 3, na::BufferUsage::DEFAULT);
	
	na::IndexBuffer ib;
	na::IndexType indices[] = { 0, 1, 2 };
	ib.Initialize(indices, 3, na::BufferUsage::DEFAULT);

	na::VertexShader vs;
	vs.Initialize("data\\red_vs.hlsl");

	na::PixelShader ps;
	ps.Initialize("data\\red_ps.hlsl");

	na::InputLayout il;
	std::vector<na::InputElement> ilElems;
	na::InputElement ie;
	ie.mSemantic = "POSITION";
	ie.mIndex = 0;
	ie.mFormat = na::Format::R32G32B32_FLOAT;
	ilElems.push_back(ie);
	il.Initialize(ilElems, vs);

	na::Shader shader;
	shader.Initialize(&il, &vs, &ps);

	na::Material mat;
	mat.Initialize(&shader);

	na::MeshInstance mesh;
	mesh.Initialize(&vb, &ib, &mat);

	scene->AddRenderable(&mesh);

	// Run
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (true) {
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			break;
		}

		na::DoFrame();
	}

	vb.Shutdown();
	ib.Shutdown();
	vs.Shutdown();
	ps.Shutdown();
	il.Shutdown();
	shader.Shutdown();
	mat.Shutdown();
	mesh.Shutdown();


	// Shutdown
	na::ShutdownEngine();

	// Success!
	return 0;
}