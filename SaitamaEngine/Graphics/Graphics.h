#pragma once

#include "..\\Shaders.h"
#include "..\\Vertex.h"

class Graphics
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
    void Clear();

private:
    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();
    bool InitializeScene();

    void CreateDevice();

    void CreateResource();

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    
    VertexShader m_vertexShader;    
    PixelShader m_pixelShader;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};

