
# SceneView Assignment

## Design choices
- In the `SceneviewScene` constructor, `settingsChanged` is called to intialize the primitives based on setting tesselation parameters. 
- `settingsChanged()` handles LOD if the member variable is set to true
- In `setGlobalData`, the constants in `m_global` are fed into `m_phongShader`
- In `setLights`, each light is fed into `m_phongShader` to be set up
- In `renderGeometry`, each `TransformedPrimitive` stored in the scene is looped through. The material and transformations are fed into `m_phongShader`, and depending on the type, a primitive is drawn (the primitive's tesselation values based on what the last settings change had, only recreated if settings are modified). 


### Extra Features
- LOD
  - In `scenegraph/SceneviewScene.h`, the boolean variable `m_should_use_LOD` can be changed to turn off LOD.
  - Essentially, if the number of primitives exceed `m_count_threshold` (also set in the same location as `m_should_use_LOD`), then the tesselation parameters would be set to a maximum of 10. 

### Bugs
- N/A

