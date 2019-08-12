#pragma once

#include "tinyxml2/tinyxml2.h"
#include "templates.h"
#include <memory>
#include <iostream>

enum RenderPass
{
    RenderPass_0,
    //RenderPass_Static = RenderPass_0,
    RenderPass_Actor = RenderPass_0,
    RenderPass_Sky,
    RenderPass_NotRendered,
    RenderPass_Last
};

using namespace std;
using namespace DirectX::SimpleMath;
#include "FastDelegate.h"
using fastdelegate::MakeDelegate;

extern Vector3 g_Right;
extern Vector3 g_Up;
extern Vector3 g_Forward;


extern Vector4 g_Up4;
extern Vector4 g_Right4;
extern Vector4 g_Forward4;

class GameObject;
class GameObjectComponent;

typedef unsigned int GameObjectId;
typedef unsigned int ComponentId;
typedef std::string GameObjectType;

const GameObjectId INVALID_GAMEOBJECT_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef std::shared_ptr<GameObject> StrongGameObjectPtr;
typedef std::weak_ptr<GameObject> WeakGameObjectPtr;
typedef std::shared_ptr<GameObjectComponent> StrongGameObjectComponentPtr;
typedef std::weak_ptr<GameObjectComponent> WeakGameObjectComponentPtr;