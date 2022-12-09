#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"
#include "yaRenderer.h"

extern ya::Application application;


namespace ya::graphics
{
    GraphicsDevice_DX11::GraphicsDevice_DX11()
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

        swapChainDesc.OutputWindow = application.GetHwnd();	                // Front Buffer �� ��½�ų ������ �ڵ�
        swapChainDesc.Windowed = true;		                // ������, ��üȭ�� ���
        swapChainDesc.BufferCount = 2;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���� ������ ����� �������� �ʴ´�.

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
        


        // DepthStencilTexture
        D3D11_TEXTURE2D_DESC texdesc = {};

        texdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

        texdesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
        texdesc.CPUAccessFlags = 0;

        texdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texdesc.Width = (UINT)1600;
        texdesc.Height = (UINT)900;
        texdesc.ArraySize = 1;

        texdesc.SampleDesc.Count = 1;
        texdesc.SampleDesc.Quality = 0;

        texdesc.MipLevels = 0;
        texdesc.MiscFlags = 0;

        if (!CreateTexture(texdesc))
            return;


        //RECT winRect;
        //GetClientRect(application.GetHwnd(), &winRect);
        //mViewPort = { 0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top)};
        //BindViewports(&mViewPort);
        //mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
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

    bool GraphicsDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* initial_data, ID3D11Buffer** buffer)
    {
        if (FAILED(mDevice->CreateBuffer(desc, initial_data, buffer)))
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

    bool GraphicsDevice_DX11::CreateShader(ShaderStage stage)
    {
        ID3DBlob* vsBlob = nullptr;

        // Triangle Vertex Shader
        std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
        shaderPath += "\\Shaders_SOURCE\\";

        std::wstring vsPath(shaderPath.c_str());
        vsPath += L"TriangleVS.hlsl";
        D3DCompileFromFile(vsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
            , "VS_Test", "vs_5_0", 0, 0, &renderer::triangleVSBlob, &renderer::errorBlob);

        mDevice->CreateVertexShader(renderer::triangleVSBlob->GetBufferPointer()
            , renderer::triangleVSBlob->GetBufferSize()
            , nullptr
            , &renderer::triangleVSShader);

        std::wstring psPath(shaderPath.c_str());
        psPath += L"TrianglePS.hlsl";
        D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
            , "PS_Test", "ps_5_0", 0, 0, &renderer::trianglePSBlob, &renderer::errorBlob);

        mDevice->CreatePixelShader(renderer::trianglePSBlob->GetBufferPointer()
            , renderer::trianglePSBlob->GetBufferSize()
            , nullptr
            , &renderer::trianglePSShader);


        // Input layout ���� ���� ����
        D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {};

        arrLayout[0].AlignedByteOffset = 0;
        arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        arrLayout[0].InputSlot = 0;
        arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        arrLayout[0].SemanticName = "POSITION";
        arrLayout[0].SemanticIndex = 0;

        arrLayout[1].AlignedByteOffset = 12;
        arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        arrLayout[1].InputSlot = 0;
        arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        arrLayout[1].SemanticName = "COLOR";
        arrLayout[1].SemanticIndex = 0;

        mDevice->CreateInputLayout(arrLayout, 2
            , renderer::triangleVSBlob->GetBufferPointer()
            , renderer::triangleVSBlob->GetBufferSize()
            , &renderer::triangleLayout);

        return true;
    }

    void GraphicsDevice_DX11::BindViewports(D3D11_VIEWPORT* viewPort)
    {
        mContext->RSSetViewports(1, viewPort);
    }

    void GraphicsDevice_DX11::BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
    {
        D3D11_MAPPED_SUBRESOURCE subRes = {};
        mContext->Map(buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subRes);
        memcpy(subRes.pData, data, size);
        mContext->Unmap(buffer, 0);
    }

    void GraphicsDevice_DX11::SetConstantBuffer(ShaderStage stage, CBTYPES type, ID3D11Buffer* buffer)
    {
        switch (stage)
        {
        case ya::graphics::ShaderStage::VS:
            {
                mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        case ya::graphics::ShaderStage::HS:
            {
                mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        case ya::graphics::ShaderStage::DS:
            {
                mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        case ya::graphics::ShaderStage::GS:
            {
                mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        case ya::graphics::ShaderStage::PS:
            {
                mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        case ya::graphics::ShaderStage::CS:
            {
                mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
            }
            break;
        default:
            break;
        }
    }

    void GraphicsDevice_DX11::Draw()
    {
        // ���ҽ� ���ε�
        //D3D11_MAPPED_SUBRESOURCE sub = {};
        //mContext->Map(renderer::triangleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
        //memcpy(sub.pData, renderer::vertexes, sizeof(renderer::Vertex) * 3);
        //mContext->Unmap(renderer::triangleBuffer, 0);
        
        // ȭ�� �����ֱ�
        FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
        mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

        //set costant buffer 
        SetConstantBuffer(ShaderStage::VS, CBTYPES::TRANSFORM, renderer::triangleConstantBuffer);

        // ViewPort, RenderTaget
        RECT winRect;
        GetClientRect(application.GetHwnd(), &winRect);
        D3D11_VIEWPORT mViewPort = { 0.0f, 0.0f, (FLOAT)(winRect.right - winRect.left), (FLOAT)(winRect.bottom - winRect.top) };
        BindViewports(&mViewPort);
        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

        // Input Assembeler �ܰ迡 ���ؽ����� ���� ����
        UINT vertexSize = sizeof(renderer::Vertex);
        UINT offset = 0;
        mContext->IASetVertexBuffers(0, 1, &renderer::triangleVertexBuffer, &vertexSize, &offset);
        mContext->IASetIndexBuffer(renderer::triangleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        mContext->IASetInputLayout(renderer::triangleLayout);
        mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // ������ ���̴� ����
        mContext->VSSetShader(renderer::triangleVSShader, 0, 0);
        mContext->PSSetShader(renderer::trianglePSShader, 0, 0);

        mContext->DrawIndexed(6, 0, 0);


        // ����ۿ� �׷��ش�
        mSwapChain->Present(0, 0);
    }
}
