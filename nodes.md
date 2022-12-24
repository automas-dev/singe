
- Scene
  - [Model](include/singe/Core/Model.hpp)
    - [Transform3d](src/Transform3d/include/Transform3d.hpp)
    - [Geometry[]](include/singe/Core/Geometry.hpp)
      - [x] [Mesh*](include/singe/Core/Mesh.hpp)
      - [x] Material Index
    - [Material[]](include/singe/Core/Material.hpp)
      - [x] Base Color
        - ambient
        - diffuse
        - specular
      - [ ] Texture
        - [x] albedo
        - [ ] normal
        - [ ] specular
      - [ ] Tint texture
      - [x] Alpha
      - [x] Specular Exponent

## Model Loading

Models are loaded into the main structs, they should be **loaded into a temp
struct** and **images should be loaded during init**, then transfer data when
sending to gpu.

## Materials

- **Send on Bind** (don't use the send method)
- Lines vs Triangles?
  - draw triangles and use polyfill to draw lines
- Color vs Texture vs MultiTexture
  - Base color
  - Use base color as tint when textured
- PBR?
  - How to support with shader
  - Shadows?
  - Reflections?
- When to send mesh to gpu?
  - Call Mesh::send()?

## Resource Ownership

ResourceManager can own assets for simple programs, but more complex games will
require loading duplicate resources as unique instances.

The executable / game should be responsible for owning resources.
ResourceManager can assist in loading these resources, but should not hold any
of them. The only exception can be shaders and possibly textures which hold no
unique state within the instance.

Scenes may need to own their resources, but then again, resources may need to
be shared between them. If models are loaded as unique instances, a model can 
have an instance for each scene.

Models hold a mesh via Geometry and some Materials. A Mesh does not hold any
state, and could be shared. Geometry holds a single mesh with some number of
Materials. Materials and Geometry are loaded with the model and should be a
unique instance for that model.

I don't want to have to own the model of a Scene. Each model should be a
unique instance because of it's transform.

### Future

Geometry or Mesh could hold the buffer in memory for reference or modification.
