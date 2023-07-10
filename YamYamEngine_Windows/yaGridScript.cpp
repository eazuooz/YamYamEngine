#include "yaGridScript.h"
#include "yaMeshRenderer.h"
#include "yaMaterial.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaApplication.h"

extern ya::Application application;


namespace ya
{
	GridScript::GridScript()
		: Script()
		, mCamera(nullptr)
	{
	}
	GridScript::~GridScript()
	{
	}
	void GridScript::Initialize()
	{
	}

	void GridScript::Update()
 	{
		if (mCamera == nullptr)
			return;

		GameObject* gameObj = mCamera->GetOwner();

		Transform* tr = gameObj->GetComponent<Transform>();
		Vector4 cameraWorldPos(tr->GetPosition());
		cameraWorldPos.w = 1.0f;

		float scale = mCamera->GetOrthographicScale();

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = (winRect.right - winRect.left);
		float height = (winRect.bottom - winRect.top);
		Vector2 resolution(width, height);

		// Constant Buffer
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Grid];
		renderer::GridCB data;
		data.cameraPosition = cameraWorldPos;
		data.cameraScale.x = scale;
		data.cameraScale.y = scale;
		data.resolution = resolution;

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}

