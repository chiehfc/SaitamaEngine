#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class IndexBuffer
{
public:
    IndexBuffer() {}

    ID3D11Buffer *Get() const
    {
        return m_buffer.Get();
    }

    ID3D11Buffer * const* GetAddressOf() const
    {
        return m_buffer.GetAddressOf();
    }

    UINT IndexCount() const
    {
        return m_indexCount;
    }

    HRESULT Initialize(ID3D11Device *device, DWORD *data, UINT indexCount)
    {
        if (m_buffer.Get() != nullptr)
        {
            m_buffer.Reset();
        }

        m_indexCount = indexCount;

        // Index Buffer
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA indexBufferData;
        ZeroMemory(&indexBufferData, sizeof(indexBufferData));
        indexBufferData.pSysMem = data;

        HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_buffer.GetAddressOf());
        DX::ThrowIfFailed(hr);

        return hr;
    }

private:
    IndexBuffer(const IndexBuffer &rhs);

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    UINT m_indexCount = 0;
};