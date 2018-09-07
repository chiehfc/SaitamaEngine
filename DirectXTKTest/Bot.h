#pragma once
#include "MovingEntity.h"

class Bot : public MovingEntity
{
public:
  Bot();
  virtual ~Bot();

  void Render();
  
  void Update();


};

