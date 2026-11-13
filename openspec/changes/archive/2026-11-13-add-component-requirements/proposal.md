## Why

A component can depend on another one on the same entity (a camera controller and a directional light both need a transform), but nothing records that. The Inspector lets the user remove a transform from under them: the light silently stops lighting the scene and logs an error every frame, and the camera controller keeps a dangling pointer that crashes on the next key press.

## What Changes

- A component type declares the component types it requires on the same entity.
- Removing a component that another component on the entity requires is refused: the Inspector shows **Remove Component** disabled, with a tooltip naming what requires it.
- Adding a component also adds any required components the entity lacks, recursively, as one undo step.
- Opening a scene checks each loaded entity: a missing required component is added with default values, recursively, with a logged warning naming the entity, the component that requires it and the added type, instead of letting the entity reach systems that assume the requirement.
- A directional light or mesh with no transform is an assert in the renderer again, and the camera controller's silent null-transform guard is removed.
- Camera, camera controller, directional light and mesh declare that they require a transform, so a transform can be removed only from an entity that has none of them.

## Capabilities

### New Capabilities

- `editor/scene-files`: what happens to invalid component combinations when a scene is opened.

### Modified Capabilities

- `editor/inspector`: Remove Component is refused for a required component; Add Component brings its requirements along.

## Impact

- `Engine/Core/src/Scene/` (Component declares requirements; a shared requirement query), `Engine/Simulation/src/Scene/WorldSubsystem.cpp` (load check), `Engine/Graphics/src/Renderer/` (light assert, camera controller guard, requirement declarations).
- `Engine/Core/src/Core/Assert.h`: `AX_ASSERT` takes a `std::format` message with arguments, formatted only on failure, so an assert can name the type it failed on. Existing asserts are unchanged; the one built from a runtime string in `GLShaderLoader.cpp` passes it as an argument instead.
- `EditorCore/src/Commands/` (Add and Remove component commands), `Editor/src/UI/Panels/InspectorPanel.cpp` (disabled menu item and tooltip).
- `Engine/Core/src/Core/Log.h`: a `Warning` level, with a warning count beside the error count.
- Tests in `Tests/Logic` (requirement query, commands) and `Tests/Runtime` (scene load). The test harness registers test-only component types through its own reflection registrar and mounts `tests://` at `Tests/Assets`, which holds the test scenes.

## Out of scope

- Enforcement inside `Entity` itself: code calling `CreateComponent` or `AddComponent` directly is not checked.
- Removing a component together with everything that requires it (cascade removal).
- Requirements across entities (a parent, a scene-wide singleton) or "one of these types" requirements.
- Showing a component's requirements in the Add Component menu.
- Writing the repaired scene back to disk on open; the added defaults are saved only when the user saves.
