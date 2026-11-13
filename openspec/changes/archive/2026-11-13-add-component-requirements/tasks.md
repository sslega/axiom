## 1. Declare requirements and query them

- [x] 1.1 Add the virtual requirements getter to `Component` (empty by default); `CameraComponent`, `CameraController`, `DirectionalLightComponent` and `MeshComponent` return the transform type. Verify: builds, the Editor runs unchanged.
- [x] 1.2 Add the requirement query in `Engine/Core/src/Scene/`: which components on an entity require a type, and which required types a component is missing. Verify: logic tests with test components (one requirement, several requirers, a requirement already present) pass with `AxiomTests -ts=logic`.

## 2. Refuse removing a required component

- [x] 2.1 Inspector: Remove Component is disabled for a required component, with a `Required by …` tooltip listing every requirer; choosing it does nothing. Verify: on the light entity in `main.scene.json`, the transform's Remove is disabled with the tooltip; removing the light enables it.
- [x] 2.2 The refusal lives in the Inspector and is driven by the requirement query, which the 1.2 logic tests cover; the panel itself is checked visually (6.2).

## 3. Add brings its requirements

- [x] 3.1 Let `AX_ASSERT` take format arguments (formatted only on failure). Add `TypeDescriptor::Create()` (asserts the type has a factory, naming it) and route existing factory calls through it. `AddComponentCommand` adds missing requirements first, recursively with a visited set, through it; asserts if a requirement isn't reflected. Verify: adding a camera controller to an empty entity gives it a transform and a gizmo.
- [x] 3.2 Undo removes every component the add created, in reverse order, keeping the objects for Redo. Verify: runtime tests with the harness's test components (the command creates through the real reflection registry) for add-with-chain, add-with-requirement-present, undo and redo (same objects).

## 4. Check requirements on scene open

- [x] 4.1 After all of an entity's components are loaded and before it joins the scene, add each missing required component with default values, logging one warning each, including the requirements of components added this way. Verify: runtime tests (test components registered by the harness, test scenes under `tests://`) open a light with no transform and find a transform added, the light's saved values kept and one warning; and open a two-level missing chain and find both added with two warnings.
- [x] 4.2 Runtime check that the committed `main.scene.json` opens with no requirement warning. Verify: `AxiomTests -ts=runtime`.

## 5. Restore the asserts

- [x] 5.1 Replace the light's error-and-no-light fallback in `RenderSubsystem` with an assert, and the mesh's identity-matrix fallback for a missing transform with an assert. Remove the camera controller's null-transform guard: the requirement guarantees the transform, and components don't re-check it. Verify: builds; the Editor runs with `main.scene.json` and the light still lights the scene.
- [x] 5.2 Update the usage comments that mention the old gaps (`RemoveComponentCommand` dangling-pointer trap, `Component` on declaring requirements). Verify: each claim matches the code.

## 6. Verification

- [x] 6.1 Automated: clean build, `build/Tests/Debug/AxiomTests.exe` exits 0 (logic and runtime suites).
- [x] 6.2 Visual: select the light entity, open the transform's menu: Remove is greyed out and the tooltip reads `Required by Directional Light Component`. A mesh entity's transform is greyed out the same way, with `Required by Mesh Component`. Remove the light, then the transform: both go, and Undo twice brings both back in order.
- [x] 6.3 Visual: create an entity, add a camera controller: a transform appears with it and the gizmo shows. Undo once: both are gone. Redo: both return.
- [x] 6.4 Visual (the case a naive version gets wrong): on an entity with a mesh and a moved transform, add a directional light. The transform keeps its position and is not replaced by a default one; Undo removes only the light.
- [x] 6.5 Visual: open a hand-edited copy of the scene with a light whose entity has no transform. The Editor keeps running, the console shows one warning naming the entity and both types, the entity has a transform at the origin, and the light keeps its colour and intensity and lights the scene.
