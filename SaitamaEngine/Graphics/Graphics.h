#pragma once

#include "..\\Shaders.h"

class Graphics
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
    void Clear();

private:
    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();


    void CreateDevice();

    void CreateResource();

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    VertexShader m_vertexShader;    
};

