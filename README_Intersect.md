
# Intersect Assignment

## Design choices

### Data structures
- The `Ray` class stores the position, `d`, minimum t-value, normals, and the transformed primitive
- The `RayShape` class stores the position, `d`, the transformed primitive, then is able to compute `t-value`s and normals, returning these two back as a pair using the  `getTValueAndNormal` function

### General design
- Upon `renderImage` in the `Canvas2D` class, the camera's transformation matrix is calculated. We use the inverse of this to convert from camera space to world space.
- Then we loop through each pixel, getting `d` values for each pixel with `getNormalizedD`, then checking each primitive stored for intersections. 
- We calculate the minimum `t-value1` and the primitive it belongs to using `getClosestIntersection`. 
- Which converts from world space to object space, using the transformations of the specific primitive.
- Then calls `getTValueAndNormal` to retrieve the minimum t-value of each primitive. 
- We take the primitive with the minimum t-value, and the correpsonding normal for that intersection
- We create a ray from that primitive and `t-value`, if an intersection exists.
- If an intersection exists, in `getColor`, we use the simplified lighting equation on that ray to determine the color of that particular pixel.

### Shape similarities and reuse of code
- The cone cap and cylinder caps share the same code, and so `pointOnCapIsValid` is used to check the validity of the `t-value`s produced
- The sphere, cone sides, and cylinder share the same code, and so `getValidTsFromQuadratic` is used to find `t-values`. The only differences are in the `A`, `B` and `C` values, which are passed in as parameters, and the surface normals, which is determined by `calculateSurfaceNormal`.

### Extra Features
- N/A

### Bugs
- Sometimes, loading in `example.xml` from lab 03 causes a parsing error. 

