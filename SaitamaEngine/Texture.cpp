#include "pch.h"
#include "Texture.h"
#include <WICTextureLoader.h>
#include "StringHelper.h"

Texture::Texture(ID3D11Device *device, const Color &color, aiTextureType type)
{
    Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device *device, const Color *colorData, UINT width, UINT height, aiTextureType type)
{
    InitializeColorTexture(device, colorData, width, height, type);
}

Texture::Texture(ID3D11Device *device, const std::string &filePath, aiTextureType type)
{
    m_type = type;
    if (StringHelper::GetFileExtension(filePath) == ".dds")
    {
        HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), m_texture.GetAddressOf(), m_textureView.GetAddressOf());
        if (FAILED(hr))
        {
            this->Initialize1x1ColorTexture(device, Color(0.0f, 1.0f, 0.0f), type);
        }
        return;
    }
    else
    {
        // WIC Texture
        HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), m_texture.GetAddressOf(), m_textureView.GetAddressOf());
        if (FAILED(hr))
        {
            this->Initialize1x1ColorTexture(device, Color(0.0f, 1.0f, 0.0f), type);
        }
    }
}

Texture::Texture(ID3D11Device *device, uint8_t *pData, size_t size, aiTextureType type)
{
    m_type = type;

    // WIC Texture
    HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, m_texture.GetAddressOf(), m_textureView.GetAddressOf());
    DX::ThrowIfFailed(hr);
}

aiTextureType Texture::GetType()
{
    return m_type;
}

ID3D11ShaderResourceView *Texture::GetTextureResourceView()
{
    return m_textureView.Get();
}

ID3D11ShaderResourceView **Texture::GetTextureResourceViewAddress()
{
    return m_textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device *device, const Color &color, aiTextureType type)
{
    InitializeColorTexture(device, &color, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device *device, const Color *colorData, UINT width, UINT height, aiTextureType type)
{
    m_type = type;
    
    CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R32G32B32A32_FLOAT, width, height);
    ID3D11Texture2D *p2DTexture = nullptr;
    D3D11_SUBRESOURCE_DATA initialData{};
    initialData.pSysMem = colorData;
    initialData.SysMemPitch = width * sizeof(Color);
    HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
    DX::ThrowIfFailed(hr);

    m_texture = static_cast<ID3D11Texture2D*>(p2DTexture);
    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
    hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureView.GetAddressOf());
    DX::ThrowIfFailed(hr);
}
