#include "pch.h"
#include "Graphics.h"

using Microsoft::WRL::ComPtr;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    if (!InitializeDirectX(hwnd, width, height)) 
    {
        return false;
    }

    if (!InitializeShaders())
    {
        return false;
    }

    return true;
}

void Graphics::RenderFrame()
{
    float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
    m_swapChain->Present(1, NULL);
}

void Graphics::Clear()
{
    // Clear the views.
    //m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    //m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
    //ComPtr<ID3D11Device> device;
    //ComPtr<ID3D11DeviceContext> context;
    //DX::ThrowIfFailed(D3D11CreateDevice(
    //    nullptr,                            // specify nullptr to use the default adapter
    //    D3D_DRIVER_TYPE_HARDWARE,
    //    nullptr,
    //    creationFlags,
    //    featureLevels,
    //    _countof(featureLevels),
    //    D3D11_SDK_VERSION,
    //    device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
    //    &m_featureLevel,                    // returns feature level of device created
    //    context.ReleaseAndGetAddressOf()    // returns the device immediate context
    //));

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
       

    ID3D11RenderTargetView* nullViews[] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);

    // Create a descriptor for the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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

    return true;
}

bool Graphics::InitializeShaders()
{


    if (!m_vertexShader.Initialize(m_d3dDevice, L"..\\Debug\\vertexshader.cso"))     
    {
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = ARRAYSIZE(layout);
    
    HRESULT hr = m_d3dDevice->CreateInputLayout(layout, numElements, m_vertexShader.GetBuffer()->GetBufferPointer(), m_vertexShader.GetBuffer()->GetBufferSize(), m_inputLayout.GetAddressOf());
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