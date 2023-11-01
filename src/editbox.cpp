#include "editbox.h"

EditBox::EditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size):
    Widget (x, y, w, h),
    txt ("", fnt, char_size),
    editing (false),
    cursor_pos (0)
    {}


void EditBox::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRect (GetBounds(), Color(255, 255, 255), to_draw);
    if (txt.str.size() > 0) rt.DrawText (txt, GetPos() + Vec(10, 10), Color(0, 0, 0), to_draw);

    if (editing) {
        double cursor_x = txt.GetSize(cursor_pos).x + GetPos().x + 11;
        rt.DrawRect (Rect (cursor_x, GetPos().y + 10, 2, txt.char_size), Color (0, 0, 128), to_draw);
    }
}

void EditBox::MousePress (const MouseState& mstate) {
    if ( MouseOnWidget(mstate.pos) && !editing) editing = true;
    if (!MouseOnWidget(mstate.pos) &&  editing) editing = false;
}

void EditBox::KeyboardPress (const KeyboardState& kstate) {
    if (!editing) return;
    if (kstate.key == Enter_KEY && !kstate.shift) {
        editing = false;
        Render (*GetRendertarget(), GetRegset());
        return;
    }
    if (kstate.key == Backspace_KEY) {
        if (txt.str.size() > 0 && cursor_pos > 0) txt.str.erase (--cursor_pos, 1);
        Render (*GetRendertarget(), GetRegset());
    }
    else if (kstate.key == Delete_KEY) {
        if (txt.str.size() > 0 && cursor_pos < txt.str.size()) txt.str.erase (cursor_pos, 1);
        Render (*GetRendertarget(), GetRegset());
    }
    else if (kstate.key == Right_KEY) {
        if (cursor_pos < txt.str.size()) cursor_pos++;
        Render (*GetRendertarget(), GetRegset());
    }
    else if (kstate.key == Left_KEY) {
        if (cursor_pos > 0) cursor_pos--;
        Render (*GetRendertarget(), GetRegset());
    }
    char c = GetSymb(kstate);
    if (c != 0) {
       txt.str.insert (cursor_pos++, 1, c);
       Render (*GetRendertarget(), GetRegset());
    }
}
