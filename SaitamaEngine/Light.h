#pragma once
#include "RenderableGameObject.h"

class Light : public RenderableGameObject
{
public:
    bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertextshader);

    DirectX::XMFLOAT4 lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    float lightStrength = 1.0f;
    float attenuation_a = 1.0f;
    float attenuation_b = 0.1f;
    float attenuation_c = 0.1f;
};

