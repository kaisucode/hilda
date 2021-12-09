
# Brush Assignment
- In this design, the masks stores the entirety of the area of paint to be applied, a squared area of `2 * brush_radius + 1`. However, we acknowledge that there is an alternate method of only storing a quarter of the mask, and inferring the rest of it from that quarter by using absolute values of distances from the click point. The alternate method is not used here (implemented in branch `symmetricMasks`), because that method does not account for brushes that are asymmetrical in the four corners. 

### Extra Brushes
- N/A

### Bugs
- When the smudge brush is moved around too quickly, it sometimes causes ripples or pixelated smudges.

