#include "pch.h"
#include "Shaders.h"
#include "SceneNode.h"
#include "D3DRenderer11.h"

bool VertexShader::Initialize(ID3D11Device *device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC *layoutDesc, UINT numElements)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = device->CreateInputLayout(layoutDesc, numElements, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());
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

ID3D11InputLayout *VertexShader::GetInputLayout()
{
    return m_inputLayout.Get();
}

HRESULT VertexShader::SetupRender(Scene *pScene, SceneNode *pNode)
{
    HRESULT hr = S_OK;

    D3DRenderer11::GetInstance()->GetDeviceContext()->VSSetShader(m_shader.Get(), NULL, 0);
    D3DRenderer11::GetInstance()->GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());
    D3DRenderer11::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return hr;
}


bool PixelShader::Initialize(ID3D11Device *device, std::wstring shaderPath)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
    DX::ThrowIfFailed(hr);

    hr = device->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
    DX::ThrowIfFailed(hr);

    return true;
}

ID3D11PixelShader *PixelShader::GetShader()
{
    return m_shader.Get();
}

ID3D10Blob *PixelShader::GetBuffer()
{
    return m_shaderBuffer.Get();
}

HRESULT PixelShader::SetupRender(Scene *pScene, SceneNode *pNode)
{
    HRESULT hr = S_OK;
    D3DRenderer11::GetInstance()->GetDeviceContext()->PSSetShader(m_shader.Get(), NULL, 0);
    
    return hr;

}