#pragma once
#include "yaResource.h"
#include "yaShader.h"
#include "yaRenderer.h"


namespace ya::graphics
{
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual HRESULT Load(const std::wstring& path) { return TRUE; };

		void SetData(eGPUParam param, void* data);
		void Bind();

		void SetShader(Shader* shader) { mShader = shader; }
		Shader* GetShader() { return mShader; }

	private:
		renderer::MaterialCB mCB;
		Shader* mShader;
	};
}
