#ifndef EDITBOX_H
#define EDITBOX_H

#include "widget.h"
#include <cstdlib>

class EditBox : public Widget {
    public:
    Text txt;
    bool editing;
    size_t cursor_pos;
    bool cursor_visible;

    explicit EditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    void DrawCursor (RenderTarget& rt, const RegionSet* to_draw) const;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void KeyboardPress (const KeyboardState& kstate) override;

    virtual void TimerEvent (double time) override;
};

class FloatNumEditBox : public EditBox {
    bool point_is_set;
    bool minus_is_set;
    public:

    explicit FloatNumEditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size);

    virtual void KeyboardPress (const KeyboardState& kstate) override;

    double TextToDouble() const;
};

#endif