#include "pch.h"
#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> &device, std::wstring shaderPath)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
    DX::ThrowIfFailed(hr);

    return true;
}

ID3D11VertexShader *VertexShader::GetShader()
{
    return m_shader.Get();
}

ID3D10Blob *VertexShader::GetBuffer()
{
    return m_shaderBuffer.Get();
}