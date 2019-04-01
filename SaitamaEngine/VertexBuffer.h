#pragma once

#include <d3d11.h>
#include <wrl/client.h>

template<class T> 
class VertexBuffer
{
public:
    VertexBuffer() {}

    VertexBuffer(const VertexBuffer<T> &vertexBuffer)
    {
        m_buffer = vertexBuffer.m_buffer;
        m_vertexCount = vertexBuffer.m_vertexCount;
        m_stride = vertexBuffer.m_stride;
    }

    VertexBuffer<T> &operator=(const VertexBuffer<T> &a)
    {
        m_buffer = a.m_buffer;
        m_vertexCount = a.m_vertexCount;
        m_stride = a.m_stride;
        return *this;
    }

    ID3D11Buffer *Get() const
    {
        return m_buffer.Get();
    }

    ID3D11Buffer * const* GetAddressOf() const
    {
        return m_buffer.GetAddressOf();
    }

    UINT VertexCount() const
    {
        return m_vertexCount;
    }

    const UINT *Stride() const
    { 
        return m_stride;
    }

    const UINT *StridePtr() const
    {
        return &m_stride;
    }

    HRESULT Initialize(ID3D11Device *device, T *data, UINT vertexCount)
    {
        if (m_buffer.Get() != nullptr)
        {
            m_buffer.Reset();
        }

        m_vertexCount = vertexCount;

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = m_stride * vertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = data;

        HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_buffer.GetAddressOf());
        DX::ThrowIfFailed(hr);

        return hr;
    }

private:

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    UINT m_stride = sizeof(T);
    UINT m_vertexCount = 0;
};