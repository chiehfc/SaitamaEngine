#pragma once

#include "tinyxml2/tinyxml2.h"
#include "templates.h"
#include <memory>

typedef 

class GameObject;
class GameObjectComponent;

typedef unsigned int GameObjectId;
typedef unsigned int ComponentId;
typedef std::string GameObjectType;

const GameObjectId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef std::shared_ptr<GameObject> StrongActorPtr;
typedef std::weak_ptr<GameObject> WeakActorPtr;
typedef std::shared_ptr<GameObjectComponent> StrongGameObjectComponentPtr;
typedef std::weak_ptr<GameObjectComponent> WeakGameObjectComponentPtr;