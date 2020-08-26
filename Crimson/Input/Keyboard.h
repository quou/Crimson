#pragma once

#include <map>

#include <SDL.h>

class Keyboard {
private:
   std::map<SDL_Scancode, bool> m_pressedKeys;
   std::map<unsigned int, bool> m_releasedKeys;
   std::map<unsigned int, bool> m_heldKeys;
public:
   void Update() {
      m_pressedKeys.clear();
      m_releasedKeys.clear();
   }

   /* FOR INTERNAL USE ONLY */
   void KeyDownEvent(const SDL_Event& e) {
      m_pressedKeys[e.key.keysym.scancode] = true;
      m_heldKeys[e.key.keysym.scancode] = true;
   }

   /* FOR INTERNAL USE ONLY */
   void KeyUpEvent(const SDL_Event& e) {
      m_releasedKeys[e.key.keysym.scancode] = true;
      m_heldKeys[e.key.keysym.scancode] = false;
   }

   bool WasKeyPressed(SDL_Scancode key) {
      return m_pressedKeys[key];
   }

   bool WasKeyReleased(SDL_Scancode key) {
      return m_releasedKeys[key];
   }

   bool IsKeyHeld(SDL_Scancode key) {
      return m_heldKeys[key];
   }
};
