#pragma once
#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>

class VertexShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> &device, std::wstring shaderPath);
    ID3D11VertexShader *GetShader();
    ID3D10Blob *GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
};

