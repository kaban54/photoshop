#include "widget.h"

Widget::Widget():
    bounds (),
    parent (nullptr),
    subwidgets (),
    regset (),
    rt (nullptr)
    {}

Widget::Widget (double x, double y, double w, double h):
    bounds (x, y, w, h),
    parent (nullptr),
    subwidgets (),
    regset (),
    rt (nullptr)
    {}

Widget::~Widget() {
    if (parent != nullptr) {
        parent -> subwidgets.RemoveWidget (this);
        parent -> UpdateAllRegsets();
    }
}

void Widget::SetRenderTarget (RenderTarget *rt_) {
    rt = rt_;
    subwidgets.SetRenderTarget(rt_);
}

void Widget::AddSubWidget (Widget* wid) {
    wid -> Move (GetPos());
    wid -> parent = this;
    wid -> SetRenderTarget (rt);
    subwidgets.AddWidget (wid);
    UpdateAllRegsets();
}

void Widget::RenderSubWidgets (RenderTarget& rt) const {
    subwidgets.Render(rt);
}

void Widget::Move (const Vec& vec) {
    bounds.Move(vec);
    regset.Move(vec);
    subwidgets.Move(vec);
}

void Widget::UpdateAllRegsets() {
    if (rt == nullptr) return;
    Widget* w_ptr = this;
    while (w_ptr -> parent != nullptr) {w_ptr = w_ptr -> parent;}
    RegionSet regs;
    GetMaxRegset(&regs);
    UpdateRegset(regs);
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
    if (!visible) return;
    dst -> AddRegion (bounds);
    subwidgets.GetMaxRegset (dst);
}

bool Widget::MouseOnWidget (const Vec& mousepos) const {
    return regset.Contains(mousepos);
}

bool Widget::MouseOnSubwidgets(const Vec& mousepos) const {
    return subwidgets.MouseOnWidgets(mousepos);
}

void Widget::UpdateRegset(const RegionSet& regs) {
    RegionSet newregs;
    if (visible) {
        RegionSet to_draw;
        to_draw += regs;

        ListNode<Widget*>* node = subwidgets.widgets.GetHead();
        while (node != subwidgets.widgets.EndOfList()) {
            RegionSet child_max_regset;
            node -> val -> GetMaxRegset(&child_max_regset);
            to_draw -= child_max_regset;
            node = node -> next;
        }

        newregs += to_draw;
        to_draw -= regset;

        #ifndef REGDEBUG
        if (to_draw.regions.GetSize() > 0) Render(*rt, &to_draw);
        #else
        Render (*rt, &newregs);
        // rt -> DrawRegset (regset, Color (0, 0, 255, 128));
        rt -> DrawRegset (to_draw, Color (255, 0, 0, 128), true);
        #endif
    }
    regset.regions.Clear();
    regset += newregs;

    if (visible) subwidgets.UpdateRegset(regs);
    else         subwidgets.UpdateRegset(newregs);
}


WidgetManager::WidgetManager ():
    widgets()
    {}

WidgetManager::~WidgetManager() {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> SetParent (nullptr);
        delete (node -> val);
        widgets.Iterate(node);
    }
}

inline void WidgetManager::AddWidget (Widget* wid) {
    widgets.InsertTail(wid);
}

inline void WidgetManager::RemoveWidget (Widget* wid) {
    widgets.Remove(widgets.GetNode(wid));
}

size_t WidgetManager::GetSize() const {
    return widgets.GetSize();
}

void WidgetManager::Render (RenderTarget& rt) const {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        Widget* wid = node -> val;
        if (wid -> visible) {
            wid -> Render(rt, wid -> GetRegset());
            wid -> RenderSubWidgets (rt);
        }
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
        ListNode<Widget*>* nextnode = node;
        widgets.Iterate (nextnode);
        node -> val -> MousePress(mstate);
        node = nextnode;
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

void WidgetManager::KeyboardPress (const KeyboardState& kstate) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> KeyboardPress(kstate);
        widgets.Iterate(node);
    }
}

void WidgetManager::KeyboardRelease (const KeyboardState& kstate) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> KeyboardRelease(kstate);
        widgets.Iterate(node);
    }
}

void WidgetManager::TimerEvent (double time) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> TimerEvent(time);
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

void WidgetManager::GetMaxRegset (RegionSet* dst) const {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> GetMaxRegset(dst);
        widgets.Iterate(node);
    }
}


TxtWidget::TxtWidget (double x, double y, double w, double h,
                      const std::string& str_, const Font& fnt_, size_t char_size_):
    Widget (x, y, w, h),
    txt (str_, fnt_, char_size_)
    {}

void TxtWidget::Render (RenderTarget& rt, const RegionSet* to_draw) const {
    rt.DrawText (txt, GetPos(), Color (0, 0, 0), to_draw);
}
