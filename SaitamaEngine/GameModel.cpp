#include "pch.h"
#include "GameModel.h"
#include "StringHelper.h"

using namespace DirectX::SimpleMath;

bool GameModel::Initialize(const std::string &filePath, ID3D11Device *device, ID3D11DeviceContext *deviceContext, ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader)
{
    m_device = device;
    m_deviceContext = deviceContext;
    m_cb_vs_vertexshader = &cb_vs_vertexshader;

    LoadModel(filePath);

    return true;
}

void GameModel::Draw(const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewProjectionMatrix)
{
    

    //m_cb_vs_vertexshader->data.alpha = alpha;
    //if (!m_cb_ps_pixelshader.ApplyChanges())
    //{
    //    return;
    //}
    //m_d3dContext->PSSetConstantBuffers(0, 1, m_cb_ps_pixelshader.GetAddressOf());
    
    for (UINT i = 0; i < m_meshes.size(); i++)
    {
        m_cb_vs_vertexshader->data.mat = m_meshes[i].GetTransformMatrix() * worldMatrix * viewProjectionMatrix;
        // from row major(DirectXMath library) to column major(HLSL)
        m_cb_vs_vertexshader->data.mat = DirectX::XMMatrixTranspose(m_cb_vs_vertexshader->data.mat);
        if (!m_cb_vs_vertexshader->ApplyChanges())
        {
            return;
        }
        m_deviceContext->VSSetConstantBuffers(0, 1, m_cb_vs_vertexshader->GetAddressOf());
        m_meshes[i].Draw();
    }
}

bool GameModel::LoadModel(const std::string &filePath)
{
    m_directory = StringHelper::GetDirectoryFromPath(filePath);

    Assimp::Importer importer;
    const aiScene *pScene = importer.ReadFile(filePath,
                                            aiProcess_Triangulate |
                                            aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
    return true;
}

void GameModel::ProcessNode(aiNode *node, const aiScene *scene, const DirectX::XMMATRIX &parentTransformMatrix)
{
    DirectX::XMMATRIX nodeTransformMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;
    
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene, nodeTransformMatrix));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
    }
}

Mesh GameModel::ProcessMesh(aiMesh *mesh, const aiScene *scene, const DirectX::XMMATRIX &transformMatrix)
{
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
            vertex.texCoord.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(vertex);
    }
        
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::vector<Texture> textures;
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

    return Mesh(m_device, m_deviceContext, vertices, indices, textures, transformMatrix);
}

TextureStorageType GameModel::DetermineTextureStorageType(const aiScene *pScene, aiMaterial *pMat, unsigned int index, aiTextureType textureType)
{
    if (pMat->GetTextureCount(textureType) == 0)
    {
        return TextureStorageType::None;
    }

    aiString path;
    pMat->GetTexture(textureType, index, &path);
    std::string texturePath = path.C_Str();

    if (texturePath[0] == '*')
    {
        if (pScene->mTextures[0]->mHeight == 0)
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }

    if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }

    if (texturePath.find('.') != std::string::npos)
    {
        return TextureStorageType::Disk;
    }

    return TextureStorageType::None;
}

std::vector<Texture> GameModel::LoadMaterialTextures(aiMaterial *pMaterial, aiTextureType textureType, const aiScene *pScene)
{
    std::vector<Texture> materialTextures;
    TextureStorageType storeType = TextureStorageType::Invalid;
    unsigned int textureCount = pMaterial->GetTextureCount(textureType);

    if (textureCount == 0)
    {
        storeType = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);
        switch (textureType)
        {
        case aiTextureType_DIFFUSE:
            pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
            if (aiColor.IsBlack()) // If color = black, just use grey
            {
                materialTextures.push_back(Texture(m_device, Color(0.3f, 0.3f, 0.3f), textureType));
                return materialTextures;
            }
            materialTextures.push_back(Texture(m_device, Color(aiColor.r, aiColor.g, aiColor.b), textureType));
            return materialTextures;
        }
    }
    else
    {
        for (UINT i = 0; i < textureCount; i++)
        {
            aiString path;
            pMaterial->GetTexture(textureType, i, &path);
            TextureStorageType storeType = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
            switch (storeType)
            {
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                int index = GetTextureIndex(&path);
                Texture embeddedIndexedTexture(m_device,
                    reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
                    pScene->mTextures[index]->mWidth,
                    textureType);
                materialTextures.push_back(embeddedIndexedTexture);
                break;
            }
            case TextureStorageType::EmbeddedCompressed:
            {
                const aiTexture *pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                Texture embeddedTexture(m_device,
                                        reinterpret_cast<uint8_t*>(pTexture->pcData),
                                        pTexture->mWidth,
                                        textureType);
                materialTextures.push_back(embeddedTexture);
                break;
            }
            case TextureStorageType::Disk:
                std::string filename = m_directory + '\\' + path.C_Str();
                Texture diskTexture(m_device, filename, textureType);
                materialTextures.push_back(diskTexture);
                break;
            }
        }
    }

    if (materialTextures.size() == 0)
    {
        materialTextures.push_back(Texture(m_device, Color(1.0f, 0.0f, 0.0f), textureType));
    }
    return materialTextures;
}

int GameModel::GetTextureIndex(aiString *pStr)
{
    assert(pStr->length >= 2);
    return atoi(&pStr->C_Str()[1]);
}