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

		std::shared_ptr<Shader> GetShader() { return mShader; }
		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }

		eRenderingMode GetRenderingMode() { return mMode; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
		std::shared_ptr<Texture> GetTexture(eTextureSlot slot) { return mTexture[(UINT)slot]; }
		void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> texture) { mTexture[(UINT)slot] = texture; }

	private:
		renderer::MaterialCB mCB;
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture[(UINT)eTextureSlot::End];
		eRenderingMode mMode;
	};
}
