#include "pch.h"
#include "KeyboardEvent.h"


KeyboardEvent::KeyboardEvent()
  :
  m_type(EventType::Invalid),
  m_key(0u)
{
}


KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
  :
  m_type(type),
  m_key(key)
{

}

KeyboardEvent::~KeyboardEvent()
{
}

bool KeyboardEvent::IsPress() const
{
  return this->m_type == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
  return this->m_type == EventType::Release;
}

bool KeyboardEvent::IsValid() const
{
  return this->m_type == EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
  return this->m_key;
}
