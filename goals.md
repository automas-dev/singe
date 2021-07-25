
# For Tonight

- [x] Voxel Engine

## Voxel Engine

- [x] Research existing APIs
- [x] Make Mesh class witch takes points to make a VBO, mesh can be drawn? maybe VBO takes a Mesh to construct
the Mesh should be an adapter between VBO and a model. It can take points or load an model from a file (eg. obj)
- [x] Render a Triangle
- [x] Render a Face
- [x] Texture each Face
- [x] Generate a point cloud
- [x] Make a cube point cloud
- [x] Render a Cube point cloud
- [x] Make some form of Chunk
- [x] Enable / Disable cube faces
- [x] Remove hidden faces

### Research Existing APIs

- [Lest make a voxel engine](https://sites.google.com/site/letsmakeavoxelengine/home?authuser=0)

### Render a Cube point cloud

The X and Z axis seem to be flipped somehow. Also, the cw and ccw from the
cube diagram doesn't line up with the culling. This may be because of the X / Z
axis flip or it may be from a deeper issue.

#### Hindsight

I had the Z axis flipped, with +z going into the screen. +z should go out of the screen

