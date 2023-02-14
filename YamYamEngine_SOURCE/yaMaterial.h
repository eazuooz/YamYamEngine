#pragma once
#include "yaResource.h"
#include "yaShader.h"
#include "yaRenderer.h"
#include "yaTexture.h"

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
		void Clear();

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }

		void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
		eRenderingMode GetRenderingMode() { return mMode; }

	private:
		renderer::MaterialCB mCB;
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture;
		eRenderingMode mMode;
	};
}
