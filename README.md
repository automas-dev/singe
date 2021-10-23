# Scene In Nodes Game Engine

This project is a learning space where I can implement concepts in 3d rendering
and general game engine mechanics. It is a fork of the S3E project that handles
more of the rendering through nodes instead of draw calls.

### Project Goals

- Sensible Defaults
- Minimal external resources, _a default font and shader are provided_
- Provide basic functionality
    - Window and Events
    - Resource loading
    - Texture / Shader / FrameBuffer / VBO
    - Camera Controls
- Provide access to underlying implementation for finer control

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
