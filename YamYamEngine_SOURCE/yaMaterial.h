#pragma once
#include "yaResource.h"
#include "yaShader.h"
#include "yaRenderer.h"
#include "yaTexture.h"
#include "yaImageFilter.h"


namespace ya::graphics
{
	class Material : public Resource
	{
	public:
		Material();
		~Material();

		virtual HRESULT Load(const std::wstring& path) { return TRUE; };

		void Bind();
		void Clear();
		std::shared_ptr<ImageFilter> CreateImageFilter(const std::wstring& prefix, int width, int height
			, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
		void RenderImageFilters(UINT indexCount);

		eRenderingMode GetRenderingMode() { return mMode; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
		std::shared_ptr<Shader> GetShader() { return mShader; }
		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Texture> GetTexture(eTextureType slot) { return mTexture[(UINT)slot]; }
		void SetTexture(eTextureType slot, std::shared_ptr<Texture> texture) { mTexture[(UINT)slot] = texture; }
		void SetColor(Vector4 diff, Vector4 spec, Vector4 amb) 
		{ 
			mAttribute.diffuse = diff; 
			mAttribute.specular = spec; 
			mAttribute.ambient = amb; 
		}

	private:
		eRenderingMode mMode;
		renderer::MaterialCB mAttribute;
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture[(UINT)eTextureType::End];
		std::vector<std::shared_ptr<ImageFilter>> mImageFilters;
	};
}
