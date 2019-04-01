#include "pch.h"
#include "Mesh.h"


Mesh::Mesh(ID3D11Device *deivce, ID3D11DeviceContext *deviceContext, std::vector<Vertex> &vertices, std::vector<DWORD> &indices, std::vector<Texture> &textures, const DirectX::XMMATRIX &transformMatrix)
{
    m_deviceContext = deviceContext;
    m_textures = textures;
    m_transformMatrix = transformMatrix;

    HRESULT hr = m_vertexBuffer.Initialize(deivce, vertices.data(), vertices.size());
    DX::ThrowIfFailed(hr);

    hr = m_indexBuffer.Initialize(deivce, indices.data(), indices.size());
    DX::ThrowIfFailed(hr);
}

Mesh::Mesh(const Mesh &mesh)
{
    m_vertexBuffer = mesh.m_vertexBuffer;
    m_indexBuffer = mesh.m_indexBuffer;
    m_deviceContext = mesh.m_deviceContext;
    m_textures = mesh.m_textures;
    m_transformMatrix = mesh.m_transformMatrix;
}

void Mesh::Draw()
{
    UINT offset = 0;
    
    for (UINT i = 0; i < m_textures.size(); i++)
    {
        if (m_textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
        {
            m_deviceContext->PSSetShaderResources(0, 1, m_textures[i].GetTextureResourceViewAddress());
            break;
        }
    }

    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &offset);
    m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_deviceContext->DrawIndexed(m_indexBuffer.IndexCount(), 0, 0);
}

const DirectX::XMMATRIX &Mesh::GetTransformMatrix()
{
    return m_transformMatrix;
}