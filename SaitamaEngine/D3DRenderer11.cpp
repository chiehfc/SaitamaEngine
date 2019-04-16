#include "pch.h"
#include "D3DRenderer11.h"
#include <string>
#include "TransformComponent.h"
#include "RenderComponent.h"


D3DRenderer11 *D3DRenderer11::m_instance = nullptr;

D3DRenderer11 *D3DRenderer11::GetInstance()
{
    if (m_instance)
    {
        return m_instance;
    } else
    {
        m_instance = new D3DRenderer11();

    }
    return m_instance;
}

HRESULT D3DRenderer11::VOnRestore()
{
    return S_OK;
}

void D3DRenderer11::VCalcLighting(Lights *lights, int maximumLights)
{
    int count = 0;
    if (lights && lights->size() > 0)
    {
        for (Lights::iterator i = lights->begin(); i != lights->end(); ++i)
        {
            shared_ptr<LightNode> light = *i;
            shared_ptr<D3DLightNode11> light11 = static_pointer_cast<D3DLightNode11>(light);
            
            for (int i = 0; i < 8; i++)
            {
                m_cb_ps_light.data.dynamicLightColor[i] = light11->GetLightProps()->color;
                m_cb_ps_light.data.dynamicLightPosition[i] =
                    Vector4(light11->GetPosition().x,
                        light11->GetPosition().y,
                        light11->GetPosition().z,
                        1.0f);
            }
            m_cb_ps_light.data.dynamicLightStrength = light11->GetLightProps()->lightStrength;
            m_cb_ps_light.data.dynamicLightAttenuation_a = light11->GetLightProps()->attenuation[0];
            m_cb_ps_light.data.dynamicLightAttenuation_b = light11->GetLightProps()->attenuation[1];
            m_cb_ps_light.data.dynamicLightAttenuation_c = light11->GetLightProps()->attenuation[2];
            count++;
        }
    }

    m_cb_ps_light.data.numberOfLights = count;

    m_cb_ps_light.ApplyChanges();
    m_d3dContext->PSSetConstantBuffers(0, 1, m_cb_ps_light.GetAddressOf());
}

bool D3DRenderer11::VPreRender()
{
    float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->RSSetState(m_rasterizerState.Get());
    m_d3dContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
    m_d3dContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
    m_d3dContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    return true;
}

bool D3DRenderer11::VPostRender(void)
{
    /*ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Light Controls");
    ImGui::DragFloat3("Ambient Light Color", &m_cb_ps_light.data.ambientLightColor.x, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Ambient Light Strength", &m_cb_ps_light.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);
    ImGui::NewLine();
    ImGui::DragFloat3("Dynamic Light Color", &m_cb_ps_light.data.dynamicLightColor[0].x, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Strength", &m_cb_ps_light.data.dynamicLightStrength, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation A", &m_cb_ps_light.data.dynamicLightAttenuation_a, 0.01f, 0.1f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation B", &m_cb_ps_light.data.dynamicLightAttenuation_b, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation C", &m_cb_ps_light.data.dynamicLightAttenuation_c, 0.01f, 0.0f, 10.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());*/



    m_swapChain->Present(1, NULL);
    return true;
}



//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
//HRESULT D3DRenderer11::CompileShader(LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
//{
//    HRESULT hr = S_OK;
//
//    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined( DEBUG ) || defined( _DEBUG )
//    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//    // Setting this flag improves the shader debugging experience, but still allows 
//    // the shaders to be optimized and to run exactly the way they will run in 
//    // the release configuration of this program.
//    dwShaderFlags |= D3DCOMPILE_DEBUG;
//#endif
//
//    ID3DBlob* pErrorBlob;
//    hr = D3DX11CompileFromMemory(pSrcData, SrcDataLen, pFileName, NULL, NULL, szEntryPoint, szShaderModel,
//        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
//    if (FAILED(hr))
//    {
//        if (pErrorBlob != NULL)
//            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
//        if (pErrorBlob) pErrorBlob->Release();
//        return hr;
//    }
//    if (pErrorBlob) pErrorBlob->Release();
//
//    return S_OK;
//}
//
//
//HRESULT D3DRenderer11::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
//{
//    HRESULT hr = S_OK;
//
//    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined( DEBUG ) || defined( _DEBUG )
//    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//    // Setting this flag improves the shader debugging experience, but still allows 
//    // the shaders to be optimized and to run exactly the way they will run in 
//    // the release configuration of this program.
//    dwShaderFlags |= D3DCOMPILE_DEBUG;
//#endif
//
//    ID3DBlob* pErrorBlob;
//    hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
//        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
//    if (FAILED(hr))
//    {
//        if (pErrorBlob != NULL)
//            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
//        if (pErrorBlob) pErrorBlob->Release();
//        return hr;
//    }
//    if (pErrorBlob) pErrorBlob->Release();
//
//    return S_OK;
//}
//
//
//void D3DRenderer11::VDrawLine(const Vec3& from, const Vec3& to, const Color& color)
//{
//    if (!m_pLineDrawer)
//    {
//        m_pLineDrawer = GCC_NEW D3DLineDrawer11();
//        m_pLineDrawer->OnRestore();
//
//
//    }
//    m_pLineDrawer->DrawLine(from, to, color);
//}
//
//
//
//
//shared_ptr<IRenderState> D3DRenderer11::VPrepareAlphaPass()
//{
//    return shared_ptr<IRenderState>(GCC_NEW D3DRendererAlphaPass11());
//}
//
//
//shared_ptr<IRenderState> D3DRenderer11::VPrepareSkyBoxPass()
//{
//    return shared_ptr<IRenderState>(GCC_NEW D3DRendererSkyBoxPass11());
//}

using Microsoft::WRL::ComPtr;

bool D3DRenderer11::Initialize(HWND hwnd, int width, int height)
{
    m_instance = this;
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


void D3DRenderer11::Clear()
{
    // Clear the views.
    //m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    //m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

bool D3DRenderer11::InitializeDirectX(HWND hwnd)
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

bool D3DRenderer11::InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    if (!m_vertexShader.Initialize(m_d3dDevice.Get(), L"..\\x64\\Debug\\vertexshader.cso", layout, numElements))
    {
        return false;
    }

    if (!m_pixelShader.Initialize(m_d3dDevice.Get(), L"..\\x64\\Debug\\pixelshader.cso"))
    {
        return false;
    }

    if (!m_pixelShader_noLight.Initialize(m_d3dDevice.Get(), L"..\\x64\\Debug\\pixelshader_nolight.cso"))
    {
        return false;
    }


    return true;
}

bool D3DRenderer11::InitializeScene()
{
    // Texture    
    HRESULT hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), L"Data\\Textures\\pikachu.jfif", nullptr, m_myTexture.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), L"Data\\Textures\\DetectivePikachu.png", nullptr, m_pikachuTexture.GetAddressOf());
    DX::ThrowIfFailed(hr);

    // Constant Buffer
    hr = m_constantBuffer.Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
    DX::ThrowIfFailed(hr);

    hr = m_cb_ps_light.Initialize(m_d3dDevice.Get(), m_d3dContext.Get());
    DX::ThrowIfFailed(hr);

    m_cb_ps_light.data.ambientLightColor = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
    m_cb_ps_light.data.ambientLightStrength = 1.0f;

    m_gameObject = m_factory.CreateGameObject("gameObject.xml", nullptr, nullptr, 0);
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr<TransformComponent>(m_gameObject->GetComponent<TransformComponent>(TransformComponent::g_Name));
    m_gameObject->SetPosition(pTransformComponent->GetPosition());
    m_gameObject->SetRotation(pTransformComponent->GetRotation());

    auto grid = m_factory.CreateGameObject("grid.xml", nullptr, nullptr, 0);
    //std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr<TransformComponent>(grid->GetComponent<TransformComponent>(TransformComponent::g_Name));
    //grid->SetPosition(pTransformComponent->GetPosition());
    //grid->SetRotation(pTransformComponent->GetRotation());

    m_light = m_factory.CreateGameObject("light.xml", nullptr, nullptr, 0);
    std::shared_ptr<TransformComponent> pLightTransform = MakeStrongPtr<TransformComponent>(m_light->GetComponent<TransformComponent>(TransformComponent::g_Name));
    m_light->SetPosition(pLightTransform->GetPosition());
    m_light->SetRotation(pLightTransform->GetRotation());

    return true;
}

void D3DRenderer11::CreateDevice()
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

StrongGameObjectPtr D3DRenderer11::GetGameObject()
{
    return m_gameObject;
}