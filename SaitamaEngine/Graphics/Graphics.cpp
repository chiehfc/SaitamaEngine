#include "pch.h"
#include "Graphics.h"

using Microsoft::WRL::ComPtr;

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    if (!InitializeDirectX(hwnd, width, height)) 
    {
        return false;
    }
    return true;
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


    // First, retrieve the underlying DXGI Device from the D3D Device.
    ComPtr<IDXGIDevice1> dxgiDevice;
    DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

    // Identify the physical adapter (GPU or card) this device is running on.

    ComPtr<IDXGIAdapter> dxgiAdapter;
    DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

    // And obtain the factory object that created it.
    ComPtr<IDXGIFactory2> dxgiFactory;
    DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

    return true;
}
