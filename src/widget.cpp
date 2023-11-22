#include "widget.h"

WidgetShell::WidgetShell() {};

WidgetShell::WidgetShell(WidgetI* wid):
        external (wid),
        internal ((wid != nullptr && !wid -> isExtern()) ? dynamic_cast<Widget*>(wid) : nullptr) {}

Widget::Widget():
    bounds (),
    parent (nullptr),
    subwidgets (),
    regset (),
    rt (nullptr),
    visible (true)
    {}

Widget::Widget (double x, double y, double w, double h):
    bounds (x, y, w, h),
    parent (nullptr),
    subwidgets (),
    regset (),
    rt (nullptr),
    visible (true)
    {}

Widget::~Widget() {
    if (parent.external != nullptr) {
        parent.external -> unregisterSubWidget(this);
    }
    if (parent.internal != nullptr) {
        parent.internal -> UpdateAllRegsets();
    }
}

void Widget::registerSubWidget(WidgetI* wid) {
    wid -> move(getPos());
    wid -> setParent(this);
    subwidgets.AddWidget (wid);
    if (!wid -> isExtern()) dynamic_cast<Widget*>(wid) -> SetRenderTarget(rt);
    UpdateAllRegsets();
}

void Widget::unregisterSubWidget(WidgetI* wid) {
    subwidgets.RemoveWidget(wid);
}

void Widget::setSize(Vec2 new_size) {
    bounds.w = new_size.x;
    bounds.h = new_size.y;
}

void Widget::setPos(Vec2 new_pos) {
    bounds.x = new_pos.x;
    bounds.y = new_pos.y;
}

void Widget::setParent(WidgetI *root) {
    parent = WidgetShell(root);
}

WidgetI* Widget::getParent() {
    return parent.external;
}

void Widget::move(Vec2 shift) {
    bounds.Move(shift);
    regset.Move(shift);
    subwidgets.Move(shift);
}

void Widget::SetRenderTarget (RenderTarget *rt_) {
    rt = rt_;
    subwidgets.SetRenderTarget(rt_);
}

void Widget::RenderSubWidgets (RenderTarget& rt) const {
    subwidgets.Render(rt);
}

void Widget::UpdateAllRegsets() {
    if (rt == nullptr) return;
    Widget* w_ptr = this;
    while (w_ptr -> parent.internal != nullptr) {w_ptr = w_ptr -> parent.internal;}
    RegionSet regs;
    GetMaxRegset(&regs);
    UpdateRegset(regs);
}

void Widget::Show() {
    if (parent.internal != nullptr) {
        parent.internal -> subwidgets.MoveToTail(this);
        parent.internal -> Show();
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

bool Widget::MouseOnWidget (const Vec2& mousepos) const {
    return regset.Contains(mousepos);
}

bool Widget::MouseOnSubwidgets(const Vec2& mousepos) const {
    return subwidgets.MouseOnWidgets(mousepos);
}

void Widget::UpdateRegset(const RegionSet& regs) {
    RegionSet newregs;
    if (visible) {
        RegionSet to_draw;
        to_draw += regs;

        ListNode<WidgetShell>* node = subwidgets.widgets.GetHead();
        while (node != subwidgets.widgets.EndOfList()) {
            if (node -> val.internal != nullptr) {
                RegionSet child_max_regset;
                node -> val.internal -> GetMaxRegset(&child_max_regset);
                to_draw -= child_max_regset;
            }
            node = node -> next;
        }

        newregs += to_draw;
        to_draw -= regset;

        #ifndef REGDEBUG
        if (to_draw.regions.GetSize() > 0) RenderInRegset(*rt, &to_draw);
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
    widgets ()
    {}

WidgetManager::~WidgetManager() {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> setParent (nullptr);
        delete (node -> val.external);
        widgets.Iterate(node);
    }
}

inline void WidgetManager::AddWidget (WidgetI* wid) {
    widgets.InsertTail(WidgetShell(wid));
}

inline void WidgetManager::RemoveWidget (WidgetI* wid) {
    widgets.Remove(widgets.GetNode(WidgetShell(wid)));
}

size_t WidgetManager::GetSize() const {
    return widgets.GetSize();
}

void WidgetManager::Render (RenderTarget& rt) const {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.internal != nullptr) {
            Widget* wid = node -> val.internal;
            if (wid -> getAvailable()) {
                wid -> RenderInRegset(rt, wid -> GetRegset());
                wid -> RenderSubWidgets(rt);
            }
            else node -> val.external -> render(&rt);
        }
        widgets.Iterate(node);
    }
}

void WidgetManager::Move (const Vec2& vec) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> move(vec);
        widgets.Iterate(node);
    }
}

void WidgetManager::SetRenderTarget (RenderTarget *rt_) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.internal != nullptr) node -> val.internal -> SetRenderTarget(rt_);
        widgets.Iterate(node);
    }
}

bool WidgetManager::MouseOnWidgets (const Vec2& mousepos) const {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.internal != nullptr &&
            node -> val.internal -> MouseOnWidget (mousepos)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onMousePress(MouseContext context) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        ListNode<WidgetShell>* nextnode = node;
        widgets.Iterate (nextnode);
        node -> val.external -> onMousePress(context);
        node = nextnode;
    }
    return false;
}

bool WidgetManager::onMouseRelease (MouseContext context) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> onMouseRelease(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onMouseMove (MouseContext context) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> onMouseMove(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardPress (KeyboardContext context) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> onKeyboardPress(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardRelease (KeyboardContext context) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> onKeyboardRelease(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onClock (uint64_t delta) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val.external -> onClock(delta);
        widgets.Iterate(node);
    }
    return false;
}

void WidgetManager::MoveToTail (WidgetI* wid) {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.external == wid) {
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
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.internal != nullptr)
        {
            RegionSet regs;
            node -> val.internal -> GetMaxRegset(&regs);
            regs ^= parent_regs;

            ListNode<WidgetShell>* node2 = node;
            widgets.Iterate(node2);
            while (node2 != nullptr) {
                if (node2 -> val.internal) {
                    RegionSet to_sub;
                    node2 -> val.internal -> GetMaxRegset(&to_sub);
                    regs -= to_sub;
                }
                widgets.Iterate(node2);
            }

            node -> val.internal -> UpdateRegset(regs);
        }
        widgets.Iterate(node);
    }
}

void WidgetManager::GetMaxRegset (RegionSet* dst) const {
    ListNode<WidgetShell>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val.internal != nullptr) node -> val.internal -> GetMaxRegset(dst);
        widgets.Iterate(node);
    }
}


// TxtWidget::TxtWidget (double x, double y, double w, double h,
//                       const std::string& str_, const Font& fnt_, size_t char_size_):
//     Widget (x, y, w, h),
//     txt (str_, fnt_, char_size_)
//     {}

// void TxtWidget::Render (RenderTarget& rt, const RegionSet* to_draw) const {
//     rt.DrawText (txt, GetPos(), Color (0, 0, 0), to_draw);
// }
