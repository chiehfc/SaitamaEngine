#pragma once

#include "..\\Shaders.h"
#include "..\\Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "..\\VertexBuffer.h"
#include "..\\IndexBuffer.h"
#include "..\\ConstantBuffer.h"
#include "..\\Camera.h"

class Graphics
{
public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();
    void Clear();

    Camera *GetCamera();

private:
    bool InitializeDirectX(HWND hwnd);
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
    Microsoft::WRL::ComPtr<ID3D11Texture2D>  m_depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    
    VertexShader m_vertexShader;    
    PixelShader m_pixelShader;
    ConstantBuffer<CB_VS_vertexshader> m_constantBuffer;
        
    VertexBuffer<Vertex> m_vertexBuffer;
    IndexBuffer m_indexBuffer;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_myTexture;

    int m_windowWidth = 0;
    int m_windowHeight = 0;

    Camera m_camera;
};

