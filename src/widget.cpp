#include "widget.h"

Widget::Widget():
    bounds (),
    parent (nullptr),
    subwidgets (),
    regset ()
    {}

Widget::Widget (int x, int y, int w, int h):
    bounds (x, y, w, h),
    parent (nullptr),
    subwidgets (),
    regset ()
    {}

void Widget::SetRenderTarget (RenderTarget *rt_) {
    rt = rt_;
    subwidgets.SetRenderTarget(rt_);
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (GetPos());
    wid -> parent = this;
    wid -> SetRenderTarget (rt);
    SubtractRegset(wid -> regset);
    subwidgets.AddWidget (wid);
}

void Widget::RenderSubWidgets (RenderTarget& rt) const {
    subwidgets.Render(rt);
}

void Widget::Move (const Vec& vec) {
    bounds.Move(vec);
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
    dst -> AddRegion (bounds);
}

bool Widget::MouseOnWidget (const Vec& mousepos) const {
    return regset.Contains(mousepos);
}

bool Widget::MouseOnSubwidgets(const Vec& mousepos) const {
    return subwidgets.MouseOnWidgets(mousepos);
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
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        delete (node -> val);
        widgets.Iterate(node);
    }
}

inline void WidgetManager::AddWidget (Widget *wid) {
    widgets.InsertTail(wid);
}

size_t WidgetManager::GetSize() const {
    return widgets.GetSize();
}

void WidgetManager::Render (RenderTarget& rt) const {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        Widget* wid = node -> val;
        wid -> Render(rt, wid -> GetRegset());
        wid -> RenderSubWidgets (rt);
        widgets.Iterate(node);
    }
}

void WidgetManager::Move (const Vec& vec) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> Move(vec);
        widgets.Iterate(node);
    }
}

void WidgetManager::SubtractRegset (const RegionSet& regions) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> SubtractRegset(regions);
        widgets.Iterate(node);
    }
}

void WidgetManager::SetRenderTarget (RenderTarget *rt_) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> SetRenderTarget(rt_);
        widgets.Iterate(node);
    }
}

void WidgetManager::MousePress (const MouseState& mstate) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> MousePress(mstate);
        widgets.Iterate(node);
    }
}

void WidgetManager::MouseRelease (const MouseState& mstate) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> MouseRelease(mstate);
        widgets.Iterate(node);
    }
}

bool WidgetManager::MouseOnWidgets (const Vec& mousepos) const {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> MouseOnWidget (mousepos)) return true;
        widgets.Iterate(node);
    }
    return false;
}

void WidgetManager::MouseMove (const MouseState& mstate) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> MouseMove(mstate);
        widgets.Iterate(node);
    }
}

void WidgetManager::MoveToTail (Widget* wid) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val == wid) {
            node -> next -> prev = node -> prev;
            node -> prev -> next = node -> next;
            node -> prev = widgets.GetTail();
            node -> next = widgets.EndOfList();
            node -> next -> prev = node;
            node -> prev -> next = node;
            break;
        }
        widgets.Iterate(node);
    }
}

void WidgetManager::UpdateRegset(const RegionSet& parent_regs) {    
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        RegionSet regs;
        node -> val -> GetMaxRegset(&regs);
        regs ^= parent_regs;

        ListNode<Widget*>* node2 = node;
        widgets.Iterate(node2);
        while (node2 != nullptr) {
            RegionSet to_sub;
            node2 -> val -> GetMaxRegset(&to_sub);
            regs -= to_sub;
            widgets.Iterate(node2);
        }

        node -> val -> UpdateRegset(regs);
        widgets.Iterate(node);
    }
}


Window::Window (int x, int y, size_t w, size_t h):
    Widget (x, y, w, h),
    is_moving (false)
    {}

void Window::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    Rect rect = GetBounds();
    rt.DrawRect (rect, Color(0, 0, 0), to_draw);
    rect.x += 2;
    rect.y += 20;
    rect.w -= 4;
    rect.h -= 22;
    rt.DrawRect (rect, WINDOW_BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(255, 0, 0, 128));
    #endif
}

void Window::MousePress (const MouseState& mstate) {
    GetSubwidgets() -> MousePress (mstate);
    if (MouseOnWidget(mstate.pos)) {
        if (Rect(GetPos().x, GetPos().y, GetSize().x, 30).Contains(mstate.pos) && mstate.btn == MOUSE_LEFT) {
                is_moving = true;
                hold_pos = mstate.pos;
            }
        Show();
    }
}

void Window::MouseRelease (const MouseState& mstate) {
    if (mstate.btn == MOUSE_LEFT && is_moving) {
        is_moving = false;
        hold_pos = Vec (0, 0);
    }
    GetSubwidgets() -> MouseRelease (mstate);
}

void Window::MouseMove (const MouseState& mstate) {
    if (is_moving) {
        Vec mousepos = mstate.pos;
        if (mousepos.x != hold_pos.x || mousepos.y != hold_pos.y) {
            Move (mousepos - hold_pos);
            //regset.regions.Clear();
            Show();
            Render(*GetRendertarget(), GetRegset());
            RenderSubWidgets(*GetRendertarget());
            hold_pos = mousepos;
        }
    }
    GetSubwidgets() -> MouseMove (mstate);
}


Background::Background (double w_, double h_):
    Widget (0, 0, w_, h_)
    {}

void Background::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawRect (GetBounds(), BG_COLOR, to_draw);

    #ifdef REGDEBUG
    rt.DrawRegset(*to_draw, Color(0, 0, 255, 128));
    #endif
}

void Background::MousePress (const MouseState& mstate) {
    GetSubwidgets() -> MousePress (mstate);
}

void Background::MouseRelease (const MouseState& mstate) {
    GetSubwidgets() -> MouseRelease (mstate);
}

void Background::MouseMove (const MouseState& mstate) {
    GetSubwidgets() -> MouseMove (mstate);
}
