# editor/viewport Specification

## Purpose
The scene viewports: camera navigation, the transform gizmo and its toolbar, and how gizmo edits interact with undo, shortcuts and window docking.

## Requirements

### Requirement: Viewports
The Editor SHALL open one viewport, `Scene`, whose image fills its panel with no padding. Every viewport SHALL outline the selected entity. Behaviours that mention several viewports SHALL hold for any viewport added later.

#### Scenario: Startup
- **WHEN** the Editor starts
- **THEN** one `Scene` viewport is shown, and its image fills the panel edge to edge

### Requirement: Camera navigation
The viewport camera SHALL move with WASD/Q/E only while the right mouse button is held. It SHALL respond only when its viewport is focused and hovered, or when the right-mouse press started there, and never during a gizmo drag.

#### Scenario: Right-click in another panel
- **WHEN** the viewport is focused and the user right-clicks in the Inspector and presses W
- **THEN** the viewport camera does not move

### Requirement: Gizmo visibility
A selected entity with a transform SHALL show a gizmo at its origin in every viewport. With no selection, or no transform, there SHALL be no gizmo.

#### Scenario: Entity without a transform
- **WHEN** the user selects an entity that has no transform
- **THEN** no gizmo is shown

### Requirement: Gizmo mode shortcuts
With a viewport focused, no right mouse button or Ctrl held and no text field active, W/E/R SHALL pick translate/rotate/scale, and Q or Esc SHALL hide the gizmo. Translate is the default. The mode SHALL be shared by all viewports and SHALL NOT change during a drag.

#### Scenario: Shortcut while typing
- **WHEN** a text field in the Inspector is active and the user types E
- **THEN** the gizmo mode does not change

### Requirement: Gizmo toolbar
Each viewport's toolbar SHALL be a vertical column at its top-left with 40px icon buttons for Select, Translate, Rotate and Scale. The active one SHALL use the accent, clicking one SHALL set the shared mode, and hovering SHALL show the name and shortcut (`Translate (W)`, `Select (Q)`).

#### Scenario: Toolbar tooltip
- **WHEN** the user hovers the translate button
- **THEN** a tooltip shows `Translate (W)`

### Requirement: Live gizmo drag
Dragging a handle in one viewport SHALL update the entity live in all viewports and in the Inspector. Other viewports' gizmos SHALL NOT respond to that drag, and their handles SHALL disappear until the drag ends. Translate SHALL change only position, rotate only rotation and scale only scale, leaving the other two fields at their exact values.

#### Scenario: Translate drag
- **WHEN** the user drags a translate handle
- **THEN** only the position changes, live in every viewport and the Inspector, and rotation and scale keep their exact values

### Requirement: Rotation without jumps
An entity rotated on all three axes SHALL turn smoothly under the rotate gizmo, with no pop at the start of a drag.

#### Scenario: Rotate a fully rotated entity
- **WHEN** an entity rotated on X, Y and Z is dragged with the rotate gizmo
- **THEN** it turns smoothly from its current orientation

### Requirement: One drag is one undo step
One drag SHALL be one undo step, ending on mouse release even outside the viewport. Undo SHALL restore the exact value from before the drag, and Redo SHALL reapply it. Clicking a handle without moving SHALL add no step. Undo, Redo, New, Open and Save shortcuts SHALL do nothing during a drag.

#### Scenario: Release outside the viewport
- **WHEN** the user drags a handle and releases the mouse outside the viewport
- **THEN** the drag ends and adds exactly one undo step

#### Scenario: Shortcut during a drag
- **WHEN** the user presses Ctrl+Z during a drag
- **THEN** nothing is undone

### Requirement: Docking and resizing
Dragging a handle in a floating viewport SHALL NOT move the window. The gizmo SHALL stay lined up with the entity after a viewport is resized or re-docked.

#### Scenario: Resize a viewport
- **WHEN** the user resizes the viewport panel
- **THEN** the gizmo stays on the entity's origin

### Requirement: Valid values and snapping
A gizmo SHALL never write a NaN or infinite value, and a scale axis at zero SHALL stay at zero under a scale drag. Holding Ctrl during a drag SHALL snap position to multiples of 1, scale to multiples of 0.1 (with the handles stepping along), and rotation to 15-degree steps from where the drag started. Snapping SHALL NOT collapse a scale axis to 0.

#### Scenario: Snapped scale
- **WHEN** the user holds Ctrl while dragging a scale handle
- **THEN** the scale moves in steps of 0.1 and never reaches 0
