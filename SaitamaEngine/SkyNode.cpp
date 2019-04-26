#include "pch.h"

#include "D3DRenderer11.h"
#include "SceneNode.h"
#include "Scene.h"
#include "Shaders.h"
#include "SkyNode.h"

SkyNode::SkyNode(const char *pTextureBaseName)
    : SceneNode(INVALID_GAMEOBJECT_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky, &Matrix::Identity)
    , m_bActive(true)
{
    m_textureBaseName = pTextureBaseName;
}

HRESULT SkyNode::VPreRender(Scene *pScene)
{
    Vector3 cameraPos = m_camera->VGet()->ToWorld().Translation();
    Matrix mat = m_Props.ToWorld();
    mat.Translation(cameraPos);
    VSetTransform(&mat);

    return SceneNode::VPreRender(pScene);
}

D3DSkyNode11::D3DSkyNode11(const char *pTextureBaseName)
    : SkyNode(pTextureBaseName)
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    m_vertexShader.Initialize(D3DRenderer11::GetInstance()->GetDevice(), L"..\\x64\\Debug\\vertexshader.cso", layout, numElements);

    m_pixelShader.Initialize(D3DRenderer11::GetInstance()->GetDevice(), L"..\\x64\\Debug\\pixelshader.cso");

    m_pVertexBuffer = nullptr;
    m_pIndexBuffer = nullptr;
    //m_VertexShader.EnableLights(false);
}

D3DSkyNode11::~D3DSkyNode11()
{
    //SAFE_RELEASE(m_pVertexBuffer);
    //SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT D3DSkyNode11::VOnRestore(Scene *pScene)
{
    HRESULT hr;

    SceneNode::VOnRestore(pScene);

    m_camera = pScene->GetCamera();

    //SAFE_RELEASE(m_pVertexBuffer);
    //SAFE_RELEASE(m_pIndexBuffer);

    //V_RETURN(m_VertexShader.OnRestore(pScene));
    //V_RETURN(m_PixelShader.OnRestore(pScene));

    m_numVerts = 20;

    // Fill the vertex buffer. We are setting the tu and tv texture
    // coordinates, which range from 0.0 to 1.0
    Vertex *pVertices = new Vertex[m_numVerts];
    //GCC_ASSERT(pVertices && "Out of memory in D3DSkyNode11::VOnRestore()");
    if (!pVertices)
        return E_FAIL;


    // Loop through the grid squares and calc the values
    // of each index. Each grid square has two triangles:
    //
    //		A - B
    //		| / |
    //		C - D

    Vertex skyVerts[4];
    Color skyVertColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
    float dim = 50.0f;

    skyVerts[0].pos = Vector3(dim, dim, dim); skyVerts[0].texCoord = Vector2(1.0f, 0.0f);
    skyVerts[1].pos = Vector3(-dim, dim, dim); skyVerts[1].texCoord = Vector2(0.0f, 0.0f);
    skyVerts[2].pos = Vector3(dim, -dim, dim); skyVerts[2].texCoord = Vector2(1.0f, 1.0f);
    skyVerts[3].pos = Vector3(-dim, -dim, dim); skyVerts[3].texCoord = Vector2(0.0f, 1.0f);

    Vector3 triangle[3];
    triangle[0] = Vector3(0.f, 0.f, 0.f);
    triangle[1] = Vector3(5.f, 0.f, 0.f);
    triangle[2] = Vector3(5.f, 5.f, 0.f);

    Vector3 edge1 = triangle[1] - triangle[0];
    Vector3 edge2 = triangle[2] - triangle[0];

    Vector3 normal;
    normal = edge1.Cross(edge2);
    normal.Normalize();

    Matrix rotY;
    rotY.CreateRotationY(DirectX::XM_PI / 2.0f);
    Matrix rotX;
    rotX.CreateRotationX(-DirectX::XM_PI / 2.0f);

    m_sides = 5;

    for (DWORD side = 0; side < m_sides; side++)
    {
        for (DWORD v = 0; v < 4; v++)
        {
            Vector4 temp;
            if (side < m_sides - 1)
            {
                temp = DirectX::XMVector4Transform(Vector3(skyVerts[v].pos), rotY);
                //temp = rotY.Xform(Vec3(skyVerts[v].Pos));
            } else
            {
                skyVerts[0].texCoord = Vector2(1.0f, 1.0f);
                skyVerts[1].texCoord = Vector2(1.0f, 1.0f);
                skyVerts[2].texCoord = Vector2(1.0f, 1.0f);
                skyVerts[3].texCoord = Vector2(1.0f, 1.0f);

                temp = DirectX::XMVector4Transform(Vector3(skyVerts[v].pos), rotX);
            }
            skyVerts[v].pos = Vector3(temp.x, temp.y, temp.z);
        }
        memcpy(&pVertices[side * 4], skyVerts, sizeof(skyVerts));
    }

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * m_numVerts;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = pVertices;
    hr = D3DRenderer11::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
    //SAFE_DELETE(pVertices);
    if (FAILED(hr))
        return hr;


    // Loop through the grid squares and calc the values
    // of each index. Each grid square has two triangles:
    //
    //		A - B
    //		| / |
    //		C - D

    WORD *pIndices = new WORD[m_sides * 2 * 3];

    WORD *current = pIndices;
    for (DWORD i = 0; i < m_sides; i++)
    {
        // Triangle #1  ACB
        *(current) = WORD(i * 4);
        *(current + 1) = WORD(i * 4 + 2);
        *(current + 2) = WORD(i * 4 + 1);

        // Triangle #2  BCD
        *(current + 3) = WORD(i * 4 + 1);
        *(current + 4) = WORD(i * 4 + 2);
        *(current + 5) = WORD(i * 4 + 3);
        current += 6;
    }

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * m_sides * 2 * 3;        // each side has 2 triangles
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = pIndices;
    hr = D3DRenderer11::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
    //SAFE_DELETE_ARRAY(pIndices);
    if (FAILED(hr))
        return hr;


    return S_OK;
}

//
// D3DSkyNode11::VRender						- Chapter 16, page 559
//
HRESULT D3DSkyNode11::VRender(Scene *pScene)
{
    HRESULT hr;

    m_vertexShader.SetupRender(pScene, this);
    m_pixelShader.SetupRender(pScene, this);

    // Set vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    D3DRenderer11::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    // Set index buffer
    D3DRenderer11::GetInstance()->GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
    D3DRenderer11::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (DWORD side = 0; side < m_sides; side++)
    {
        // FUTURTE WORK: A good optimization would be to transform the camera's
        // world look vector into local space, and do a dot product. If the
        // result is positive, we shouldn't draw the side since it has to be
        // behind the camera!

        // Sky boxes aren't culled by the normal mechanism

        /***
        // [mrmike] - This was slightly changed post press, look at the lines below this commented out code
        const char *suffix[] = { "_n.jpg", "_e.jpg",  "_s.jpg",  "_w.jpg",  "_u.jpg" };
        name += suffix[side];
        ****/

        std::string name = GetTextureName(side);
        m_pixelShader.SetTexture(name.c_str());
        //D3DRenderer11::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

        D3DRenderer11::GetInstance()->GetDeviceContext()->DrawIndexed(6, side * 6, 0);
    }
    return S_OK;
}


std::string SkyNode::GetTextureName(const int side)
{
    std::string name = m_textureBaseName;
    char *letters[] = { "n", "e", "s", "w", "u" };
    unsigned int index = name.find_first_of("_");
    //GCC_ASSERT(index >= 0 && index < name.length() - 1);
    if (index >= 0 && index < name.length() - 1)
    {
        name[index + 1] = *letters[side];
    }
    return name;
}



