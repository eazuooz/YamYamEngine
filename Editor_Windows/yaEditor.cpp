#include "yaEditor.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaGridScript.h"
#include "yaRenderer.h"
#include "BaseRenderer.h"

namespace ya
{
	std::vector<gui::Widget*> Editor::mWidgets;
	std::vector<EditorObject*> Editor::mEditorObjects;
	std::vector<DebugObject*> Editor::mDebugObjects;

	void Editor::Initialize()
	{
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"DebugRect");
		std::shared_ptr<Material> mateiral = Resources::Find<Material>(L"DebugMaterial");
		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
		mDebugObjects[(UINT)eColliderType::Rect]->GetComponent<MeshRenderer>()->SetMaterial(mateiral);
		mDebugObjects[(UINT)eColliderType::Rect]->GetComponent<MeshRenderer>()->SetMesh(mesh);


		std::shared_ptr<Mesh> circleMesh = Resources::Find<Mesh>(L"DebugCircle");
		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>();
		mDebugObjects[(UINT)eColliderType::Circle]->GetComponent<MeshRenderer>()->SetMaterial(mateiral);
		mDebugObjects[(UINT)eColliderType::Circle]->GetComponent<MeshRenderer>()->SetMesh(circleMesh);


		//Grid 
		EditorObject* gridObj = new EditorObject();
		gridObj->SetName(L"Grid");
		gridObj->AddComponent<MeshRenderer>();

		GridScript* gridScript = gridObj->AddComponent<GridScript>();
		gridScript->SetCamera(renderer::mainCamera);

		MeshRenderer* girdMeshRenderer = gridObj->GetComponent<MeshRenderer>();
		girdMeshRenderer->SetMesh(Resources::Find<Mesh>(L"GridMesh"));
		girdMeshRenderer->SetMaterial(Resources::Find<Material>(L"GridMaterial"));
		//gridObj->GetComponent<Transform>()->SetPosition(0, 0, 20);

		mEditorObjects.push_back(gridObj);
	}
	void Editor::Run()
	{
		Update();
		FixedUpdate();
		Render();
	}
	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}
	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}
	}
	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}
		
		for (ya::graphics::DebugMesh& mesh : renderer::debugMeshes)
		{
			if (mesh.duration > 0.0f)
				continue;

			//mesh.position = Vector3(0.0f, 0.0f, 20.0f);
			DebugRender(mesh);
		}
		renderer::debugMeshes.clear();
	}

	void Editor::Release()
	{
		for (auto widget : mWidgets)
		{
			delete widget;
			widget = nullptr;
		}
		for (auto editorObject : mEditorObjects)
		{
			delete editorObject;
			editorObject = nullptr;
		}
		for (auto debugObject : mDebugObjects)
		{
			delete debugObject;
			debugObject = nullptr;
		}
	}

	void Editor::DebugRender(DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		Transform* tr = debugObj->GetComponent<Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotation);

		if (mesh.type == eColliderType::Rect)
		{
			tr->SetScale(mesh.scale);
		}
		else
		{
			tr->SetScale(Vector3(mesh.radius, mesh.radius, mesh.radius));
		}
		
		BaseRenderer* baseRenderer = debugObj->GetComponent<BaseRenderer>();
		Camera* mainCamera = renderer::mainCamera;

		tr->FixedUpdate();

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetGpuViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetGpuProjectionMatrix());

		debugObj->Render();
	}
}

