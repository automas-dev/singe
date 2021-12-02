
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
