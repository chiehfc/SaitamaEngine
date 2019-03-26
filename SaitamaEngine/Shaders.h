#pragma once
#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>

class VertexShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> &device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC *layoutDesc, UINT numElements);
    ID3D11VertexShader *GetShader();
    ID3D10Blob *GetBuffer();

    ID3D11InputLayout *GetInputLayout();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

class PixelShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> &device, std::wstring shaderPath);
    ID3D11PixelShader *GetShader();
    ID3D10Blob *GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
};