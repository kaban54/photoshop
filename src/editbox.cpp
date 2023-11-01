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

void EditBox::KeyboardPress (KeyboardKey key) {
    if (!editing) return;
    if (key == Enter_KEY) {
        editing = false;
        return;
    }
    else if (key >= A_KEY && key <= Z_KEY) {
        txt.str.insert (cursor_pos++, 1, 'a' + key - A_KEY);
        Render (*GetRendertarget(), GetRegset());
    }
    else if (key >= NUM0_KEY && key <= NUM9_KEY) {
        txt.str.insert (cursor_pos++, 1, '0' + key - NUM0_KEY);
        Render (*GetRendertarget(), GetRegset());
    }
    else if (key == Backspace_KEY) {
        if (txt.str.size() > 0 && cursor_pos > 0) txt.str.erase (--cursor_pos, 1);
        Render (*GetRendertarget(), GetRegset());
    }
    else if (key == Right_KEY) {
        if (cursor_pos < txt.str.size()) cursor_pos++;
        Render (*GetRendertarget(), GetRegset());
    }
    else if (key == Left_KEY) {
        if (cursor_pos > 0) cursor_pos--;
        Render (*GetRendertarget(), GetRegset());
    }


}
