# editor/selection Specification

## Purpose
How the editor's single selection stays valid when entities leave and return to the scene through delete, undo and redo.

## Requirements

### Requirement: Selection is cleared when the entity leaves
Once the selected entity leaves the scene (Delete, or undo of its creation), nothing SHALL be selected: no Inspector content, gizmo or outline.

#### Scenario: Delete the selected entity
- **WHEN** the user deletes the selected entity
- **THEN** the Inspector is empty and no gizmo or outline is shown

### Requirement: Selection returns with the entity
If undo or redo brings the previously selected entity back, it SHALL be selected again, including when undoing a Delete in the Outliner.

#### Scenario: Undo a delete
- **WHEN** the user deletes the selected entity and presses Undo
- **THEN** the entity is back and selected

### Requirement: Undone entities stay untouched
Creating an entity, undoing it and then making any new edit SHALL NOT touch the removed entity.

#### Scenario: New edit after undoing a creation
- **WHEN** the user creates an entity, undoes it, then edits another entity
- **THEN** the edit applies only to the other entity and nothing crashes
