This is a real time strategy and factory builder game I'm working on.

When updating dependencies:

```bash
make clean && bear -- make
```

## Profiling with Tracy

Tracy 0.13.1 is pinned as a Git submodule. Clone the project with its
dependencies, then compile the instrumented build:

```bash
git submodule update --init --recursive
make clean
make PROFILE=1
./build/space-wars
```

Run the matching Tracy 0.13.1 profiler application and connect to the local
`space-wars` process. The capture exposes frame boundaries and frame-time
plots, with zones for scene updates, render-queue construction, GPU data
updates, Vulkan command recording, queue submission, presentation, and UI.

Use plain `make` for a build with all Tracy instrumentation compiled out.

In blender:

- Shade smooth on or off

When exporting in blender:
- Forward as "Z Forward"
- Up as "-Y Up"
- Turn on "Triangulate Faces"
