//
// Created by Diego S. Seabra on 06/03/22.
//

#ifndef VULKAN_ENGINE_INPUT_H
#define VULKAN_ENGINE_INPUT_H

#include <cstdint>

#define MAX_KEYBOARD_KEYS 256
#define MAX_MOUSE_BUTTONS 3 // REVIEW: What is the best value for this?
#define MAX_JOYSTICK_BUTTONS 16

struct KeyboardState
{
    uint32_t keys[MAX_KEYBOARD_KEYS]; // REVIEW: Could this be uint8_t?
};

struct MouseState
{
    float x;
    float y;
    uint32_t buttons[MAX_MOUSE_BUTTONS];
};

struct JoystickState
{
    uint32_t buttons[MAX_JOYSTICK_BUTTONS];
};

struct InputState
{
    KeyboardState keyboard;
    MouseState mouse;
    JoystickState joystick;
};

struct CInputImpl
{
    CInputImpl();
    ~CInputImpl();

    InputState mInputState;
//    KeyboardState mKeyboardState;
//    MouseState mMouseState;
//    JoystickState mJoystickState;
};

class CInput
{
public:
    static void Init();
    static void Update();
    static void Shutdown();
};


#endif //VULKAN_ENGINE_INPUT_H
