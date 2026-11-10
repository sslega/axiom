---
tags: [engine, core, frame]
updated: 2026-11-08
---

# Frame Lifecycle

**In one sentence:** each frame is *events → update → render*, and render is split into **collect** (requests are recorded) and **execute** (the renderer consumes them all at once).

> [!tip] The most important page
> If you can draw this from memory, you can place almost any feature. "It runs at the wrong time" bugs are answered here.

## One frame

```mermaid
flowchart TB
    E["Poll events<br/>window / keyboard events dispatched"] --> U
    subgraph U["Update (simulation)"]
        U1["subsystems: World → Scene → Entity → Component"] --> U2["app: editor cameras"]
    end
    U --> B
    subgraph B["Begin frame"]
        B1["Render: clear last frame's requests<br/>(views, render flags), recycle temp buffers"] --> B2["ImGui: new frame"]
    end
    B --> C
    subgraph C["COLLECT (app render hook)"]
        C1["panels draw ImGui"] --> C2["viewports submit Views"] --> C3["selection → render flags"]
    end
    C --> X
    subgraph X["EXECUTE (subsystem render hooks)"]
        X1["Render: build render commands,<br/>render every View"] --> X2["ImGui: draw UI to window"]
    end
    X --> P["End frame: swap buffers"]
```

## Collect, then execute

The renderer never draws when asked. It **records requests** in the collect window and **executes them all** later:

```mermaid
flowchart LR
    BS["Begin frame:<br/>requests cleared"] --> R1["submit View"]
    BS --> R2["add render flags"]
    R1 --> EX["Render subsystem<br/>consumes all requests"]
    R2 --> EX
```

- A request made **before** the clear is lost. One made **after** execute is too late.
- This is the same shape a future **render thread** needs: one side records, the other executes.

> [!warning] Fragile contract (flagged for refactor)
> The collect window exists only because of where hooks happen to be called. Inside the editor's render hook, the selection must be submitted **after** the panels ran, because ImGui is immediate mode: clicks, Delete and Ctrl+Z are handled *while* panels draw. An explicit "submit to renderer" phase would make this visible instead of implicit.

## Two surprises

- **The viewport shows this frame's image even though its ImGui call comes before rendering.** ImGui only *records* "draw this texture". The texture is filled during execute, and ImGui draws it afterwards.
- **Update's delta time is in milliseconds**, not seconds.

## Game build

The Game app submits **no views** today, so it shows only its ImGui window. See [Known Oddities](../Reference/Known%20Oddities.md).

## Where to next

- [Scene and ECS](../Data/Scene%20and%20ECS.md): what runs during update.
- [Rendering Pipeline](../Rendering/Rendering%20Pipeline.md): what happens inside execute.
- [Editor](../Editor/Editor.md): the panels and viewports that fill the collect window.
- [Application and Subsystems](Application%20and%20Subsystems.md): why the hooks run in this order.
- Back to the [Engine Map](../Engine%20Map.md).
