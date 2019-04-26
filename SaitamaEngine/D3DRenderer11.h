#pragma once
#include "IRenderer.h"

#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Timer.h"
#include "Imgui\\imgui.h"
#include "Imgui\\imgui_impl_win32.h"
#include "Imgui\\imgui_impl_dx11.h"
#include "RenderableGameObject.h"
#include "Light.h"
#include "GameObjectFactory.h"



class D3DRenderer11 : public IRenderer
{
public:

    static D3DRenderer11 *GetInstance();
    ID3D11Device1 *GetDevice() { return m_d3dDevice.Get(); }
    ID3D11DeviceContext1 *GetDeviceContext() { return m_d3dContext.Get(); }

    StrongGameObjectPtr GetGameObject();

    StrongGameObjectPtr GetLight() { return m_light; }

    bool Initialize(HWND hwnd, int width, int height);
    void Clear();
    virtual void VShutdown() {  }

    virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB)
    {
        m_backgroundColor[0] = float(bgA) / 255.0f;
        m_backgroundColor[1] = float(bgR) / 255.0f;
        m_backgroundColor[2] = float(bgG) / 255.0f;
        m_backgroundColor[3] = float(bgB) / 255.0f;
    }

    virtual bool VPreRender();
    virtual bool VPostRender();
    virtual HRESULT VOnRestore();
    virtual void VCalcLighting(Lights *lights, int maximumLights);

    // These three functions are done for each shader, not as a part of beginning the render - so they do nothing in D3D11.
    virtual void VSetWorldTransform(const Matrix *m) {  }
    virtual void VSetViewTransform(const Matrix *m) {  }
    virtual void VSetProjectionTransform(const Matrix *m) {  }

    virtual void VDrawLine(const Vector3& from, const Vector3& to, const Color& color) {}
    
    //virtual shared_ptr<IRenderState> VPrepareAlphaPass();
    virtual shared_ptr<IRenderState> VPrepareSkyBoxPass();

    //HRESULT CompileShader(LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
    //HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:
    float m_backgroundColor[4];


private:
    bool InitializeDirectX(HWND hwnd);
    bool InitializeShaders();
    bool InitializeScene();
    void CreateDevice();

    
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
    PixelShader m_pixelShader_noLight;
    ConstantBuffer<CB_VS_vertexshader> m_constantBuffer;
    ConstantBuffer<CB_PS_light> m_cb_ps_light;


    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_myTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pikachuTexture;

    int m_windowWidth = 0;
    int m_windowHeight = 0;

    Timer m_fpsTimer;

    StrongGameObjectPtr m_gameObject;
    StrongGameObjectPtr m_light;
    GameObjectFactory m_factory;


    static D3DRenderer11 *m_instance;


};

class D3DRendererSkyBoxPass11 : public IRenderState
{
protected:
    ID3D11DepthStencilState* m_pOldDepthStencilState;
    ID3D11DepthStencilState* m_pSkyboxDepthStencilState;

public:
    D3DRendererSkyBoxPass11();
    ~D3DRendererSkyBoxPass11();
    std::string VToString() { return "D3DRendererSkyBoxPass11"; }
};
