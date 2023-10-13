#include "widget.h"

Widget::Widget():
    pos (0, 0),
    visible (true),
    parent (nullptr),
    subwidgets (),
    regset ()
    {}

Widget::Widget (int x, int y, int w, int h, size_t subw_cap):
    pos (x, y),
    size (w, h),
    visible (true),
    parent (nullptr),
    subwidgets (subw_cap),
    regset ()
    {}

void Widget::SetRenderTarget (RenderTarget *rt_) {
    rt = rt_;
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> pos += pos;
    wid -> parent = this;
    wid -> SetRenderTarget (rt);
    subwidgets.AddWidget (wid);
}

void Widget::RenderSubWidgets (RenderTarget& screen) const {
    subwidgets.Render(screen);
}

void Widget::Move (const Vec& vec) {
    pos += vec;
    regset.Move(vec);
    subwidgets.Move(vec);
}

void Widget::UpdateRegSet (const Rect& movingwindow, Widget* no_update) {
    RegionSet newregs;
    newregs.AddRegion (Rect(pos, pos + size));
    newregs.SubtractRegion(movingwindow);
    newregs -= regset;

    Render (*rt, newregs);

    regset.AddRegion (Rect (pos, pos + size));

    for (size_t i = 0; i < subwidgets.GetSize(); i++) {
        Widget* wid = subwidgets[i];
        newregs.SubtractRegion (Rect (wid -> pos, wid -> pos + wid -> size));
        if (wid != no_update) wid -> UpdateRegSet (movingwindow, no_update);
    }
}

void Widget::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    MousePressAction (mousepos, mousebtn);
    subwidgets.MousePress (mousepos, mousebtn);
}

void Widget::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    MouseReleaseAction (mousepos, mousebtn);
    subwidgets.MouseRelease (mousepos, mousebtn);
}

void Widget::MouseMove (const Vec& mousepos) {
    MouseMoveAction (mousepos);
    subwidgets.MouseMove (mousepos);
}


WidgetManager::WidgetManager (size_t cap):
    size (0),
    capacity (cap)
    {
        widgets = (Widget**) calloc (capacity, sizeof (widgets[0]));
        assert (widgets != nullptr);
    }

WidgetManager::~WidgetManager() {
    for (size_t i = 0; i < size; i++) {
        delete widgets[i];
    }
    free (widgets);
}

void WidgetManager::AddWidget (Widget *wid) {
    if (size >= capacity) {
        widgets = (Widget**) Recalloc (widgets, capacity * 2, sizeof (widgets[0]), capacity);
        assert (widgets != nullptr);
        capacity *= 2;
    }
    widgets[size++] = wid;
}

size_t WidgetManager::GetSize() const {
    return size;
}

Widget* WidgetManager::operator[] (size_t index) {
    assert (index < size);
    return widgets[index];
}

void WidgetManager::Render (RenderTarget& screen) const {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Render (screen, widgets[i] -> regset);
        widgets[i] -> RenderSubWidgets(screen);
    }
}

void WidgetManager::Move (const Vec& vec) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Move (vec);
    }
}

void WidgetManager::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    for (size_t i = 0; i < size; i++) {
        if (widgets[i] -> MouseOnWidget (mousepos))
            widgets[i] -> MousePress (mousepos, mousebtn);
    }
}

void WidgetManager::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> MouseRelease (mousepos, mousebtn);
    }
}

void WidgetManager::MouseMove (const Vec& mousepos) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> MouseMove (mousepos);
    }
}



Window::Window (size_t w, size_t h):
    Widget (0, 0, w, h),
    is_moving (false)
    {}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y, w, h),
    is_moving (false)
    {}


void Window::Render (RenderTarget& screen, const RegionSet& to_draw) const {
    Rect rect (pos, pos + size);
    screen.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.vert1 += Vec (2, 20);
    rect.vert2 -= Vec (2, 2);
    screen.DrawRect (rect, WINDOW_BG_COLOR, to_draw);
}

void Window::MousePressAction (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x &&
        mousepos.y >= pos.y && mousepos.y <= pos.y + 30 && mousebtn == MOUSE_LEFT) {
            is_moving = true;
            hold_pos = mousepos;
        }
    if (MouseOnWidget(mousepos)) Show();
}

void Window::MouseReleaseAction (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousebtn == MOUSE_LEFT) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
}

void Window::MouseMoveAction (const Vec& mousepos) {
    if (is_moving) {
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            Move (mousepos - hold_pos);
            if (parent != nullptr) parent -> UpdateRegSet(Rect (pos, size), this);
            Render(*rt, regset);
            RenderSubWidgets(*rt);

            hold_pos = mousepos;
        }
    }
}

bool Window::MouseOnWidget (const Vec& mousepos) {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + size.y);
}



Button::Button (double x, double y, size_t w, size_t h):
    Widget (x, y, w, h, 0),
    width (w),
    height (h),
    state (BTN_NORMAL)
    {
        regset.AddRegion (Rect(Vec(x, y), Vec(x + w, y + h)));
    }


bool Button::MouseOnWidget (const Vec& mousepos) {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + width ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + height);
}

void Button::MouseMoveAction (const Vec& mousepos) {
    if (state == BTN_DISABLED) return;

    if (MouseOnWidget (mousepos)) {
        if (state == BTN_NORMAL) state = BTN_FOCUSED;
    }
    else {
        if (state == BTN_FOCUSED) state = BTN_NORMAL;
    }
}


ImgButton::ImgButton (double x, double y, size_t w, size_t h, const Texture* textures_):
    Button (x, y, w, h)
    {
        if (textures_ != nullptr) {
            textures[0] = textures_[0];
            textures[1] = textures_[1];
            textures[2] = textures_[2];
            textures[3] = textures_[3];
        }
    }

void ImgButton::SetTextures (const Texture* textures_) {
    assert (textures != nullptr);

    textures[0] = textures_[0];
    textures[1] = textures_[1];
    textures[2] = textures_[2];
    textures[3] = textures_[3];
}


void ImgButton::Render (RenderTarget& screen, const RegionSet& to_draw) const {
    screen.DrawTexture (textures[state], pos, Vec(width, height), to_draw);
}


/*
TxtButton::TxtButton (double x, double y, size_t w, size_t h, const Text& txt_):
    Button (x, y, w, h),
    txt (txt_)
    {
        regset.AddRegion (Rect(Vec(x, y), Vec(x + w, y + h)));
    }

void TxtButton::SetText (const Text& txt_) {
    
}*/



void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num) {
    memptr = realloc (memptr, num * size);
    if (memptr == nullptr) return nullptr;

    if (num > old_num) memset ((void *) ((char *) memptr + old_num * size), 0, (num - old_num) * size);

    return memptr;
}
