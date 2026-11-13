# editor/scene-files Specification

## Purpose
What the editor does with a scene file's contents when it opens one, including files that break the rules components declare.

## Requirements

### Requirement: Missing requirements on open
When a scene is opened, every required component type missing from an entity SHALL be added with default values before the entity joins the scene, including the requirements of components added this way. A warning SHALL be logged for each added component naming the entity, the component that requires it and the added type. Components read from the file SHALL keep their saved values.

#### Scenario: Light without a transform
- **WHEN** a scene file holds an entity with a directional light and no transform, and the user opens it
- **THEN** the entity gets a default transform, the light keeps its saved colour and intensity, and one warning naming the entity, `DirectionalLightComponent` and `TransformComponent` is logged

#### Scenario: Missing requirement that has its own requirement
- **WHEN** a scene file holds an entity with a component whose required type in turn requires another type, and neither is in the file
- **THEN** both required components are added and one warning is logged for each

#### Scenario: Valid scene
- **WHEN** a scene saved by the editor is opened
- **THEN** no requirement warning is logged and every component is loaded as saved, whatever order the file lists them in
