# Elixir Game Engine

Game Engine being created in C++ from scratch using Vulkan, GLFW, ImGUI and FMOD

## Roadmap

- Renderer System (Vulkan)
  - 2D object (texture) ✅
  - 3D object (mesh) ✅
  - Batch 2D rendering
  - 2D Atlas support
  - Font/Text (truetype fonts)
- Audio Engine (FMOD)
  - 3D audio ✅
  - 2D audio ✅
  - Bank support ✅
  - Event support ✅
  - External Audio File (from specific folder) ✅
  - State machine (fade, effects, new features, etc)
- Input System
  - Keyboard
  - Mouse
  - Gamepad/Joystick
- Scripting
  - Basic lua integration (script reading) ✅
  - Run lua code at runtime
- Tools
  - File logging (debugging info) ✅
  - IMGUI
- Future
  - Fallback to DirectX/OpenGL on non-supported Vulkan hardware