#pragma once
#include "Saitama.h"
#include "GameObjectComponent.h"
#include "GameModel.h"
#include "SceneNode.h"
#include "Lights.h"

class BaseRenderComponent : public GameObjectComponent
{
public:
    virtual bool VInit(tinyxml2::XMLElement *pData) override;
    virtual void VPostInit(void) override;

    virtual shared_ptr<SceneNode> VGetSceneNode(void);

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) { return true; }
    shared_ptr<SceneNode> m_pSceneNode;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) = 0;  // factory method to create the appropriate scene node

private:
    //virtual shared_ptr<SceneNode> VGetSceneNode(void); TODO: After event system, put this back to private and use delegate to add SceneNode to Scene
};


class ModelRenderComponent : public BaseRenderComponent
{
public:    
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

    std::string GetFilePath() const { return m_filePath; }
    Matrix GetWorldMatrix() const { return m_worldMatrix; }
    void SetWorldMatrix(Matrix worldMatrix) { m_worldMatrix = worldMatrix; }

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) override;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;

private:
    std::string m_filePath;
    GameModel m_gameModel;
    Matrix m_worldMatrix = DirectX::XMMatrixIdentity();
};

class LightRenderComponent : public BaseRenderComponent
{
public:
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) override;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

private:
    LightProperties m_lightProps;
};

class GridRenderComponent : public BaseRenderComponent
{
public:
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

    const char* GetTextureResource() { return m_textureResource.c_str(); }
    const int GetDivision() { return m_squares; }

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) override;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

private:
    std::string m_textureResource = "";
    int m_squares = 0;
};


class SphereRenderComponent : public BaseRenderComponent
{
public:
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

    //const char* GetTextureResource() { return m_textureResource.c_str(); }
    //const int GetDivision() { return m_squares; }

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) override;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

private:
    //std::string m_textureResource = "";
    //int m_squares = 0;
};

class SkyRenderComponent : public BaseRenderComponent
{
public:
    static const char *g_Name;
    virtual const char *VGetName() const { return g_Name; }

    //const char* GetTextureResource() { return m_textureResource.c_str(); }
    //const int GetDivision() { return m_squares; }

protected:
    virtual bool VDelegateInit(tinyxml2::XMLElement *pData) override;
    virtual shared_ptr<SceneNode> VCreateSceneNode(void) override;  // factory method to create the appropriate scene node

private:
    std::string m_textureResource[6];
};