#include "pch.h"
#include "KeyboardClass.h"


KeyboardClass::KeyboardClass()
{
  for (int i = 0; i < 256; i++)
  {
    this->m_keyStates[i] = false;
  }
}


KeyboardClass::~KeyboardClass()
{
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
  return this->m_keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
  return this->m_keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
  return this->m_charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
  if (this->m_keyBuffer.empty())
  {
    return KeyboardEvent();
  }
  else
  {
    KeyboardEvent e = this->m_keyBuffer.front();
    this->m_keyBuffer.pop();
    return e;
  }
}

unsigned char KeyboardClass::ReadChar()
{
  if (this->m_charBuffer.empty())
  {
    return 0u;
  }
  else
  {
    unsigned char c = this->m_charBuffer.front();
    this->m_charBuffer.pop();
    return c;
  }
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
  this->m_keyStates[key] = true;
  this->m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
  this->m_keyStates[key] = false;
  this->m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
  this->m_charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
  this->m_autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
  this->m_autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
  this->m_autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
  this->m_autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
  return this->m_autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
  return this->m_autoRepeatChars;
}
