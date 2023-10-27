#include "widget.h"

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

void Widget::Show() {
    if (parent != nullptr) {     
        parent -> subwidgets.MoveToTail(this);
        parent -> Show();
    }
    else {
        RegionSet regs;
        GetMaxRegset(&regs);
        UpdateRegset(regs);
    }
}

void Widget::GetMaxRegset (RegionSet* dst) const {
    assert(dst != nullptr);
    dst -> AddRegion (Rect(pos, pos + size));
}

bool Widget::MouseOnWidget (const Vec& mousepos) const {
    return regset.Contains(mousepos);
}

void Widget::UpdateRegset(const RegionSet& regs) {
    RegionSet to_draw;
    to_draw += regs;

    ListNode<Widget*>* node = subwidgets.widgets.GetHead();
    while (node != subwidgets.widgets.EndOfList()) {
        RegionSet child_max_regset;
        node -> val -> GetMaxRegset(&child_max_regset);
        to_draw -= child_max_regset;
        node = node -> next;
    }

    RegionSet newregs;
    newregs += to_draw;
    to_draw -= regset;
    
    #ifndef REGDEBUG
    if (to_draw.regions.GetSize() > 0) Render(*rt, &to_draw);
    #else
    Render (*rt, &newregs);
    // rt -> DrawRegset (regset, Color (0, 0, 255, 128));
    // rt -> DrawRegset (to_draw, Color (255, 0, 0, 128));
    #endif

    regset.regions.Clear();
    regset += newregs;
    subwidgets.UpdateRegset(regs);
}


WidgetManager::WidgetManager ():
    widgets()
    {}

WidgetManager::~WidgetManager() {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        delete (node -> val);
        node = node -> next;
    }
}

void WidgetManager::AddWidget (Widget *wid) {
    widgets.InsertTail(wid);
}

size_t WidgetManager::GetSize() const {
    return widgets.GetSize();
}

void WidgetManager::Render (RenderTarget& rt) const {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        node -> val -> Render(rt, &(node -> val -> regset));
        node -> val -> RenderSubWidgets (rt);
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
        node -> val -> MousePress(mousepos, mousebtn);
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

bool WidgetManager::MouseOnWidgets (const Vec& mousepos) const {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        if (node -> val -> MouseOnWidget (mousepos)) return true;
        node = node -> next;
    }
    return false;
}

void WidgetManager::MouseMove (const Vec& mousepos) {
    ListNode<Widget*>* node = widgets.GetHead();
    ListNode<Widget*>* end_of_list = widgets.EndOfList();
    while (node != end_of_list) {
        node -> val -> MouseMove(mousepos);
        node = node -> next;
    }
}

void WidgetManager::MoveToTail (Widget* wid) {
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        if (node -> val == wid) {
            node -> next -> prev = node -> prev;
            node -> prev -> next = node -> next;
            node -> prev = widgets.GetTail();
            node -> next = widgets.EndOfList();
            node -> next -> prev = node;
            node -> prev -> next = node;
            break;
        }
        node = node -> next;
    }
}

void WidgetManager::UpdateRegset(const RegionSet& parent_regs) {    
    ListNode<Widget*>* node = widgets.GetHead();
    while (node != widgets.EndOfList()) {
        RegionSet regs;
        node -> val -> GetMaxRegset(&regs);
        regs ^= parent_regs;

        ListNode<Widget*>* node2 = node -> next;
        while (node2 != widgets.EndOfList()) {
            RegionSet to_sub;
            node2 -> val -> GetMaxRegset(&to_sub);
            regs -= to_sub;
            node2 = node2 -> next;
        }

        node -> val -> UpdateRegset(regs);
        node = node -> next;
    }
}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y, w, h),
    is_moving (false)
    {}

void Window::Render (RenderTarget& rt, RegionSet* to_draw) const {
    Rect rect (pos, pos + size);
    rt.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.vert1 += Vec (2, 20);
    rect.vert2 -= Vec (2, 2);
    rt.DrawRect (rect, WINDOW_BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(255, 0, 0, 128));
    #endif
}

void Window::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    subwidgets.MousePress (mousepos, mousebtn);
    if (MouseOnWidget(mousepos)) {
        if (Rect(pos, pos + Vec(size.x, 30)).Contains(mousepos) && mousebtn == MOUSE_LEFT) {
                is_moving = true;
                hold_pos = mousepos;
            }
        Show();
    }
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
            Move (mousepos - hold_pos);
            regset.regions.Clear();
            Show();
            Render(*rt, &regset);
            RenderSubWidgets(*rt);
            hold_pos = mousepos;
        }
    }
    subwidgets.MouseMove (mousepos);
}
