#include "widget.h"

/*
void Mouse::Press (const Vec& pos_, MouseButton btn) {
    btns[btn] = MBTN_PRESSED;
    pos = pos_;
}

void Mouse::Release (const Vec& pos_, MouseButton btn) {
    btns[btn] = MBTN_RELEASED;
    pos = pos_;
}

void Mouse::Move (const Vec& pos_) {
    for (int i = 0; i < NUM_OF_MBUTONS; i++) {
        if (btns[i] == MBTN_PRESSED ) btns[i] = MBTN_DOWN;
        if (btns[i] == MBTN_RELEASED) btns[i] = MBTN_UP;
    }
    pos = pos_;
}*/

Widget::Widget():
    pos (0, 0),
    size (0, 0),
    parent (nullptr),
    subwidgets (),
    regset ()
    {}

Widget::Widget (int x, int y, int w, int h):
    pos (x, y),
    size (w, h),
    parent (nullptr),
    subwidgets (),
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
    SubtractRegset(wid -> regset);
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


void Widget::SubtractRegset (const RegionSet& regions) {
    regset -= regions;
    subwidgets.SubtractRegset(regions);
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

void Widget::Show() {
    RegionSet regs;
    regs.AddRegion (Rect(pos, pos + size));
    UpdateRegset(regs);

    if (parent != nullptr) {        
        parent -> subwidgets.MoveToTail(this);

    }

}

void Widget::UpdateRegset(const RegionSet& regs) {
    RegionSet to_draw;
    to_draw += regs;

    ListNode<Widget*>* node = subwidgets.widgets.GetHead();
    while (node != subwidgets.widgets.EndOfList()) {
        Rect region (node -> val -> pos, node -> val -> pos + node -> val -> size);
        to_draw.SubtractRegion (region);
    }

    RegionSet newregs;
    newregs += to_draw;

    to_draw -= regset;
    Render(*rt, &to_draw);
    regset.regions.Clear();
    regset += newregs;

    subwidgets.UpdateRegset();
}


WidgetManager::WidgetManager ():
    widgets()
    {}


void WidgetManager::AddWidget (Widget *wid) {
    widgets.InsertTail(wid);
}

size_t WidgetManager::GetSize() const {
    widgets.GetSize();
}

void WidgetManager::Render (RenderTarget& rt, RegionSet *to_draw) const {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> Render(rt, to_draw);
        node = node -> next;
    }
}

void WidgetManager::Move (const Vec& vec) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> Move(vec);
        node = node -> next;
    }
}

void WidgetManager::SubtractRegset (const RegionSet& regions) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> SubtractRegset(regions);
        node = node -> next;
    }
}

void WidgetManager::SetRenderTarget (RenderTarget *rt_) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> SetRenderTarget(rt_);
        node = node -> next;
    }
}

void WidgetManager::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        if (node -> val -> MouseOnWidget(mousepos)) node -> val -> MousePress(mousepos, mousebtn);
        node = node -> next;
    }
}

void WidgetManager::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> MouseRelease(mousepos, mousebtn);
        node = node -> next;
    }
}

void WidgetManager::MouseMove (const Vec& mousepos) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> MouseMove(mousepos);
        node = node -> next;
    }
}

void WidgetManager::Remove (Widget* wid) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        if (node -> val == wid) {
            widgets.Remove(node);
            break;
        }
        node = node -> next;
    }
}

void WidgetManager::UpdateRegset() {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        RegionSet regs;
        Rect region (node -> val -> pos, node -> val -> pos + node -> val -> size);
        regs.AddRegion (region);

        ListNode<Widget*>* node2 = node -> next;
        while (node2 != widgets.EndOfList()) {
            Rect to_sub (node2 -> val -> pos, node2 -> val -> pos + node2 -> val -> size);
            regs.SubtractRegion(to_sub);
            node2 = node2 -> next;
        }

        node -> val -> UpdateRegset(regs);
        node = node -> next;
    }
}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y, w, h),
    is_moving (false)
    {
        regset.AddRegion(Rect(pos, pos + size));
    }

void Window::Render (RenderTarget& rt, RegionSet* to_draw) const {
    Rect rect (pos, pos + size);
    rt.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.vert1 += Vec (2, 20);
    rect.vert2 -= Vec (2, 2);
    rt.DrawRect (rect, WINDOW_BG_COLOR, to_draw);
}

void Window::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    if (MouseOnWidget(mousepos)) {
        if (Rect(pos, pos + Vec(size.x, 30)).Contains(mousepos) && mousebtn == MOUSE_LEFT) {
                is_moving = true;
                hold_pos = mousepos;
            }
        Show(); // ???
    }
    subwidgets.MousePress (mousepos, mousebtn);
}

void Window::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    if (mousebtn == MOUSE_LEFT && is_moving) {
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

bool Window::MouseOnWidget (const Vec& mousepos) const {
    return regset.Contains(mousepos);
}
