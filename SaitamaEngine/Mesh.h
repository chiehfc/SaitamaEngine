#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"

class Mesh
{
public:
    Mesh(ID3D11Device *deivce, ID3D11DeviceContext *deviceContext, std::vector<Vertex> &vertices, std::vector<DWORD> &indices, std::vector<Texture> &textures, const DirectX::XMMATRIX &matrixTransform);
    Mesh(const Mesh &mesh);
    void Draw();
    const DirectX::XMMATRIX &GetTransformMatrix();

private:

    VertexBuffer<Vertex> m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    ID3D11DeviceContext * m_deviceContext = nullptr;
    std::vector<Texture> m_textures;
    DirectX::XMMATRIX m_transformMatrix;
};

