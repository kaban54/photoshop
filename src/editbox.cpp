#include "editbox.h"

EditBox::EditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size):
    Widget (x, y, w, h),
    txt ("", fnt, char_size),
    editing (false),
    cursor_pos (0),
    cursor_visible (false)
    {}


void EditBox::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRect (GetBounds(), Color(255, 255, 255), to_draw);
    if (txt.str.size() > 0) rt.DrawText (txt, GetPos() + Vec(10, 10), Color(0, 0, 0), to_draw);
}

void EditBox::DrawCursor (RenderTarget& rt, const RegionSet* to_draw) const {
    double cursor_x = txt.GetSize(cursor_pos).x + GetPos().x + 11;
    Color cursor_col = cursor_visible ? Color (0, 0, 128) : Color (255, 255, 255);
    rt.DrawRect (Rect (cursor_x, GetPos().y + 10, 2, txt.char_size), cursor_col, to_draw);
}

void EditBox::MousePress (const MouseState& mstate) {
    if ( MouseOnWidget(mstate.pos) && !editing) editing = true;
    if (!MouseOnWidget(mstate.pos) &&  editing) editing = false;
}

void EditBox::KeyboardPress (const KeyboardState& kstate) {
    if (!editing) return;
    switch (kstate.key) {
    
        case Enter_KEY:
            if (!kstate.shift) {
                editing = false;
                Render (*GetRendertarget(), GetRegset());
            }
            break;
        
        case Backspace_KEY:
            if (txt.str.size() > 0 && cursor_pos > 0) txt.str.erase (--cursor_pos, 1);
            Render (*GetRendertarget(), GetRegset());
            break;

        case Delete_KEY:
            if (txt.str.size() > 0 && cursor_pos < txt.str.size()) txt.str.erase (cursor_pos, 1);
            Render (*GetRendertarget(), GetRegset());
            break;
        
        case Right_KEY:
            if (cursor_pos < txt.str.size()) cursor_pos++;
            Render (*GetRendertarget(), GetRegset());
            break;
        
        case Left_KEY:
            if (cursor_pos > 0) cursor_pos--;
            Render (*GetRendertarget(), GetRegset());
            break;

        default:
            char c = GetSymb(kstate);
            if (c != 0) {
                txt.str.insert (cursor_pos++, 1, c);
                Render (*GetRendertarget(), GetRegset());
            }
            break;
    }
}

void EditBox::TimerEvent (double time) {
    if (!editing) return;

    if (time - floor (time) < 0.5) {
        if (cursor_visible) {
            cursor_visible = false;
            DrawCursor (*GetRendertarget(), GetRegset());
        }
    }
    else {
        if (!cursor_visible) {
            cursor_visible = true;
            DrawCursor (*GetRendertarget(), GetRegset());
        }
    }

}


FloatNumEditBox::FloatNumEditBox (double x, double y, double w, double h, const Font& fnt, size_t char_size):
    EditBox (x, y, w, h, fnt, char_size),
    point_is_set (false),
    minus_is_set (false)
    {}

void FloatNumEditBox::KeyboardPress (const KeyboardState& kstate) {
    if (!editing) return;
    switch (kstate.key) {
        case Enter_KEY:
            if (!kstate.shift) {
                editing = false;
                Render (*GetRendertarget(), GetRegset());
            }
            break;
        
        case Backspace_KEY:
            if (txt.str.size() > 0 && cursor_pos > 0) {
                if      (txt.str[cursor_pos - 1] == '.') point_is_set = false;
                else if (txt.str[cursor_pos - 1] == '-') minus_is_set = false;
                txt.str.erase (--cursor_pos, 1);
                Render (*GetRendertarget(), GetRegset());
            }
            break;

        case Delete_KEY:
            if (txt.str.size() > 0 && cursor_pos < txt.str.size()) {
                if      (txt.str[cursor_pos] == '.') point_is_set = false;
                else if (txt.str[cursor_pos] == '-') minus_is_set = false;
                txt.str.erase (cursor_pos, 1);
                Render (*GetRendertarget(), GetRegset());
            }
            break;
        
        case Right_KEY:
            if (cursor_pos < txt.str.size()) cursor_pos++;
            Render (*GetRendertarget(), GetRegset());
            break;
        
        case Left_KEY:
            if (cursor_pos > 0) cursor_pos--;
            Render (*GetRendertarget(), GetRegset());
            break;

        case Period_KEY:
            if (!point_is_set && !kstate.shift) {
                txt.str.insert (cursor_pos++, 1, '.');
                point_is_set = true;
                Render (*GetRendertarget(), GetRegset());
            }
            break;

        default:
            char c = GetSymb(kstate);
            if (c >= '0' && c <= '9' && !(minus_is_set && cursor_pos == 0)) {
                txt.str.insert (cursor_pos++, 1, c);
                Render (*GetRendertarget(), GetRegset());
            }
            if (c == '-' && cursor_pos == 0 && !minus_is_set) {
                txt.str.insert (cursor_pos++, 1, c);
                minus_is_set = true;
                Render (*GetRendertarget(), GetRegset());
            }
            break;
    }
}

double FloatNumEditBox::TextToDouble() const {
    return atof (txt.str.c_str());
}