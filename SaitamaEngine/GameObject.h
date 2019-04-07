#pragma once
#include "GameModel.h"
#include "Saitama.h"

class GameObject
{
    friend class GameObjectFactory;

public:   
    const DirectX::SimpleMath::Vector3 &GetPositionVector() const;
    const DirectX::SimpleMath::Vector3 &GetRotationVector() const;

    void SetPosition(const DirectX::SimpleMath::Vector3 &pos);
    void AdjustPosition(const DirectX::SimpleMath::Vector3 &pos);
    void SetRotation(const DirectX::SimpleMath::Vector3 &rot);
    void AdjustRotation(const DirectX::SimpleMath::Vector3 &rot);
    void SetLookAtPos(const DirectX::SimpleMath::Vector3 &pos);

    const DirectX::SimpleMath::Vector3 &GetForwardVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetLeftVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetRightVector(bool omitY = false);
    const DirectX::SimpleMath::Vector3 &GetBackwardVector(bool omitY = false);



    typedef std::map<ComponentId, StrongGameObjectComponentPtr> GameObjectComponents;
    GameObject() {}
    explicit GameObject(GameObjectId id);
    ~GameObject(void);

    bool Init(tinyxml2::XMLElement* pData);
    void PostInit(void);
    void Destroy(void);

    // accessors
    GameObjectId GetId(void) const { return m_id; }
    GameObjectType GetType(void) const { return m_type; }

    // template function for retrieving components
    template <class ComponentType>
    std::weak_ptr<ComponentType> GetComponent(ComponentId id)
    {
        ActorComponents::iterator findIt = m_components.find(id);
        if (findIt != m_components.end())
        {
            StrongActorComponentPtr pBase(findIt->second);
            shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
            weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  // return the weak pointer
        }
        else
        {
            return weak_ptr<ComponentType>();
        }
    }

    template <class ComponentType>
    std::weak_ptr<ComponentType> GetComponent(const char *name)
    {
        ComponentId id = GameObjectComponent::GetIdFromName(name);
        GameObjectComponents::iterator findIt = m_components.find(id);
        if (findIt != m_components.end())
        {
            StrongGameObjectComponentPtr pBase(findIt->second);
            std::shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
            std::weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
            return pWeakSub;  // return the weak pointer
        }
        else
        {
            return std::weak_ptr<ComponentType>();
        }
    }

    const GameObjectComponents* GetComponents() { return &m_components; }

    void AddComponent(StrongGameObjectComponentPtr pComponent);



protected:
    virtual void UpdateMatrix();
    void UpdateDirectionVectors();

    DirectX::SimpleMath::Vector3 m_posVector;
    DirectX::SimpleMath::Vector3 m_rotVector;

    const DirectX::SimpleMath::Vector3 DEFAULT_FORWARD_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_UP_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_BACKWARD_VECTOR = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_LEFT_VECTOR = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
    const DirectX::SimpleMath::Vector3 DEFAULT_RIGHT_VECTOR = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);

    DirectX::SimpleMath::Vector3 m_vecForward;
    DirectX::SimpleMath::Vector3 m_vecLeft;
    DirectX::SimpleMath::Vector3 m_vecRight;
    DirectX::SimpleMath::Vector3 m_vecBack;
    
    DirectX::SimpleMath::Vector3 m_vecForward_noY;
    DirectX::SimpleMath::Vector3 m_vecLeft_noY;
    DirectX::SimpleMath::Vector3 m_vecRight_noY;
    DirectX::SimpleMath::Vector3 m_vecBack_noY;

    GameObjectId m_id;					// unique id for the actor
    GameObjectComponents m_components;	// all components this actor has
    GameObjectType m_type;

};

