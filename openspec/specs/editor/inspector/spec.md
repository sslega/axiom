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
Each reflected component's header SHALL have an ellipsis button at its right edge; the button, or a right-click on the header, SHALL open a menu with **Remove Component**. An unreflected component SHALL show an `<unreflected component>` line and no menu. Remove Component on a component that no other component on the entity requires SHALL take it off the entity at once: its section disappears, Add Component offers the type again, and the scene follows.

#### Scenario: Remove a mesh component
- **WHEN** the user chooses Remove Component on a mesh component
- **THEN** its section disappears, Add Component offers the type again, and the entity stops being drawn

#### Scenario: Remove a transform
- **WHEN** the user removes the transform of an entity that has no camera, camera controller, directional light or mesh
- **THEN** its section disappears and the gizmo disappears

### Requirement: Removals are undoable
A removal SHALL be one undo step. Undo SHALL bring the same component back with every value it had, and earlier undo steps on its fields SHALL still work afterwards.

#### Scenario: Undo a removal
- **WHEN** the user edits a component's field, removes the component, then presses Undo twice
- **THEN** the component returns with its edited values, and the second Undo reverts the field edit

### Requirement: Required components can't be removed
When another component on the same entity requires a component, its **Remove Component** item SHALL be shown disabled, and hovering it SHALL show a tooltip naming every component on the entity that requires it (`Required by Camera Controller`). Choosing it SHALL do nothing and add no undo step. Once nothing on the entity requires the component any more, the item SHALL be enabled again.

#### Scenario: Transform required by a light
- **WHEN** the user opens the component menu of the transform on an entity with a directional light
- **THEN** Remove Component is disabled, its tooltip reads `Required by Directional Light Component`, and the transform stays

#### Scenario: Transform required by a mesh
- **WHEN** the user opens the component menu of the transform on an entity with a mesh
- **THEN** Remove Component is disabled and its tooltip reads `Required by Mesh Component`

#### Scenario: Requirement goes away
- **WHEN** the user removes the directional light from that entity and opens the transform's menu again
- **THEN** Remove Component is enabled and removes the transform

### Requirement: Adding a component brings its requirements
Add Component SHALL first add every component type the chosen one requires that the entity lacks, including the requirements of those, each with default values. Requirements the entity already has SHALL be kept as they are. The whole add SHALL be one undo step: Undo removes the chosen component and every component added for it, and Redo restores the same components with their values.

#### Scenario: Add a camera controller to an empty entity
- **WHEN** the user adds a camera controller to an entity with no components
- **THEN** the entity gets a transform and a camera controller, and the gizmo appears

#### Scenario: Requirement already present
- **WHEN** the user moves an entity's transform and then adds a directional light to it
- **THEN** only the light is added and the transform keeps its position

#### Scenario: Undo the add
- **WHEN** the user adds a camera controller to an entity with no components and presses Undo
- **THEN** both the camera controller and the transform are gone, and Redo brings both back
