# Axiom

A from-scratch C++ game engine — OpenGL forward renderer, reflection-driven ECS, and a custom scene editor, built to learn low-level engine architecture from first principles.

Currently a single-threaded OpenGL forward renderer with a component-based ECS. Physics and multithreading are next on the roadmap.

## Highlights

- **Forward renderer** — depth pre-pass with instancing, Blinn-Phong lighting, shader variants (preprocessor defines resolved per-material), multi-view pipeline with shadow mapping.
- **Platform abstraction** — a `GraphicsDevice` factory interface keeps all OpenGL calls out of the renderer core, leaving room for a future Vulkan backend without a rewrite.
- **Entity-Component System** — `Scene` → `Entity` → typed components, with a closure-based reflection system driving automatic serialization and Inspector UI.
- **Data-driven scenes & materials** — JSON scene files and `.mat` materials resolved through a virtual filesystem (`engine://`, `project://`) and typed resource loaders with caching.
- **Editor** — a Godot-inspired scene editor (Outliner, Inspector, Viewport) with full undo/redo built on reflected property editing.

## Structure

- `Engine/` — static library: renderer, ECS, resources, reflection, platform layer
- `Editor/` — ImGui-based scene editor
- `Game/` — sandbox application exercising the engine

## Building

Requires CMake 3.20+ and Visual Studio 2022 (MSVC). Dependencies (GLFW, ImGui, GLAD, STB, nlohmann/json) are fetched automatically via CMake FetchContent.

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
