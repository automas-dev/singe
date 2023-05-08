
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

**Re-structure**

- [Scene](src/singe-graphics/include/singe/Graphics/Scene.hpp)
  - vector<shared_ptr<[Scene](src/singe-graphics/include/singe/Graphics/Scene.hpp)>>
  - vector<shared_ptr<[Mesh](src/singe-graphics/include/singe/Graphics/Mesh.hpp)>>
  - glpp::Transform
- [Mesh](src/singe-graphics/include/singe/Graphics/Mesh.hpp)
  - VertexArrayBuffer
  - vector<Vertex>
  - shared_ptr<[Material](src/singe-graphics/include/singe/Graphics/Material.hpp)>
  - glpp::Transform
- [Material](src/singe-graphics/include/singe/Graphics/Material.hpp)
  - shared_ptr<[Shader](src/singe-graphics/include/singe/Graphics/Shader.hpp)>
  - shared_ptr<glpp::Texture>
  - ...
- [Shader](src/singe-graphics/include/singe/Graphics/Shader.hpp)
  - glm::Shader
  - glm::Uniform
- [RenderState](src/singe-graphics/include/singe/Graphics/RenderState.hpp)
  - glm::mat4

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

I don't want to have to own the model of a Scene. Each model should be a unique
instance because of it's transform.

### Future

☑️ Mesh could hold the buffer in memory for reference or modification.

# ☑️ Local Mesh

Goal: store mesh locally for modification.

Idea

- Mesh has source mesh that can be modified and used to update the vba

## Looking at Godot

- Mesh contains Material and Vertices
  - Could switch Mesh and Geometry back
  - Keep VertexArrayBuffer
