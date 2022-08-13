# Elixir Game Engine

Game Engine being created in C++ from scratch using Vulkan, GLFW, ImGUI and FMOD

## Engine Development (Notion)

https://diegosseabra.notion.site/Engine-Development-67f7d4d012bb47f1a21879e2b898bd24

## Roadmap

- Renderer System (Vulkan)
  - 2D object (texture) ✅
  - 3D object (mesh) ✅
  - Batch 2D rendering
  - 2D Atlas support
  - Font/Text (truetype fonts)
- Editor
  - Simple object inspector
  - Render the game at a texture/viewport
  - Play/Pause game in the editor
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
  - IMGUI ✅
- Future
  - Fallback to DirectX/OpenGL on non-supported Vulkan hardware