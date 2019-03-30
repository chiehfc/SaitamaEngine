#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Model
{
public:
    bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ID3D11ShaderResourceView *texture, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader);
    void SetTexture(ID3D11ShaderResourceView *texture);
    void Draw(const DirectX::XMMATRIX &viewProjectionMatrix);
private:
    void UpdateWorldMatrix();

    ID3D11Device *m_device = nullptr;
    ID3D11DeviceContext *m_deviceContext = nullptr;
    ConstantBuffer<CB_VS_vertexshader> *m_cb_vs_vertexshader = nullptr;
    ID3D11ShaderResourceView *m_texture = nullptr;

    VertexBuffer<Vertex> m_vertexBuffer;
    IndexBuffer m_indexBuffer;

    DirectX::XMMATRIX m_worldMatrix = DirectX::XMMatrixIdentity();
};

