#include "pch.h"
#include "Light.h"


bool Light::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader)
{
    if (!m_gameModel.Initialize("Data\\Models\\light.fbx", device, deviceContext, cb_vs_vertexshader))
    {
        return false;
    }

    SetPosition(Vector3(0.0f, 10.0f, 0.0f));
    SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    UpdateMatrix();
    return true;
}