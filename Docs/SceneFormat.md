# Scene File Format

Scenes are described in JSON. The loader (Phase 2 Chunk 3) will parse them and
construct entities + components via the ECS.

---

## Top-level structure

```json
{
  "version": 1,
  "materials": { },
  "entities":  [ ]
}
```

`version` is a forward-compatibility guard. The loader rejects files whose version
it does not recognise.

---

## Materials

Named entries in the `materials` object. Entities reference them by name.
Sharing one material name across multiple entities maps to a single
`SharedPtr<Material>` at runtime — matching how the code already works.

```json
"materials": {
  "<material_name>": {
    "shader": "<virtual_path>",
    "uniforms": { },
    "textures": { }
  }
}
```

### `shader`

Virtual path resolved by `FileSystemModule`. Prefix determines the mount point:
`engine://` → engine assets, `project://` → sandbox assets.

### `uniforms`

Key → value map. Types are inferred from JSON:

| JSON value          | GLSL / C++ type |
|---------------------|-----------------|
| `0.4`               | `float`         |
| `1`                 | `int`           |
| `[x, y]`            | `Vec2`          |
| `[x, y, z]`         | `Vec3`          |
| `[x, y, z, w]`      | `Vec4`          |

`u_CameraPos` is set every frame by the renderer and must **not** appear here.

### `textures`

```json
"textures": {
  "<uniform_name>": {
    "path": "<virtual_path>",
    "slot": 0
  }
}
```

`slot` is the texture unit index passed to `Material::SetTexture`.

---

## Entities

An ordered array. Entities are created in array order, which determines the order
they appear in the scene's entity list.

```json
"entities": [
  {
    "name": "<string>",
    "transform":         { },
    "mesh":              { },
    "camera":            { },
    "camera_controller": { }
  }
]
```

`name` is optional but recommended for debugging. All component keys are optional;
omitting one means the entity has no component of that type.

---

### `transform`

```json
"transform": {
  "position": [0.0, 0.0, 0.0],
  "rotation": [0.0, 0.0, 0.0],
  "scale":    [1.0, 1.0, 1.0]
}
```

- **position** — world-space XYZ. Default `[0, 0, 0]`.
- **rotation** — Euler angles in **degrees**, XYZ order. Converted to radians on
  load. Default `[0, 0, 0]`.
- **scale** — XYZ scale factors. A single number is shorthand for uniform scale:
  `"scale": 2.0` → `[2, 2, 2]`. Default `[1, 1, 1]`.

Any omitted field uses its default.

---

### `mesh`

```json
"mesh": {
  "path":     "<virtual_path>",
  "material": "<material_name>"
}
```

- **path** — virtual path to a mesh file, or a builtin geometry name:
  - `builtin://Quad`
  - `builtin://Cube`
  - `builtin://Triangle`
- **material** — must match a key in the top-level `materials` object.

---

### `camera`

```json
"camera": {
  "type": "perspective",
  "fov":  60.0,
  "near": 0.1,
  "far":  1000.0
}
```

`type` is `"perspective"` or `"orthographic"`.

**Perspective** fields: `fov` (vertical, degrees), `near`, `far`.
Aspect ratio is **not** stored — it is derived from the window at runtime and
updated on resize.

**Orthographic** fields: `left`, `right`, `bottom`, `top`, `near`, `far`.

---

### `camera_controller`

```json
"camera_controller": {
  "move_speed":        0.5,
  "look_sensitivity":  0.002
}
```

Both fields are optional and default to the values above.

---

## Example — current sandbox scene

See [../Sandbox/Assets/Scenes/main.scene.json](../Sandbox/Assets/Scenes/main.scene.json).
