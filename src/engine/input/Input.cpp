//
// Created by Diego S. Seabra on 06/03/22.
//

#include "Input.h"

//
// Callbacks
//

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::stringstream ss;
    ss << "[Key callback] " << "KEY: " << key << " | SCANCODE: " << scancode << " | ACTION: " << action << " | MODS: " << mods;
    Logger::Info(ss.str());

    // TODO: Track state to "know" when we pressed and then released the key

//    if (key == GLFW_KEY_E && action == GLFW_PRESS)
//        activate_airship();
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    std::stringstream ss;
    ss << "[Mouse button callback]" << " | BUTTON: " << button << " | ACTION: " << action << " | MODS: " << mods;
    Logger::Info(ss.str());
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//        popup_menu();
}

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::stringstream ss;
    ss << "[Mouse scroll callback]" << " | OFFSET (X): " << xoffset << " | OFFSET (Y): " << yoffset;
    Logger::Info(ss.str());

    /* NOTE: Temporary */
    if (yoffset == 1)
        Camera::ZoomIn();

    if (yoffset == -1)
        Camera::ZoomOut();
}

// NOTE: In screen coordinates
void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    std::stringstream ss;
    ss << "[Mouse position callback]" << " | POS (X): " << xpos << " | POS (Y): " << ypos;

    glm::vec2 mousePos = glm::vec2(xpos, ypos);
    Camera::ScreenToWorldSpace(mousePos);

    Logger::Info(ss.str());
}

void JoystickCallback(int jid, int event)
{
    std::stringstream ss;
    GLFWgamepadstate state;

    ss << "Joystick event" << event;
    Logger::Info(ss.str());

    // checking for connection/disconnection
    if (event == GLFW_CONNECTED)
    {
        // REVIEW: Do we need a joystick object or just its "id" is enough?
//        auto joy = Joystick();
//        joy.id = jid;
        Input::joysticks.push_back(jid);

        ss << "Joystick " << jid << " connected!";
        Logger::Info(ss.str());
    }
    else if (event == GLFW_DISCONNECTED)
    {
        // REVIEW: This can probably be better implemented!
        for (int i = 0; i < Input::joysticks.size(); ++i)
        {
            if (i == jid)
                Input::joysticks.erase(Input::joysticks.begin() + i);
        }

        ss << "Joystick " << jid << " disconnected!";
        Logger::Info(ss.str());
    }

    // register the inputs of all connected joysticks
    // FIXME: Joystick buttons are not being registered!
//    for (int i = 0; i < Input::joysticks.size(); ++i)
//    {
//        if (glfwJoystickIsGamepad(i))
//        {
//            if (glfwGetGamepadState(i, &state))
//            {
//                if (state.buttons[JoystickButton::DPAD_DOWN])
//                    Logger::Debug("DPAD DOWN");
//
//                if (state.buttons[JoystickButton::DPAD_UP])
//                    Logger::Debug("DPAD UP");
//
//                if (state.buttons[JoystickButton::DPAD_LEFT])
//                    Logger::Debug("DPAD LEFT");
//
//                if (state.buttons[JoystickButton::DPAD_RIGHT])
//                    Logger::Debug("DPAD RIGHT");
//            }
//        }
//    }
}

//
// Implementation
//

void Input::Init()
{
    Logger::Info("Initializing input system");

    SetupGLFWCallbacks();
}

void Input::Update()
{
    glfwPollEvents();

    /* NOTE: Temporary */

    // TODO: (WIP) Get from Input system instead of directly from the GLFW framework ;)
    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_D))
        if (Camera::HasControl())
            Camera::MoveRight();

    // TODO: (WIP) Get from Input system instead of directly from the GLFW framework ;)
    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_A))
        if (Camera::HasControl())
            Camera::MoveLeft();

    // TODO: (WIP) Get from Input system instead of directly from the GLFW framework ;)
    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_W))
        if (Camera::HasControl())
            Camera::MoveUp();

    // TODO: (WIP) Get from Input system instead of directly from the GLFW framework ;)
    if (glfwGetKey(Window::GetWindow(), GLFW_KEY_S))
        if (Camera::HasControl())
            Camera::MoveDown();

    /* NOTE: Temporary END */

    // TODO: (WIP) Get from Input system instead of directly from the GLFW framework ;)
//    if (glfwGetMouseButton(Window::GetWindow(), GLFW_MOUSE_BUTTON_MIDDLE))
//        Logger::Info("Middle mouse click");
}

void Input::Shutdown()
{
    Logger::Info("Shutting down input system");
}

//
// Internal
//

// NOTE: When calling this, make sure we already have the GLFW Window available! Otherwise it won't work!
void Input::SetupGLFWCallbacks()
{
    glfwSetKeyCallback(Window::GetWindow(), KeyCallback);

    glfwSetMouseButtonCallback(Window::GetWindow(), MouseButtonCallback);
    glfwSetScrollCallback(Window::GetWindow(), MouseScrollCallback);
    glfwSetCursorPosCallback(Window::GetWindow(), MousePositionCallback);

    glfwSetJoystickCallback(JoystickCallback);
}
