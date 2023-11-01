#ifndef EDITBOX_H
#define EDITBOX_H

#include "widget.h"

class EditBox : public Widget {
    Text txt;
    bool editing;
    size_t cursor_pos;

    public:

    explicit EditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size);

    virtual void Render (RenderTarget& rt, const RegionSet* to_draw) const override;

    virtual void MousePress (const MouseState& mstate) override;

    virtual void KeyboardPress (const KeyboardState& kstate) override;
};

#endif