#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"
#include "yaResources.h"
#include "yaRenderer.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"
#include "yaTexture.h"

extern ya::Application application;


namespace ya::graphics
{
	GraphicsDevice_DX11::GraphicsDevice_DX11()
	{
		HWND hWnd = application.GetHwnd();
		ya::graphics::GetDevice() = this;

		const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		const D3D_FEATURE_LEVEL featureLevels[2] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_9_3 };
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDevice(nullptr      // Specify nullptr to use the default adapter.
			, driverType                // Create a device using the hardware graphics driver.
			, 0                         // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
			, createDeviceFlags         // Set debug and Direct2D compatibility flags.
			, featureLevels             // List of feature levels this app can support.
			, ARRAYSIZE(featureLevels)  // Size of the list above.
			, D3D11_SDK_VERSION         // Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
			, mDevice.GetAddressOf()    // Returns the Direct3D device created.
			, &featureLevel             // Returns feature level of device created.
			, mContext.GetAddressOf()); // Returns the device immediate context.

		// 4X MSAA surported check
		UINT quility = 0;
		mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
		Texture::SetQuality(quility);

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = application.GetHwnd();	                // Front Buffer 를 출력시킬 윈도우 핸들
		swapChainDesc.Windowed = true;		                            // 윈도우, 전체화면 모드
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		if (Texture::GetQuality() > 0)
		{
			swapChainDesc.SampleDesc.Count = 4; // how many multisamples
			swapChainDesc.SampleDesc.Quality = quility - 1;

		}
		else
		{
			swapChainDesc.SampleDesc.Count = 1; // how many multisamples
			swapChainDesc.SampleDesc.Quality = 0;
		}

		if (!CreateSwapChain(swapChainDesc))
			return;

		mRenderTarget = std::make_shared<Texture>();

		// Get render target by Swapchain
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(renderTarget.GetAddressOf()));
		D3D11_TEXTURE2D_DESC desc;
		renderTarget->GetDesc(&desc);

		//mRenderTarget->SetTexture(renderTarget);
		mRenderTarget->Create(renderTarget);

		renderer::renderTarget = mRenderTarget;
		//Resources::Insert<Texture>(L"RenderTargetTexture", mRenderTarget);

		mDepthStencil = std::make_shared<Texture>();
		mDepthStencil->Create(1600, 900, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	}

	GraphicsDevice_DX11::~GraphicsDevice_DX11()
	{
		//ID3D11Debug* debugDev;
		//mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
	}

	bool GraphicsDevice_DX11::CreateSwapChain(DXGI_SWAP_CHAIN_DESC desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice>     pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter>    pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory>    pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &desc, mSwapChain.GetAddressOf())))
			return false;

		//        const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		//        UINT createDeviceFlags = 0;
		//#if defined(DEBUG) || defined(_DEBUG)
		//        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		//#endif
		//        const D3D_FEATURE_LEVEL featureLevels[2] = {
		//    D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
		//    D3D_FEATURE_LEVEL_9_3 };
		//        D3D_FEATURE_LEVEL featureLevel;
		//
		//        if (FAILED(D3D11CreateDeviceAndSwapChain(0, // Default adapter
		//            driverType,
		//            0, // No software device
		//            createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION,
		//            &desc, mSwapChain.GetAddressOf(), mDevice.GetAddressOf(), &featureLevel,
		//            mContext.GetAddressOf()))) 
		//        {
		//            return false;
		//        }

		return true;
	}

	bool GraphicsDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* initial_data, ID3D11Buffer** buffer)
	{
		if (FAILED(mDevice->CreateBuffer(desc, initial_data, buffer)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateTexture(const D3D11_TEXTURE2D_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
	{
		// Create Depth Stencil Buffer
		if (FAILED(mDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(pBlendStateDesc, ppBlendState)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
		, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(pInputElementDescs, NumElements
			, pShaderBytecodeWithInputSignature
			, BytecodeLength
			, ppInputLayout)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CompileFromFile(const std::wstring& fileName
		, const std::string& funcName
		, const std::string& version
		, ID3DBlob** ppCode)
	{
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\Shaders_SOURCE\\";

		std::wstring filePath(path.c_str());
		filePath += fileName;

		ID3DBlob* errorBlob = nullptr;
		if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, funcName.c_str(), version.c_str()
			, 0, 0, ppCode, &errorBlob)))
		{
			if (errorBlob != nullptr)
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());

			return false;
		}




		return true;
	}

	bool GraphicsDevice_DX11::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode
			, BytecodeLength
			, nullptr
			, ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11PixelShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode
			, BytecodeLength
			, nullptr
			, ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11GeometryShader** ppGeometryShader)
	{
		if (FAILED(mDevice->CreateGeometryShader
		(pShaderBytecode
			, BytecodeLength
			, nullptr
			, ppGeometryShader)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ComputeShader** ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(pShaderBytecode
			, BytecodeLength
			, nullptr
			, ppComputeShader)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
	{

		if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
			return false;

		return true;
	}

	bool GraphicsDevice_DX11::CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(pResource, pDesc, ppUAView)))
			return false;

		return true;
	}

	void GraphicsDevice_DX11::BindInputLayout(ID3D11InputLayout* pInputLayout)
	{
		mContext->IASetInputLayout(pInputLayout);
	}

	void GraphicsDevice_DX11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
	{
		mContext->IASetPrimitiveTopology(Topology);
	}

	void GraphicsDevice_DX11::BindSamplers(eShaderStage stage, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		if ((UINT)eShaderStage::VS & (UINT)stage)
			mContext->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);

		if ((UINT)eShaderStage::HS & (UINT)stage)
			mContext->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);

		if ((UINT)eShaderStage::DS & (UINT)stage)
			mContext->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);

		if ((UINT)eShaderStage::GS & (UINT)stage)
			mContext->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);

		if ((UINT)eShaderStage::PS & (UINT)stage)
			mContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
	}

	void GraphicsDevice_DX11::BindsSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		BindSamplers(eShaderStage::VS, StartSlot, NumSamplers, ppSamplers);
		BindSamplers(eShaderStage::HS, StartSlot, NumSamplers, ppSamplers);
		BindSamplers(eShaderStage::DS, StartSlot, NumSamplers, ppSamplers);
		BindSamplers(eShaderStage::GS, StartSlot, NumSamplers, ppSamplers);
		BindSamplers(eShaderStage::PS, StartSlot, NumSamplers, ppSamplers);
	}

	void GraphicsDevice_DX11::BindVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT stride, const UINT offset)
	{
		mContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, &stride, &offset);
	}

	void GraphicsDevice_DX11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT Offset)
	{
		mContext->IASetIndexBuffer(pIndexBuffer, format, Offset);
	}

	void GraphicsDevice_DX11::CopyResource(ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource)
	{
		mContext->CopyResource(pDstResource, pSrcResource);
	}

	void GraphicsDevice_DX11::BindVertexShader(ID3D11VertexShader* pVertexShader)
	{
		mContext->VSSetShader(pVertexShader, 0, 0);
	}

	void GraphicsDevice_DX11::BindHullShader(ID3D11HullShader* pHullShader)
	{
		mContext->HSSetShader(pHullShader, 0, 0);
	}

	void GraphicsDevice_DX11::BindDomainShader(ID3D11DomainShader* pDomainShader)
	{
		mContext->DSSetShader(pDomainShader, 0, 0);
	}

	void GraphicsDevice_DX11::BindGeometryShader(ID3D11GeometryShader* pGeometryShader)
	{
		mContext->GSSetShader(pGeometryShader, 0, 0);
	}

	void GraphicsDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader)
	{
		mContext->PSSetShader(pPixelShader, 0, 0);
	}

	void GraphicsDevice_DX11::BindComputeShader(ID3D11ComputeShader* computeShader)
	{
		mContext->CSSetShader(computeShader, 0, 0);
	}

	void GraphicsDevice_DX11::Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
	{
		mContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	}

	void GraphicsDevice_DX11::BindViewports(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicsDevice_DX11::SetData(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		mContext->Map(buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		memcpy(subRes.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicsDevice_DX11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
	{
		mContext->RSSetState(pRasterizerState);
	}

	void GraphicsDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
	{
		mContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}

	void GraphicsDevice_DX11::BindBlendState(ID3D11BlendState* pBlendState)
	{
		mContext->OMSetBlendState(pBlendState, nullptr, 0xffffff);
	}

	void GraphicsDevice_DX11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		if (eShaderStage::VS == stage)
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);

		if (eShaderStage::HS == stage)
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);

		if (eShaderStage::DS == stage)
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);

		if (eShaderStage::GS == stage)
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);

		if (eShaderStage::PS == stage)
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);

		if (eShaderStage::CS == stage)
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
	}

	void GraphicsDevice_DX11::BindShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV)
	{
		if (eShaderStage::VS == stage)
			mContext->VSSetShaderResources(startSlot, 1, ppSRV);

		if (eShaderStage::HS == stage)
			mContext->HSSetShaderResources(startSlot, 1, ppSRV);

		if (eShaderStage::DS == stage)
			mContext->DSSetShaderResources(startSlot, 1, ppSRV);

		if (eShaderStage::GS == stage)
			mContext->GSSetShaderResources(startSlot, 1, ppSRV);

		if (eShaderStage::PS == stage)
			mContext->PSSetShaderResources(startSlot, 1, ppSRV);

		if (eShaderStage::CS == stage)
			mContext->CSSetShaderResources(startSlot, 1, ppSRV);
	}

	void GraphicsDevice_DX11::BindUnorderedAccessViews(UINT startSlot, ID3D11UnorderedAccessView** ppUnorderedAccessViews, const UINT* pUAVInitialCounts)
	{
		mContext->CSSetUnorderedAccessViews(startSlot, 1, ppUnorderedAccessViews, pUAVInitialCounts);
	}

	void GraphicsDevice_DX11::Clear()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTarget->GetRTV().Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencil->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void GraphicsDevice_DX11::AdjustViewport()
	{
		// ViewPort, RenderTaget
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		D3D11_VIEWPORT mViewPort = {};
		mViewPort.TopLeftX = 0.0f;
		mViewPort.TopLeftY = 0.0f;
		mViewPort.Width = (FLOAT)(winRect.right - winRect.left);
		mViewPort.Height = (FLOAT)(winRect.bottom - winRect.top);
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;

		BindViewports(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTarget->GetRTV().GetAddressOf(), mDepthStencil->GetDSV().Get());
	}

	void GraphicsDevice_DX11::ReSizeGrphicDevice(D3D11_VIEWPORT viewport)
	{
		renderer::renderTarget->Reset();
		mRenderTarget->Reset();
		mDepthStencil->Reset();

		HRESULT hr = mSwapChain->ResizeBuffers(0, // 현재 개수 유지
			(UINT)viewport.Width, // 해상도 변경
			(UINT)viewport.Height,
			DXGI_FORMAT_UNKNOWN, // 현재 포맷 유지
			0);

		// Get render target by Swapchain
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(renderTarget.GetAddressOf()));

		D3D11_TEXTURE2D_DESC desc;
		renderTarget->GetDesc(&desc);

		mRenderTarget->Create(renderTarget);
		renderer::renderTarget = mRenderTarget;

		mDepthStencil = std::make_shared<Texture>();
		mDepthStencil->Create(viewport.Width, viewport.Height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);

		BindViewports(&viewport);
		mContext->OMSetRenderTargets(1, mRenderTarget->GetRTV().GetAddressOf(),
			mDepthStencil->GetDSV().Get());

		if (renderer::postProcessing)
		{
			renderer::postProcessing->Reset();
			renderer::postProcessing->Create(viewport.Width, viewport.Height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
			renderer::postProcessing->BindShaderResource(eShaderStage::PS, 60);
		}
	}

	void GraphicsDevice_DX11::Draw(UINT VertexCount, UINT StartVertexLocation)
	{
		mContext->Draw(VertexCount, StartVertexLocation);
	}

	void GraphicsDevice_DX11::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		mContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
	}

	void GraphicsDevice_DX11::DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
	{
		mContext->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	}

	void GraphicsDevice_DX11::Present()
	{
		// 백버퍼에 그려준다
		mSwapChain->Present(0, 0);
	}

	void GraphicsDevice_DX11::Render()
	{
		//set costant buffer 
		renderer::constantBuffers[(UINT)graphics::eCBType::Transform]->Bind(eShaderStage::VS);

		//// Input Assembeler 단계에 버텍스버퍼 정보 지정
		//renderer::rectMesh->BindBuffer();


		Vector4 pos(0.5f, 0.2f, 0.0f, 0.0f);
		renderer::constantBuffers[(UINT)graphics::eCBType::Transform]->SetData(&pos);

		// Set Inputlayout, shader
		//renderer::shader->Bind();


		DrawIndexed(6, 0, 0);
	}

	Viewport GraphicsDevice_DX11::GetViewPort()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		Viewport viewport = {};
		viewport.top_left_x = 0.0f;
		viewport.top_left_y = 0.0f;
		viewport.width = (FLOAT)(winRect.right - winRect.left);
		viewport.height = (FLOAT)(winRect.bottom - winRect.top);
		viewport.min_depth = 0.0f;
		viewport.max_depth = 1.0f;

		return viewport;
	}
}
