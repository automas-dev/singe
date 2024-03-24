# Dev Log

## 2023-09-04

I've updated logging in the ResourceManager. I added debug messages when loading
resources from file and when using the cache. Resource logging was also updated
to use info messages instead of debug for method calls (ie. calling getTexture
will use info for the first log message detailing the method name and
parameters).

## 2023-07-23

This is the start of a development log. I have used markdown files in the past
to sketch out ideas or capture current state / in progress tasks. The scope of
this log is to capture thoughts and ideas about the current design and what
could be improved.

Catching up on some history, this is not the first incarnation of this project.
Originally it went by the name **S3E** which stood for **SFML 3D Engine**. I was
using SFML at the time for the majority of code including window management,
text / menus, loading textures, etc. Currently SFML is still used for window
management, though through the proxy `Window` class, and for Text which is used
in the menu system.

The first commit was Sep, 8 2020. The early days of the project were built with
makefile. This was later replaced with CMake which now maintains a complex set
of dependencies and build options.

As the project grew, it split into multiple sub-components build separately and
linked by CMake. Some of these sub-components were even migrated into separate
repos for cleaner maintenance and ability to re-use in more projects. Now,
dependencies like **glpp** and **Wavefront** have their own versioning and
testing structures with CMake to allow fast integration with this project
through the CMake fetch content module.

The project name was changed to **SINGE** which now stands for **Scene In Nodes
Game Engine** (or Graphics Engine if you prefer). Godot has been a heavy
inspiration.

In it's current state, many of the lower level details have been solidified in
the form of **glpp**, while nodes and their structure is changing with every
version. Recent development has enabled loading scenes from an xml file, though
ResourceManager, Model and Scene classes do not support all of it's features.
Future work for this project includes figuring out how nodes should be
implemented, how to load / run animations and more.
