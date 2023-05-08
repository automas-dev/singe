# Scene In Nodes Game Engine

> This project has been through some major re-structuring in recent versions.
> Not everything has been updated and much has not yet been documented.

This project is a learning space where I can implement concepts in 3d rendering
and general game engine mechanics. It is a fork of the S3E project that focuses
on rendering through nodes instead of draw calls.

## Project Goals

- Sensible Defaults
- Minimal external resources, _a default font and shader are provided_
- Provide basic functionality
    - Window and Events
    - Resource loading
    - Texture / Shader / FrameBuffer / VBO
    - Camera Controls
- Provide access to underlying implementation for finer control
- Use latest and best practices for C++

### ToDo

- [ ] Raise exceptions for all load errors (eg. parsing model)
- [ ] All the Nodes
  - [ ] Node?
    - [ ] Spacial
      - [ ] Geometry
        - [ ] Mesh
        - [ ] Custom Draw
      - [ ] Camera
      - [ ] Light
        - [ ] Point
        - [ ] Directional
        - [ ] Spot
      - [ ] Physics Body
        - [ ] Kinematic
        - [ ] Rigid
        - [ ] Static
    - [ ] Environment
- [ ] Well Defined Resources
  - [ ] Texture
  - [ ] Mesh
  - [ ] Environment / Sky
  - [ ] Shader?
- [ ] Scene Descriptor File
- [ ] Remove derived Game class? Maybe?
  - [ ] Update GameBase to load scene's and other resources

## Dependencies

This project relies on abstractions for OpenGL form the
[GLPP](https://github.com/twh2898/glpp.git) project. For an exact list of
dependencies and their versions, see the cmake files in the
[external/](external/) directory.

- SFML 2.5.1
- GLEW 2.2.0
- OpenGL 3.0
- glm
- fmtlib
- spdlog
- (optional) Doxygen 1.9.2

## Building

```sh
git clone https://github.com/twh2898/singe.git
cd singe
mkdir build
cd build
cmake ..
make
```

## Quickstart

TODO
