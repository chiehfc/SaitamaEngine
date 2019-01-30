#pragma once
#include "KeyboardEvent.h"
#include <queue>

class KeyboardClass
{
public:
  KeyboardClass();
  ~KeyboardClass();



private:
  bool m_autoRepeatKeys = false;
  bool m_autoRepeatChars = false;
  bool m_keyStates[256];
  std::queue<KeyboardEvent> m_keyBuffer;
  std::queue<unsigned char> m_charBuffer;
};

