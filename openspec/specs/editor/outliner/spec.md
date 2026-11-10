# editor/outliner Specification

## Purpose
The Outliner panel lists the scene's entities and is where entities are selected, renamed, created and deleted.

## Requirements

### Requirement: Entity type icons
Each row SHALL show an icon for what the entity is: camera, then light, then mesh, in that priority, or a generic marker for an entity with none of those.

#### Scenario: Entity with a camera and a mesh
- **WHEN** an entity has both a camera and a mesh component
- **THEN** its row shows the camera icon

#### Scenario: Entity with none of those
- **WHEN** an entity has only a transform
- **THEN** its row shows the generic marker
