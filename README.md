# Elixir Game Engine

Game Engine being created in C++ from scratch using Vulkan, GLFW, ImGUI and FMOD

## Roadmap

- [ ] Renderer System (Vulkan)
  - [X] 2D object (texture)
  - [X] 3D object (mesh)
  - [ ] Batch 2D rendering
  - [ ] 2D Atlas support
- [ ] Audio Engine (FMOD)
  - [X] 3D audio
  - [X] 2D audio
  - [X] Bank support
  - [X] Event support
  - [X] External Audio File (from specific folder)
  - [ ] State machine (fade, effects, new features, etc)
- [ ] Input System
  - [ ] Keyboard
  - [ ] Mouse
  - [ ] Gamepad/Joystick
- [ ] Scripting
  - [X] Basic lua integration
  - [ ] Run lua code at runtime
- [ ] Tools
  - [ ] File logging (debugging info)
  - [ ] IMGUI
- [ ] Future
  - [ ] Fallback to DirectX/OpenGL on non-supported Vulkan hardware