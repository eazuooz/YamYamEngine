#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"

extern ya::Application application;

namespace ya::graphics
{
    GraphicsDevice_DX11::GraphicsDevice_DX11(ValidationMode validationMode)
    {
        HWND hWnd = application.GetHwnd();

        // Device , Device Context
        UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

        ID3D11Device* pDevice = nullptr;
        HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
                                        , DeviceFlag, nullptr, 0
                                        , D3D11_SDK_VERSION
                                        , mDevice.GetAddressOf()
                                        , &FeatureLevel
                                        , mContext.GetAddressOf());

        // Swap Chain
        SwapChain swapChain;
        swapChain.desc.width = application.GetWidth();
        swapChain.desc.height = application.GetHeight();
        swapChain.desc.buffer_count = 2;
        if (!CreateSwapChain(&swapChain.GetDesc(), hWnd, &swapChain))
            return;

        // Get render target by Swapchain
        hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mFrameBuffer.GetAddressOf());

        // Create Rendertarget view
        hr = mDevice->CreateRenderTargetView(mFrameBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf());
        
        Texture texture;
        texture.desc.bind_flags = BindFlag::DEPTH_STENCIL;
        texture.desc.format = Format::D24_UNORM_S8_UINT;
        texture.desc.usage = Usage::DEFAULT;
        texture.desc.width = application.GetWidth();
        texture.desc.height = application.GetHeight();
        texture.desc.array_size = 1;
        texture.desc.sample_count = 1;
        texture.desc.misc_flags = ResourceMiscFlag::NONE;

        SubresourceData sub;
        if (!CreateTexture(&texture.GetDesc(), &sub, &texture))
            return;

        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
    }

    GraphicsDevice_DX11::~GraphicsDevice_DX11()
    {

    }

    bool GraphicsDevice_DX11::CreateSwapChain(const SwapChainDesc* desc, HWND hWnd, SwapChain* swapchain)
    {
        DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

        dxgiDesc.OutputWindow = hWnd;	                // Front Buffer 를 출력시킬 윈도우 핸들
        dxgiDesc.Windowed = true;		                // 윈도우, 전체화면 모드
        dxgiDesc.BufferCount = desc->buffer_count;
        dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

        dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgiDesc.BufferDesc.Width = desc->width;
        dxgiDesc.BufferDesc.Height = desc->height;
        dxgiDesc.BufferDesc.Format = (DXGI_FORMAT)desc->format;
        dxgiDesc.BufferDesc.RefreshRate.Numerator = 144;
        dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
        dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        dxgiDesc.SampleDesc.Count = 1;
        dxgiDesc.SampleDesc.Quality = 0;

        Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
        Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

        if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
            return false;

        if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
            return false;

        if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
            return false;
        
        if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &dxgiDesc, mSwapChain.GetAddressOf())))
            return false;

        return true;
    }

    bool GraphicsDevice_DX11::CreateBuffer(const GPUBufferDesc* desc, const void* initial_data, GPUBuffer* buffer) 
    {

        return false;
    }

    bool GraphicsDevice_DX11::CreateTexture(const TextureDesc* desc, const SubresourceData* initial_data, Texture* texture) 
    {
        D3D11_TEXTURE2D_DESC dxgiDesc = {};

        dxgiDesc.BindFlags = (D3D11_BIND_FLAG)desc->bind_flags;

        dxgiDesc.Usage = (D3D11_USAGE)desc->usage;
        dxgiDesc.CPUAccessFlags = 0;

        dxgiDesc.Format = (DXGI_FORMAT)desc->format;
        dxgiDesc.Width = desc->width;
        dxgiDesc.Height = desc->height;
        dxgiDesc.ArraySize = desc->array_size;

        dxgiDesc.SampleDesc.Count = desc->sample_count;
        dxgiDesc.SampleDesc.Quality = 0;

        dxgiDesc.MipLevels = desc->mip_levels;
        dxgiDesc.MiscFlags = (UINT)desc->misc_flags;

        // Create Depth Stencil Buffer
        if (FAILED(mDevice->CreateTexture2D(&dxgiDesc, nullptr, mDepthStencilBuffer.GetAddressOf())))
            return false;

        // Create Depth Stencil Buffer View
        if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
            return false;

        return true;
    }

    bool GraphicsDevice_DX11::CreateSampler(const SamplerDesc* desc, Sampler* sampler) 
    {

        return false;
    }

    bool GraphicsDevice_DX11::CreateShader(ShaderStage stage, const void* shadercode, size_t shadercode_size, Shader* shader) 
    {
        return false;
    }

    void GraphicsDevice_DX11::Draw()
    {
        FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);

        mSwapChain->Present(1, 0);
    }
}
