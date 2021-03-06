#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
    DirectX::XMMATRIX wvpMatrix;
    DirectX::XMMATRIX worldMatrix;
};

struct CB_PS_light
{
    DirectX::XMFLOAT3 ambientLightColor;
    float ambientLightStrength;

    DirectX::XMFLOAT4 dynamicLightColor[8];    
    DirectX::XMFLOAT4 dynamicLightPosition[8];
    float dynamicLightStrength;
    float dynamicLightAttenuation_a;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
    float numberOfLights;
};