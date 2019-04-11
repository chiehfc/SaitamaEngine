#pragma once
#include "Saitama.h"
#include "GameObjectComponent.h"
#include "GameModel.h"
#include "SceneNode.h"

class RenderComponent : public GameObjectComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;
    virtual void VPostInit(void) override {}
    virtual shared_ptr<SceneNode> VCreateSceneNode(void);  


    void UpdateMatrix();

    std::string GetFilePath() const { return m_filePath; }

    GameModel *GetGameModel() { return &m_gameModel; }

    void SetGameModel(GameModel gameModel) { m_gameModel = gameModel; }

    Matrix GetWorldMatrix() const { return m_worldMatrix; }

    void SetWorldMatrix(Matrix worldMatrix) { m_worldMatrix = worldMatrix; }

    //void Draw(const DirectX::XMMATRIX &viewProjectionMatrix);
    virtual shared_ptr<SceneNode> VGetSceneNode(void);

private:
    
    shared_ptr<SceneNode> m_pSceneNode;

    std::string m_filePath;

    GameModel m_gameModel;
    Matrix m_worldMatrix = DirectX::XMMatrixIdentity();
};

