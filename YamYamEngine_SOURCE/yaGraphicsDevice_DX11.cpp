#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"
#include "yaRenderer.h"

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

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

        swapChainDesc.OutputWindow = application.GetHwnd();	                // Front Buffer 를 출력시킬 윈도우 핸들
        swapChainDesc.Windowed = true;		                // 윈도우, 전체화면 모드
        swapChainDesc.BufferCount = 2;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferDesc.Width = application.GetWidth();
        swapChainDesc.BufferDesc.Height = application.GetHeight();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        if (!CreateSwapChain(swapChainDesc))
            return;

        // Get render target by Swapchain
        hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mFrameBuffer.GetAddressOf());

        // Create Rendertarget view
        hr = mDevice->CreateRenderTargetView(mFrameBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf());
        


        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
        textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
        textureDesc.Width = application.GetWidth();
        textureDesc.Height = application.GetWidth();
        textureDesc.ArraySize = application.GetHeight();
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.MipLevels = 1;
        textureDesc.MiscFlags = (UINT)ResourceMiscFlag::NONE;

        if (!CreateTexture(textureDesc))
            return;

        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
    }

    GraphicsDevice_DX11::~GraphicsDevice_DX11()
    {

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

        return true;
    }

    bool GraphicsDevice_DX11::CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data)
    {
        // System -> GPU
        D3D11_BUFFER_DESC bufferdesc = {};

        bufferdesc.ByteWidth = sizeof(renderer::Vertex) * 3;
        bufferdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
        bufferdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
        bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA tSubData = {};
        tSubData.pSysMem = renderer::vertexes;

        if (FAILED(mDevice->CreateBuffer(&bufferdesc, &tSubData, buffer)))
            return false;

        return true;
    }

    bool GraphicsDevice_DX11::CreateTexture(const D3D11_TEXTURE2D_DESC desc)
    {
        // Create Depth Stencil Buffer
        if (FAILED(mDevice->CreateTexture2D(&desc, nullptr, mDepthStencilBuffer.GetAddressOf())))
            return false;

        // Create Depth Stencil Buffer View
        if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
            return false;

        return true;
    }

    bool GraphicsDevice_DX11::CreateSampler(/*const SamplerDesc* desc, Sampler* sampler*/) 
    {

        return false;
    }

    bool GraphicsDevice_DX11::CreateShader()
    {
        ID3DBlob* vsBlob = nullptr;

        // Triangle Vertex Shader
        std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
        shaderPath += "\\Shaders_SOURCE\\";

        std::wstring vsPath(shaderPath.c_str());
        vsPath += L"TriangleVS.hlsl";
        D3DCompileFromFile(vsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
            , "main", "vs_4_0", 0, 0, &renderer::triangleVSBlob, &renderer::errorBlob);

        mDevice->CreateVertexShader(renderer::triangleVSBlob->GetBufferPointer(), renderer::triangleVSBlob->GetBufferSize(), nullptr, &renderer::triangleVSShader);

        std::wstring psPath(shaderPath.c_str());
        psPath += L"TrianglePS.hlsl";
        D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
            , "main", "ps_5_0", 0, 0, &renderer::trianglePSBlob, &renderer::errorBlob);

        mDevice->CreateVertexShader(renderer::trianglePSBlob->GetBufferPointer(), renderer::trianglePSBlob->GetBufferSize(), nullptr, &renderer::trianglePSShader);

        return false;
    }

    void GraphicsDevice_DX11::Draw()
    {
        FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);

        mSwapChain->Present(0, 0);
    }
}
