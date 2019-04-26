#pragma once

#include "Shaders.h"

// Forward declarations
class SceneNode;
class Scene;

class SkyNode : public SceneNode
{
protected:
    DWORD					m_numVerts;
    DWORD					m_sides;
    const char *			m_textureBaseName;
    shared_ptr<CameraNode>	m_camera;
    bool					m_bActive;

    std::string GetTextureName(const int side);

public:
    SkyNode(const char *textureFile);
    virtual ~SkyNode() { }
    HRESULT VPreRender(Scene *pScene);
    bool VIsVisible(Scene *pScene) const { return m_bActive; }
};

class D3DSkyNode11 : public SkyNode
{
public:
    D3DSkyNode11(const char *pTextureBaseName);
    virtual ~D3DSkyNode11();
    HRESULT VOnRestore(Scene *pScene);
    HRESULT VRender(Scene *pScene);

protected:
    ID3D11Buffer*               m_pIndexBuffer;
    ID3D11Buffer*               m_pVertexBuffer;

    VertexShader m_vertexShader;
    PixelShader m_pixelShader;
};

