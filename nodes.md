
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

# Nodes

After completing the scene file definition and SceneParser code, things are
starting to come together. The current structure of ResourceManager does not
allow for loading shaders, or any other resources, directly from a path
relative to the resources folder. In addition, Models and Scenes etc. don't
have any name attribute.

Plan: Create a base `Node` class from which all other classes extend. This is
exactly like nodes in Godot. I'm still not sure how to handle variable node
types with the C++ type system. Possible solutions include

- "Stringly" typed nodes with attributes, name, etc.
- Scenes are handled by code which does not need knowledge of the types
  - Game code will still create nodes that are directly interacted with
- Some lookup system by node name with casing to the "assumed" class type
- Can a visitor pattern be used here?
