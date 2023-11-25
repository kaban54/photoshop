#include "editbox.h"

EditBox::EditBox (double x, double y, double w, double h, size_t char_size_):
    Widget (x, y, w, h),
    txt (),
    char_size (char_size_),
    cursor_pos (0),
    timer (0),
    editing (false),
    cursor_visible (false)
    {}


void EditBox::render(RenderTargetI* rt) {

}

void EditBox::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRect_rs (GetBounds(), Color(255, 255, 255), to_draw);
    if (txt.size() > 0) rt.DrawText_rs (getPos() + Vec2(10, 10), txt.c_str(), char_size, Color(0, 0, 0), to_draw);
    DrawCursor_rs(rt, to_draw);
}

void EditBox::DrawCursor_rs (RenderTarget& rt, const RegionSet* to_draw) {
    double cursor_x = rt.GetTxtSize(txt.c_str(), char_size, cursor_pos).x + getPos().x + 11;
    Color cursor_col = cursor_visible ? Color (0, 0, 128) : Color (255, 255, 255);
    rt.DrawRect_rs (Rect (cursor_x, getPos().y + 10, 2, char_size), cursor_col, to_draw);
}

bool EditBox::onMousePress (MouseContext context) {
    if ( MouseOnWidget(context.position) && !editing) editing = true;
    if (!MouseOnWidget(context.position) &&  editing) {
        editing = false;
        if (cursor_visible) {
            cursor_visible = false;
            DrawCursor_rs(*GetRendertarget(), GetRegset());
        }
    }
    return false;
}

bool EditBox::onKeyboardPress (KeyboardContext context) {
    if (!editing) return false;
    switch (context.key) {
    
        case Key::Enter:
            if (!context.shift) {
                editing = false;
                if (cursor_visible) {
                    cursor_visible = false;
                    DrawCursor_rs(*GetRendertarget(), GetRegset());
                }
            }
            break;
        
        case Key::Backspace:
            if (txt.size() > 0 && cursor_pos > 0) txt.erase (--cursor_pos, 1);
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;

        case Key::Delete:
            if (txt.size() > 0 && cursor_pos < txt.size()) txt.erase (cursor_pos, 1);
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;
        
        case Key::Right:
            if (cursor_pos < txt.size()) cursor_pos++;
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;
        
        case Key::Left:
            if (cursor_pos > 0) cursor_pos--;
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;

        default:
            char c = GetSymb(context);
            if (c != 0) {
                txt.insert (cursor_pos++, 1, c);
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;
    }
    return false;
}

bool EditBox::onClock (uint64_t delta) {
    if (!editing) return false;
    timer += delta;
    timer %= 2000000;

    if (timer < 1000000) {
        if (cursor_visible) {
            cursor_visible = false;
            DrawCursor_rs (*GetRendertarget(), GetRegset());
        }
    }
    else {
        if (!cursor_visible) {
            cursor_visible = true;
            DrawCursor_rs (*GetRendertarget(), GetRegset());
        }
    }
    return false;
}


FloatNumEditBox::FloatNumEditBox (double x, double y, double w, double h, size_t char_size):
    EditBox (x, y, w, h, char_size),
    point_is_set (false),
    minus_is_set (false)
    {}

bool FloatNumEditBox::onKeyboardPress (KeyboardContext context) {
    if (!editing) return false;
    switch (context.key) {
        case Key::Enter:
            if (!context.shift) {
                editing = false;
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;
        
        case Key::Backspace:
            if (txt.size() > 0 && cursor_pos > 0) {
                if      (txt[cursor_pos - 1] == '.') point_is_set = false;
                else if (txt[cursor_pos - 1] == '-') minus_is_set = false;
                txt.erase (--cursor_pos, 1);
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;

        case Key::Delete:
            if (txt.size() > 0 && cursor_pos < txt.size()) {
                if      (txt[cursor_pos] == '.') point_is_set = false;
                else if (txt[cursor_pos] == '-') minus_is_set = false;
                txt.erase (cursor_pos, 1);
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;
        
        case Key::Right:
            if (cursor_pos < txt.size()) cursor_pos++;
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;
        
        case Key::Left:
            if (cursor_pos > 0) cursor_pos--;
            RenderInRegset (*GetRendertarget(), GetRegset());
            break;

        case Key::Period:
            if (!point_is_set && !context.shift) {
                txt.insert (cursor_pos++, 1, '.');
                point_is_set = true;
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;

        default:
            char c = GetSymb(context);
            if (c >= '0' && c <= '9' && !(minus_is_set && cursor_pos == 0)) {
                txt.insert (cursor_pos++, 1, c);
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            if (c == '-' && cursor_pos == 0 && !minus_is_set) {
                txt.insert (cursor_pos++, 1, c);
                minus_is_set = true;
                RenderInRegset (*GetRendertarget(), GetRegset());
            }
            break;
    }
    return false;
}

double FloatNumEditBox::TextToDouble() const {
    return atof (txt.c_str());
}

const char SHIFT_NUMS[] = ")!@#$%^&*(";
const char SYMBS[]       = "[];,.\'/\\`=- ";
const char SHIFT_SYMBS[] = "{}:<>\"?|~+_ ";


char GetSymb (const KeyboardContext context) {
    Key key = context.key;
    char ret = 0;

    if (key >= Key::A && key <= Key::Z) {
        ret = 'a' + (int)key - (int)Key::A;
        if (context.shift) ret += 'A' - 'a';
    }
    else if (key >= Key::Num0 && key <= Key::Num9) {
        if (context.shift) ret = SHIFT_NUMS[(int)key - (int)Key::Num0];
        else ret = '0' + (int)key - (int)Key::Num0;
    }
    else if (key >= Key::LBracket && key <= Key::Space) {
        if (context.shift) ret = SHIFT_SYMBS[(int)key - (int)Key::LBracket];
        else ret = SYMBS[(int)key - (int)Key::LBracket];
    }
    else if (key == Key::Tab) ret = '\t';
    else if (key == Key::Enter && context.shift) ret = '\n';

    return ret;
}