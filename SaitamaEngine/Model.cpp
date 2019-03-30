#include "pch.h"
#include "Model.h"

bool Model::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11ShaderResourceView *texture, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader)
{
    m_device = device;
    m_deviceContext = deviceContext;
    m_texture = texture;
    m_cb_vs_vertexshader = &cb_vs_vertexshader;

    // Vertex Buffer
    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 0.0f),
        Vertex(0.5f,  0.5f, 0.0f, 1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),

        Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 1.0f, 0.0f, 0.0f),
        Vertex(0.5f,  0.5f, 1.0f, 1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 1.0f, 1.0f, 1.0f)
    };

    HRESULT hr = m_vertexBuffer.Initialize(m_device, v, ARRAYSIZE(v));
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

    hr = m_indexBuffer.Initialize(m_device, indices, ARRAYSIZE(indices));
    DX::ThrowIfFailed(hr);

    UpdateWorldMatrix();
    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView *texture)
{
    m_texture = texture;
    
}

void Model::Draw(const DirectX::XMMATRIX &viewProjectionMatrix)
{
    m_cb_vs_vertexshader->data.mat = m_worldMatrix * viewProjectionMatrix;
    // from row major(DirectXMath library) to column major(HLSL)
    m_cb_vs_vertexshader->data.mat = DirectX::XMMatrixTranspose(m_cb_vs_vertexshader->data.mat);
    if (!m_cb_vs_vertexshader->ApplyChanges())
    {
        return;
    }
    m_deviceContext->VSSetConstantBuffers(0, 1, m_cb_vs_vertexshader->GetAddressOf());

    //m_cb_vs_vertexshader->data.alpha = alpha;
    //if (!m_cb_ps_pixelshader.ApplyChanges())
    //{
    //    return;
    //}
    //m_d3dContext->PSSetConstantBuffers(0, 1, m_cb_ps_pixelshader.GetAddressOf());
    UINT offset = 0;
    m_deviceContext->PSSetShaderResources(0, 1, &m_texture);
    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &offset);
    m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_deviceContext->DrawIndexed(m_indexBuffer.BufferSize(), 0, 0);
}

void Model::UpdateWorldMatrix()
{
    m_worldMatrix = DirectX::XMMatrixIdentity();
}