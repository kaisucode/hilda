#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "QuadraticBrush.h"
#include "RGBA.h"

/**
 * @class SmudgeBrush
 *
 * Smudge brush, which smudges color values across the canvas as the mouse is dragged.
 */
class SmudgeBrush : public QuadraticBrush {

public:
    SmudgeBrush(RGBA color, int radius);
    virtual ~SmudgeBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas) override;
    virtual void brushDragged(int x, int y, Canvas2D *canvas) override;
    virtual void brushUp(int x, int y, Canvas2D *canvas) override;

protected:
    // Constructs the mask for this brush.
    void makeMask() override;

    //! Copy paint in the bounding box of the brush from canvas to m_paint
    void pickUpPaint(int x, int y, Canvas2D* canvas);

    std::vector<RGBA> m_paint;
};

#endif
