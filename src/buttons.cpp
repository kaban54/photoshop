#include "buttons.h"

Button::Button (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_):
    Widget (x, y, w, h),
    state (BTN_NORMAL),
    action (action_),
    action_args (action_args_)
    {}

bool Button::onMousePress (MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        state = BTN_PRESSED;
        action(action_args);
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Button::onMouseRelease (MouseContext context) {
    if (state == BTN_PRESSED) {
        if (MouseOnWidget(context.position)) state = BTN_FOCUSED;
        else                                 state = BTN_NORMAL;

        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

bool Button::onMouseMove (MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        if (state == BTN_NORMAL) {
            state = BTN_FOCUSED;
            RenderInRegset (*GetRendertarget(), GetRegset());
        }
    }
    else if (state == BTN_FOCUSED) {
        state = BTN_NORMAL;
        RenderInRegset (*GetRendertarget(), GetRegset());
    }
    return false;
}

void Button::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    DrawButton (rt, GetBounds(), state, to_draw);
}

void Button::render(RenderTargetI*) {
    
}


// ImgButton::ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_):
//     Button (x, y, w, h)
//     {
//         if (textures_ != nullptr) {
//             textures[0] = textures_[0];
//             textures[1] = textures_[1];
//             textures[2] = textures_[2];
//             textures[3] = textures_[3];
//         }
//     }

// void ImgButton::SetTextures (const Texture* textures_) {
//     assert (textures != nullptr);
//     textures[0] = textures_[0];
//     textures[1] = textures_[1];
//     textures[2] = textures_[2];
//     textures[3] = textures_[3];
// }

// void ImgButton::Render (RenderTarget& rt, const RegionSet* to_draw) const {
//     rt.DrawTexture (textures[state], GetPos(), GetSize(), to_draw);

//     #ifdef REGDEBUG
//     rt.DrawRegset(*to_draw, Color(0, rand() % 128 + 128, 0));
//     #endif
// }


TxtButton::TxtButton (double x, double y, double w, double h, BtnFunc* action_, BtnArgs* action_args_,
                      const char *str, uint16_t char_size_):
    Button (x, y, w, h, action_, action_args_),
    txt (str),
    char_size (char_size_)
    {}

void TxtButton::SetText (const char *str, uint16_t char_size_) {
    txt = str;
    char_size = char_size_;
}

void TxtButton::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    DrawButton (rt, GetBounds(), state, to_draw);
    rt.DrawText_rs (getPos() + Vec2 (40, 18), txt, char_size, Color (0, 0, 0), to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 255, 0, 128));
    #endif
}


void DrawButton (RenderTarget& rt, const Rect& rect, ButtonState state, const RegionSet* to_draw) {
    if (state == BTN_NORMAL) {
        rt.DrawRect_rs (rect, Color(128, 128, 128), to_draw);
        rt.DrawRect_rs (Rect (rect.x, rect.y, rect.w - 4, 8)     , Color (192, 192, 192), to_draw);
        rt.DrawRect_rs (Rect (rect.x, rect.y + 8, 8, rect.h - 12), Color (192, 192, 192), to_draw);
        rt.DrawRect_rs (Rect (rect.x + 4, rect.Bot() - 8, rect.w - 4, 8)   , Color (64, 64, 64), to_draw);
        rt.DrawRect_rs (Rect (rect.Right() - 8, rect.y + 4, 8, rect.h - 12), Color (64, 64, 64), to_draw);
        rt.DrawRect_rs (Rect (rect.x + 4, rect.Bot() - 8, 4, 4)  , Color (128, 128, 128), to_draw);
        rt.DrawRect_rs (Rect (rect.Right() - 8, rect.y + 4, 4, 4), Color (128, 128, 128), to_draw);
    }
    else if (state == BTN_FOCUSED) {
        rt.DrawRect_rs (rect, Color(160, 160, 160), to_draw);
        rt.DrawRect_rs (Rect (rect.x, rect.y, rect.w - 4, 8)     , Color (224, 224, 224), to_draw);
        rt.DrawRect_rs (Rect (rect.x, rect.y + 8, 8, rect.h - 12), Color (224, 224, 224), to_draw);
        rt.DrawRect_rs (Rect (rect.x + 4, rect.Bot() - 8, rect.w - 4, 8)   , Color (96, 96, 96), to_draw);
        rt.DrawRect_rs (Rect (rect.Right() - 8, rect.y + 4, 8, rect.h - 12), Color (96, 96, 96), to_draw);
        rt.DrawRect_rs (Rect (rect.x + 4, rect.Bot() - 8, 4, 4)  , Color (160, 160, 160), to_draw);
        rt.DrawRect_rs (Rect (rect.Right() - 8, rect.y + 4, 4, 4), Color (160, 160, 160), to_draw);
    }
    else if (state == BTN_PRESSED) {
        rt.DrawRect_rs (rect, Color (64, 64, 64), to_draw);
        rt.DrawRect_rs (Rect (rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8), Color (128, 128, 128), to_draw);
    }
}