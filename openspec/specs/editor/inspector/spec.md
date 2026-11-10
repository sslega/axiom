# editor/inspector Specification

## Purpose
The Inspector panel shows and edits the selected entity's name and components, including resource fields and removing components, with every edit undoable.

## Requirements

### Requirement: Resource field dropdown
A resource field SHALL show a dropdown with its current virtual path, or `None` when it is empty. Opening the dropdown SHALL list `None`, then every file under `engine://` and `project://` that loads as the field's resource type, as virtual paths sorted by name without case, with the current one marked. The folders SHALL be scanned each time the dropdown opens.

#### Scenario: Listing candidates
- **WHEN** the user opens the dropdown of a mesh field
- **THEN** it lists `None` followed by every loadable mesh under `engine://` and `project://`, sorted case-insensitively, with the current mesh marked

#### Scenario: File added while running
- **WHEN** a new mesh file is added to `Game/Assets` while the Editor runs and the dropdown is opened again
- **THEN** the new file appears without a restart

### Requirement: Picking a resource
Picking an entry SHALL change the resource at once, in every viewport. Picking `None` SHALL clear the field, and a mesh component with an empty mesh or material SHALL NOT be drawn. Picking the entry that is already current SHALL do nothing; paths compare without case.

#### Scenario: Pick a different mesh
- **WHEN** the user picks another mesh in the dropdown
- **THEN** every viewport draws the entity with the new mesh immediately

#### Scenario: Pick None
- **WHEN** the user picks `None` for a mesh component's material
- **THEN** the field shows `None` and the entity is no longer drawn

#### Scenario: Pick the current entry
- **WHEN** the user picks the entry that is already current, in any letter case
- **THEN** nothing changes and no undo step is added

### Requirement: Resource picks are undoable and saved
One pick SHALL be one undo step: Undo restores the previous resource, Redo reapplies the pick. A picked path SHALL be saved with the scene and come back on Open, in the same form as a hand-written one (`engine://Meshes/cube.obj`).

#### Scenario: Undo a pick
- **WHEN** the user picks a new mesh and then presses Undo
- **THEN** the previous mesh is restored, and Redo applies the pick again

#### Scenario: Save and reopen
- **WHEN** the user picks a mesh, saves the scene and opens it again
- **THEN** the field shows the picked virtual path

### Requirement: Headers and icons
The entity name SHALL show its type icon. Each component header SHALL show an accent-coloured type icon and a spaced title (`Mesh Component`). Components SHALL be listed sorted by type name, so a component stays in the same place through remove and undo.

#### Scenario: Component order is stable
- **WHEN** the user removes a component and then undoes the removal
- **THEN** the component reappears in the same position in the list

### Requirement: Remove Component
Each reflected component's header SHALL have an ellipsis button at its right edge; the button, or a right-click on the header, SHALL open a menu with **Remove Component**. An unreflected component SHALL show an `<unreflected component>` line and no menu. Remove Component SHALL take the component off the entity at once: its section disappears, Add Component offers the type again, and the scene follows.

#### Scenario: Remove a mesh component
- **WHEN** the user chooses Remove Component on a mesh component
- **THEN** its section disappears, Add Component offers the type again, and the entity stops being drawn

#### Scenario: Remove a transform
- **WHEN** the user removes the transform of an entity that has a mesh
- **THEN** the gizmo disappears and the mesh is drawn at the origin

### Requirement: Removals are undoable
A removal SHALL be one undo step. Undo SHALL bring the same component back with every value it had, and earlier undo steps on its fields SHALL still work afterwards.

#### Scenario: Undo a removal
- **WHEN** the user edits a component's field, removes the component, then presses Undo twice
- **THEN** the component returns with its edited values, and the second Undo reverts the field edit
