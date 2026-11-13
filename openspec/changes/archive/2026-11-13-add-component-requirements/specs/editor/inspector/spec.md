## MODIFIED Requirements

### Requirement: Remove Component
Each reflected component's header SHALL have an ellipsis button at its right edge; the button, or a right-click on the header, SHALL open a menu with **Remove Component**. An unreflected component SHALL show an `<unreflected component>` line and no menu. Remove Component on a component that no other component on the entity requires SHALL take it off the entity at once: its section disappears, Add Component offers the type again, and the scene follows.

#### Scenario: Remove a mesh component
- **WHEN** the user chooses Remove Component on a mesh component
- **THEN** its section disappears, Add Component offers the type again, and the entity stops being drawn

#### Scenario: Remove a transform
- **WHEN** the user removes the transform of an entity that has no camera, camera controller, directional light or mesh
- **THEN** its section disappears and the gizmo disappears

## ADDED Requirements

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
