#pragma once

using namespace DirectX::SimpleMath;

class Entity
{
public:
  Entity();
  virtual ~Entity() {}

  virtual void Update();

  virtual void Render() = 0;

  Vector3 GetPos() { return m_position; };

  void SetPos(Vector3 position) { m_position = position; }


private:
  int m_ID;
  static int m_nextValidID;

  void SetID(int val);

protected:
  DirectX::SimpleMath::Vector3 m_position;

  

};

