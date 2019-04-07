#pragma once
#include "Saitama.h"
#include "GameObjectComponent.h"
#include "GameModel.h"

class RenderComponent : public GameObjectComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;

    void UpdateMatrix();

    std::string GetFilePath() const { return m_filePath; }

    GameModel *GetGameModel() { return &m_gameModel; }

    void SetGameModel(GameModel gameModel) { m_gameModel = gameModel; }

    Matrix GetWorldMatrix() const { return m_worldMatrix; }

    void SetWorldMatrix(Matrix worldMatrix) { m_worldMatrix = worldMatrix; }

    //void Draw(const DirectX::XMMATRIX &viewProjectionMatrix);

private:
    std::string m_filePath;

    GameModel m_gameModel;
    Matrix m_worldMatrix = DirectX::XMMatrixIdentity();
};

