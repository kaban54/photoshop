#ifndef EDITBOX_H
#define EDITBOX_H

#include "widget.h"
#include <cstdlib>
#include <string>

static const size_t MAX_TXT_LEN = 256;

struct EditBox : public Widget {
    std::string txt;
    size_t char_size;
    size_t cursor_pos;
    uint64_t timer;
    bool editing;
    bool cursor_visible;


    explicit EditBox (double x, double y, double w, double h, size_t char_size_);

    ~EditBox() = default;

    void SetText(const char* str);

    virtual void RenderInRegset(RenderTarget& rt, const RegionSet* to_draw) override;

    void DrawCursor_rs (RenderTarget& rt, const RegionSet* to_draw);

    virtual bool onMousePress   (MouseContext context) override;
    virtual bool onMouseRelease (MouseContext context) override {return false;}
    virtual bool onMouseMove    (MouseContext context) override {return false;}
    virtual bool onKeyboardPress   (KeyboardContext context) override;
    virtual bool onKeyboardRelease (KeyboardContext context) override {return false;}
    virtual bool onClock (uint64_t delta) override;
};

class FloatNumEditBox : public EditBox {
    bool point_is_set;
    bool minus_is_set;
    public:

    explicit FloatNumEditBox (double x, double y, double w, double h, size_t char_size);

    virtual bool onKeyboardPress (KeyboardContext context) override;

    double TextToDouble() const;
};

char GetSymb (const KeyboardContext context);

#endif