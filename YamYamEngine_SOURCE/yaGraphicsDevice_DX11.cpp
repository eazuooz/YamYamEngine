#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"
#include "yaRenderer.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaConstantBuffer.h"

extern ya::Application application;


namespace ya::graphics
{
    GraphicsDevice_DX11::GraphicsDevice_DX11()
    {
        HWND hWnd = application.GetHwnd();

        // Device , Device Context
        UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

        HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
                                        , DeviceFlag, nullptr, 0
                                        , D3D11_SDK_VERSION
                                        , mDevice.GetAddressOf()
                                        , &FeatureLevel
                                        , mContext.GetAddressOf());

        UINT quility = 0;
        mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
        

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

    bool GraphicsDevice_DX11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
    {
        
        if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
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

    void GraphicsDevice_DX11::BindVertexShader(ID3D11VertexShader* pVertexShader)
    {
        mContext->VSSetShader(pVertexShader, 0, 0);
    }

    void GraphicsDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader)
    {
        mContext->PSSetShader(pPixelShader, 0, 0);
    }

    void GraphicsDevice_DX11::BindViewports(D3D11_VIEWPORT* viewPort)
    {
        mContext->RSSetViewports(1, viewPort);
    }

    void GraphicsDevice_DX11::BindBuffer(ID3D11Buffer* buffer, void* data, UINT size)
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

    void GraphicsDevice_DX11::SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
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
        
    }

    void GraphicsDevice_DX11::SetShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV)
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
    }

    void GraphicsDevice_DX11::Clear()
    {
        FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
        mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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
        mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
    }

    void GraphicsDevice_DX11::Draw(UINT VertexCount, UINT StartVertexLocation)
    {
        mContext->Draw(VertexCount, StartVertexLocation);
    }

    void GraphicsDevice_DX11::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
    {
        mContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    }

    void GraphicsDevice_DX11::Present()
    {
        // 백버퍼에 그려준다
        mSwapChain->Present(0, 0);
    }

    void GraphicsDevice_DX11::Render()
    {
        //set costant buffer 
        renderer::constantBuffers[(UINT)graphics::eCBType::Transform]->SetPipline(eShaderStage::VS);

        //// Input Assembeler 단계에 버텍스버퍼 정보 지정
        //renderer::rectMesh->BindBuffer();
        
        
        Vector4 pos(0.5f, 0.2f, 0.0f, 0.0f);
        renderer::constantBuffers[(UINT)graphics::eCBType::Transform]->Bind(&pos);

        // Set Inputlayout, shader
        //renderer::shader->Bind();


        DrawIndexed(6, 0, 0);
    }
}
