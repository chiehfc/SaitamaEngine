#pragma once
#include "Saitama.h"
#include "GameObjectComponent.h"

class RenderComponent : public GameObjectComponent
{
public:
    static const char* g_Name;
    virtual const char* VGetName() const { return g_Name; }

    virtual bool VInit(tinyxml2::XMLElement *pData) override;

    std::string GetFilePath() const { return m_filePath; }

    void Draw(const DirectX::XMMATRIX &viewProjectionMatrix);

private:
    std::string m_filePath;

    /*GameModel m_gameModel;
    Matrix m_worldMatrix = DirectX::XMMatrixIdentity();*/
};

