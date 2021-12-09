
# Filter Assignment

## Design choices

### General design
- The base `Filter` class only contains one shared function: `getCanvasPixFloat`. This loads in the RGBA pixels on the canvas, then flattens them to a 1D vector of vec4's containing all four color channels (values `[0.0f, 1.0f]`).
- `FilterUtils` contains two notable functions that are called externally: `vec4toRGBA`, which converts a vec4 pixel of color channels (values `[0.0f, 1.0f]`) to an RGBA pixel.
- The other notable function in `FilterUtils` is `ConvolveSeparate2D`, which takes in two separated kernels and applies it to the vector of pixels. This function calls the helper function `Convolve1D` twice.

### Similarities and reuse of code
- All three filters implemented use the `getCanvasPixFloat` to retrieve and convert the canvas pixel data into usable form
- `FilterEdgeDetect` and `FilterBlur` both use `ConvolveSeparate2D`, with the latter generating a gaussian kernel instead of a pre-specified one
- `FilterScale` uses similar code for horizontal and vertical scaling, with minor tweaks in the `hPrime` function to differentiate between the orientations


### Extra Features
- The blur filter implementation uses Gaussian blur (see `filter/FilterBlur.cpp`)

### Bugs
- N/A

