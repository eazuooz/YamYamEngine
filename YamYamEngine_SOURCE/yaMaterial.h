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

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }

	private:
		renderer::MaterialCB mCB;
		std::shared_ptr<Shader> mShader;
	};
}
