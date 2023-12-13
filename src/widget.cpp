#include "widget.h"

Widget::Widget():
    bounds (),
    subwidgets (),
    regset (),
    parent (nullptr),
    rt (nullptr),
    visible (true)
    {}

Widget::Widget (double x, double y, double w, double h):
    bounds (x, y, w, h),
    subwidgets (),
    regset (),
    parent (nullptr),
    rt (nullptr),
    visible (true)
    {}

Widget::~Widget() {
    if (parent != nullptr) {
        parent -> unregisterSubWidget(this);
        parent -> UpdateAllRegsets();
    }
}

void Widget::registerSubWidget(WidgetI* wid_i) {
    Widget* wid = dynamic_cast<Widget*>(wid_i);
    wid -> move(getPos());
    wid -> setParent(this);
    subwidgets.AddWidget (wid);
    wid -> SetRenderTarget(rt);
    UpdateAllRegsets();
}

void Widget::unregisterSubWidget(WidgetI* wid) {
    subwidgets.RemoveWidget(dynamic_cast<Widget*>(wid));
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
    parent = dynamic_cast<Widget*>(root);
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
    if (parent != nullptr) parent -> UpdateAllRegsets();
    else {
        RegionSet regs;
        GetMaxRegset(&regs);
        UpdateRegset(regs);
    }
}

void Widget::Show() {
    if (rt == nullptr) return;
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

        ListNode<Widget*>* node = subwidgets.widgets.GetHead();
        while (node != subwidgets.widgets.EndOfList()) {
            if (node -> val != nullptr) {
                RegionSet child_max_regset;
                node -> val -> GetMaxRegset(&child_max_regset);
                to_draw -= child_max_regset;
            }
            node = node -> next;
        }

        newregs += to_draw;
        to_draw -= regset;

        if (to_draw.regions.GetSize() > 0) RenderInRegset(*rt, &to_draw);
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
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> setParent (nullptr);
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
        if (wid -> getAvailable()) {
            wid -> RenderInRegset(rt, wid -> GetRegset());
            wid -> RenderSubWidgets(rt);
        }
        widgets.Iterate(node);
    }
}

void WidgetManager::Move (const Vec2& vec) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> move(vec);
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

bool WidgetManager::MouseOnWidgets (const Vec2& mousepos) const {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> MouseOnWidget (mousepos)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onMousePress(MouseContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        ListNode<Widget*>* nextnode = node;
        widgets.Iterate (nextnode);
        node -> val -> onMousePress(context);
        node = nextnode;
    }
    return false;
}

bool WidgetManager::onMouseRelease (MouseContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> onMouseRelease(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onMouseMove (MouseContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> onMouseMove(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardPress (KeyboardContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> onKeyboardPress(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardRelease (KeyboardContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> onKeyboardRelease(context);
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onClock (uint64_t delta) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> onClock(delta);
        widgets.Iterate(node);
    }
    return false;
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


TxtWidget::TxtWidget (double x, double y, double w, double h, const char* txt_, size_t char_size_,
                      Color fill_col_, Color bg_col_):
    Widget (x, y, w, h),
    txt (txt_),
    char_size (char_size_),
    fill_col (fill_col_),
    bg_col (bg_col_)
    {}

void TxtWidget::RenderInRegset (RenderTarget& rt, const RegionSet* to_draw) {
    rt.DrawRect_rs(Rect(getPos(), getPos() + getSize()), bg_col, to_draw);
    rt.DrawText_rs(getPos(), txt, char_size, fill_col, to_draw);
}


ExternWidget::ExternWidget(PluginWidgetI* plug_wid_):
    plug_wid (plug_wid_) {}

void ExternWidget::RenderInRegset(RenderTarget& rt, const RegionSet* to_draw) {
    plug_wid -> render(&rt);
}

bool ExternWidget::onMousePress(MouseContext context) {
    return plug_wid -> onMousePress(context);
}

bool ExternWidget::onMouseRelease(MouseContext context) {
    return plug_wid -> onMouseRelease(context);
}

bool ExternWidget::onMouseMove(MouseContext context) {
    return plug_wid -> onMouseMove(context);
}

bool ExternWidget::onKeyboardPress(KeyboardContext context) {
    return plug_wid -> onKeyboardPress(context);
}

bool ExternWidget::onKeyboardRelease(KeyboardContext context) {
    return plug_wid -> onKeyboardRelease(context);
}

bool ExternWidget::onClock(uint64_t delta) {
    return plug_wid -> onClock(delta);
}

uint8_t ExternWidget::getPriority() const {
    return plug_wid -> getPriority();
}