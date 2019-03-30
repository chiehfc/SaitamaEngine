#include "pch.h"
#include "Graphics.h"
#include <string>


using Microsoft::WRL::ComPtr;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;

    m_fpsTimer.Start();

    if (!InitializeDirectX(hwnd)) 
    {
        return false;
    }

    if (!InitializeShaders())
    {
        return false;
    }

    if (!InitializeScene())
    {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(m_d3dDevice.Get(), m_d3dContext.Get());
    ImGui::StyleColorsDark();

    return true;
}

void Graphics::RenderFrame()
{
    float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->IASetInputLayout(m_vertexShader.GetInputLayout());
    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_d3dContext->RSSetState(m_rasterizerState.Get());    
    m_d3dContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
    m_d3dContext->OMSetBlendState(m_blendState.Get(), NULL, 0xFFFFFFFF);
    m_d3dContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    m_d3dContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
    m_d3dContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);

    UINT offset = 0;

    m_camera.UpdateViewMatrix();
    static float alpha = 0.5f;
    // Update Constant Buffer            
    {
        DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
        m_constantBuffer.data.mat = worldMatrix * DirectX::XMMATRIX(m_camera.GetView()) * DirectX::XMMATRIX(m_camera.Proj());
        // from row major(DirectXMath library) to column major(HLSL)
        m_constantBuffer.data.mat = DirectX::XMMatrixTranspose(m_constantBuffer.data.mat);
        if (!m_constantBuffer.ApplyChanges())
        {
            return;
        }
        m_d3dContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        m_cb_ps_pixelshader.data.alpha = alpha;
        if (!m_cb_ps_pixelshader.ApplyChanges())
        {
            return;
        }
        m_d3dContext->PSSetConstantBuffers(0, 1, m_cb_ps_pixelshader.GetAddressOf());

        m_d3dContext->PSSetShaderResources(0, 1, m_myTexture.GetAddressOf());
        m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &offset);
        m_d3dContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        m_d3dContext->DrawIndexed(m_indexBuffer.BufferSize(), 0, 0);
    }
    

    // Draw Text
    static int fpsCounter = 0;
    static std::wstring fpsString = L"SAITAMA!!! FPS: 0";
    fpsCounter += 1;
    if (m_fpsTimer.GetMillisecondsElapsed() > 1000.0)
    {
        fpsString = L"SAITAMA!!! FPS: " + std::to_wstring(fpsCounter);
        fpsCounter = 0;
        m_fpsTimer.Restart();
    }
    
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), fpsString.c_str(), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
    m_spriteBatch->End();




    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");
    ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.0f, 1.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    m_swapChain->Present(1, NULL);
}

void Graphics::Clear()
{
    // Clear the views.
    //m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    //m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
    CreateDevice();

    // First, retrieve the underlying DXGI Device from the D3D Device.
    ComPtr<IDXGIDevice1> dxgiDevice;
    DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

    // Identify the physical adapter (GPU or card) this device is running on.

    ComPtr<IDXGIAdapter> dxgiAdapter;
    DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

    // And obtain the factory object that created it.
    ComPtr<IDXGIFactory2> dxgiFactory;
    DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

    // Create a descriptor for the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = m_windowWidth;
    swapChainDesc.Height = m_windowHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
    fsSwapChainDesc.Windowed = TRUE;

    // Create a SwapChain from a Win32 window.
    DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
        m_d3dDevice.Get(),
        hwnd,
        &swapChainDesc,
        &fsSwapChainDesc,
        nullptr,
        m_swapChain.ReleaseAndGetAddressOf()
    ));

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));
        
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = m_windowWidth;
    depthStencilDesc.Height = m_windowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
        
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, m_depthStencilState.GetAddressOf()));

    // Set the viewport. - Rasterizer
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 0.0f, 1.0f);
    m_d3dContext->RSSetViewports(1, &viewport);

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    DX::ThrowIfFailed(m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf()));
    
    // Create Blend State 
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
    
    blendDesc.RenderTarget[0] = rtbd;
    DX::ThrowIfFailed(m_d3dDevice->CreateBlendState(&blendDesc, m_blendState.GetAddressOf()));

    // Fonts
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_d3dContext.Get());
    m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_d3dDevice.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

    // SamplerState
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DX::ThrowIfFailed(m_d3dDevice->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf()));

    return true;
}

bool Graphics::InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    if (!m_vertexShader.Initialize(m_d3dDevice, L"..\\Debug\\vertexshader.cso", layout, numElements))
    {
        return false;
    }

    if (!m_pixelShader.Initialize(m_d3dDevice, L"..\\Debug\\pixelshader.cso"))
    {
        return false;
    }
    
    

    return true;
}

bool Graphics::InitializeScene()
{
    // Vertex Buffer
    Vertex v[] = 
    {
        Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 0.0f),
        Vertex( 0.5f,  0.5f, 0.0f, 1.0f, 0.0f),        
        Vertex( 0.5f, -0.5f, 0.0f, 1.0f, 1.0f),

        Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 1.0f, 0.0f, 0.0f),
        Vertex(0.5f,  0.5f, 1.0f, 1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 1.0f, 1.0f, 1.0f)
    };

    HRESULT hr = m_vertexBuffer.Initialize(m_d3dDevice.Get(), v, ARRAYSIZE(v));
    DX::ThrowIfFailed(hr);


    // Index Buffer
    DWORD indices[] = 
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 1,
        4, 1, 0,
        1, 5, 6,
        1, 6, 2,
        3, 2, 6,
        3, 6, 7,
        4, 6, 5,
        4, 7, 6,
        4, 0, 3,
        4, 3, 7
    };    

    hr = m_indexBuffer.Initialize(m_d3dDevice.Get(), indices, ARRAYSIZE(indices));
    DX::ThrowIfFailed(hr);

    // Texture    
    hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), L"Data\\Textures\\pikachu.jfif", nullptr, m_myTexture.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), L"Data\\Textures\\DetectivePikachu.png", nullptr, m_pikachuTexture.GetAddressOf());
    DX::ThrowIfFailed(hr);

    // Constant Buffer
    hr = m_constantBuffer.Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
    DX::ThrowIfFailed(hr);

    hr = m_cb_ps_pixelshader.Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
    DX::ThrowIfFailed(hr);

    return true;
}


void Graphics::CreateDevice() 
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    m_featureLevel = D3D_FEATURE_LEVEL_11_0;

    static const D3D_FEATURE_LEVEL featureLevels[] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
    ));

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));
}


Camera *Graphics::GetCamera()
{
    return &m_camera;
}