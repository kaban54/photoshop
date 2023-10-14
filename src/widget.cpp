#include "widget.h"

Widget::Widget():
    pos (0, 0),
    size (0, 0),
    parent (nullptr),
    subwidgets (),
    regset ()
    {}

Widget::Widget (int x, int y, int w, int h, size_t subw_cap):
    pos (x, y),
    size (w, h),
    parent (nullptr),
    subwidgets (subw_cap),
    regset ()
    {}

void Widget::SetRenderTarget (RenderTarget *rt_) {
    rt = rt_;
    subwidgets.SetRenderTarget(rt_);
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (pos);
    wid -> parent = this;
    wid -> SetRenderTarget (rt);
    subwidgets.AddWidget (wid);
}

void Widget::RenderSubWidgets (RenderTarget& rt) const {
    subwidgets.Render(rt);
}

void Widget::Move (const Vec& vec) {
    pos += vec;
    regset.Move(vec);
    subwidgets.Move(vec);
}

void Widget::SetRegions (const RegionSet& regs) {
    regset.regions.Clear();
    RegionSet tmp;
    tmp.AddRegion (Rect (pos, pos + size));
    IntersectRegsets (tmp, regs, regset);

    for (size_t i = 0; i < subwidgets.GetSize(); i++) {
        Widget* wid = subwidgets[i];
        RegionSet wid_regs;
        tmp.regions.Clear();
        tmp.AddRegion (Rect (wid -> pos, wid -> pos + wid -> size));
        IntersectRegsets (tmp, regset, wid_regs);
        for (size_t j = i + 1; j < subwidgets.GetSize(); j++) {
            wid_regs.SubtractRegion(Rect (subwidgets[j] -> pos, subwidgets[j] -> pos + subwidgets[j] -> size));
        }
        wid -> SetRegions (wid_regs);

        regset.SubtractRegion (Rect (wid -> pos, wid -> pos + wid -> size));
    }
}
/*
void Widget::UpdateRegSet (const Rect& old_pos, const Rect& new_pos, Widget* no_update) {
    RegionSet newregs;
    newregs.AddRegion (old_pos);
    newregs.SubtractRegion (new_pos);
    newregs -= regset;

    Render (*rt, newregs);

    regset.AddRegion (Rect (pos, pos + size));

    for (size_t i = 0; i < subwidgets.GetSize(); i++) {
        Widget* wid = subwidgets[i];
        regset.SubtractRegion (Rect (wid -> pos, wid -> pos + wid -> size));
        if (wid != no_update) wid -> UpdateRegSet (old_pos, new_pos, no_update);
    }
}

void Widget::Show() {
    RegionSet newregs;
    newregs.AddRegion (Rect (pos, pos + size));
    newregs -= regset;

    for (size_t i = 0; i < subwidgets.GetSize(); i++) {
        Widget* wid = subwidgets[i];
        newregs.SubtractRegion (Rect (wid -> pos, wid -> pos + wid -> size));
        wid -> Show();
    }

    Render (*rt, newregs);
    regset += newregs;
}
*/
/*
void Widget::Show() {
    RegionSet tmp;
    tmp.AddRegion (Rect(pos, pos + size));
    SetRegions(tmp);
    Render (*rt, regset);
    RenderSubWidgets (*rt);
}
*/

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

Widget* WidgetManager::operator[] (size_t index) const {
    assert (index < size);
    return widgets[index];
}

void WidgetManager::Render (RenderTarget& rt) const {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Render (rt, widgets[i] -> regset);
        //widgets[i] -> RenderSubWidgets(rt);
    }
}

void WidgetManager::Move (const Vec& vec) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> Move (vec);
    }
}

void WidgetManager::SetRenderTarget (RenderTarget *rt_) {
    for (size_t i = 0; i < size; i++) {
        widgets[i] -> SetRenderTarget(rt_);
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


void Window::Render (RenderTarget& rt, const RegionSet& to_draw) const {
    Rect rect (pos, pos + size);
    rt.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.vert1 += Vec (2, 20);
    rect.vert2 -= Vec (2, 2);
    rt.DrawRect (rect, WINDOW_BG_COLOR, to_draw);

    subwidgets.Render(rt);
}

void Window::MousePress (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x &&
        mousepos.y >= pos.y && mousepos.y <= pos.y + 30 && mousebtn == MOUSE_LEFT) {
            is_moving = true;
            hold_pos = mousepos;
        }
    //if (MouseOnWidget(mousepos)) Show();

    subwidgets.MousePress (mousepos, mousebtn);
}

void Window::MouseRelease (const Vec& mousepos, MouseButtons mousebtn) {
    if (mousebtn == MOUSE_LEFT) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
    subwidgets.MouseRelease (mousepos, mousebtn);
}

void Window::MouseMove (const Vec& mousepos) {
    if (is_moving) {
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            /*
            Vec old_pos = pos;
            Move (mousepos - hold_pos);
            if (parent != nullptr)
                parent -> UpdateRegSet(Rect (old_pos, old_pos + size), Rect (pos, pos + size), this);
            Render(*rt, regset);
            RenderSubWidgets(*rt);*/

            Move (mousepos - hold_pos);
            hold_pos = mousepos;
        }
    }
    subwidgets.MouseMove (mousepos);
}

bool Window::MouseOnWidget (const Vec& mousepos) {
    return (mousepos.x >= pos.x && mousepos.x <= pos.x + size.x ) &&
           (mousepos.y >= pos.y && mousepos.y <= pos.y + size.y);
}


void *Recalloc (void *memptr, size_t num, size_t size, size_t old_num) {
    memptr = realloc (memptr, num * size);
    if (memptr == nullptr) return nullptr;

    if (num > old_num) memset ((void *) ((char *) memptr + old_num * size), 0, (num - old_num) * size);

    return memptr;
}
