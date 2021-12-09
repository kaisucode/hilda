
# Ray Assignment

## Changing the recursive depth limit
- Edit `m_maxDepth` member variable in `scenegraph/RayScene.h` to change the maximum depth value (currently set to `5`).

## Design choices

### Data structures
- The `IntersectPoint` class stores the position, `d`, minimum t-value, normals, the transformed primitive, and `uv` values
- The `RayBaseShape` class stores the position, `d`, the transformed primitive, then is able to `findIntersection` and then generate an `IntersectPoint` from that result. 

### General design
- `Canvas2D` loops through each pixel, and determines the closest intersection point by using `getClosestIntersectionPoint` to craft shapes and compare t-values, as well as compute normals, `uv`, and other information.
- If an intersection is found, then the lighting equation is applied.
- Each light in the scene is looped through, and if it's obstructed by another object then that light is not counted (shadow)
- Diffuse and specular components are calculated.
- Reflections are then added, by recursively going through `getColor` with new starting points (different IntersectionPoints)

### Shape similarities and reuse of code
- All shapes inherit `RayBaseShape`, with override functions of `getSurfaceNormals` and `findIntersection` for their unique features. 
- The Cone, Cylinder, and Sphere `uv` values share the same `u` computations, and so the sphere shape overwrites the `v` value after computing `uv` the same way Cone and Cylinder work for their barrel portions.
- Cone barrels, Cylinder barrels, and Sphere share code for finding t-values.
- Cone bottoms, Cylinder tops and bottoms, and cube share code for finding `uv` values.


### Extra Features
- N/A

### Bugs
- Reflections on `recursiveSpheres2.xml` don't work on certain spheres. The textures render correctly when reflections are turned off, but when it's on the shade is incorrect.

