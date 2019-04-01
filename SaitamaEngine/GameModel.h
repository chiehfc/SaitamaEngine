#pragma once
#include "Mesh.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class GameModel
{
public:
    bool Initialize(const std::string &filePath, ID3D11Device *device, ID3D11DeviceContext *deviceContext, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader);
    void Draw(const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewProjectionMatrix);
    
private:
    bool LoadModel(const std::string &filePath);
    void ProcessNode(aiNode *node, const aiScene *scene, const DirectX::XMMATRIX &parentTransformMatrix);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, const DirectX::XMMATRIX &transformMatrix);
    TextureStorageType DetermineTextureStorageType(const aiScene *pScene, aiMaterial *pMat, unsigned int index, aiTextureType type);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *pMaterial, aiTextureType textureType, const aiScene *pScene);
    int GetTextureIndex(aiString *pStr);

    ID3D11Device *m_device = nullptr;
    ID3D11DeviceContext *m_deviceContext = nullptr;
    ConstantBuffer<CB_VS_vertexshader> *m_cb_vs_vertexshader = nullptr;
    
    std::vector<Mesh> m_meshes;

    std::string m_directory = "";
};

