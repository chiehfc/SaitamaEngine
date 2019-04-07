#pragma once

#include "tinyxml2/tinyxml2.h"
#include "templates.h"
#include <memory>

using namespace std;

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