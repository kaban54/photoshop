#include "widget.h"

Widget::Widget():
    bounds (),
    subwidgets (),
    regset (),
    parent (nullptr),
    rt (nullptr),
    visible (true),
    avalible (true)
    {}

Widget::Widget (double x, double y, double w, double h):
    bounds (x, y, w, h),
    subwidgets (),
    regset (),
    parent (nullptr),
    rt (nullptr),
    visible (true),
    avalible (true)
    {}

Widget::~Widget() {
    if (parent != nullptr) {
        parent -> unregisterSubWidget(this);
    }
}

void Widget::registerSubWidget(WidgetI* wid_i) {
    Widget* wid = dynamic_cast<Widget*>(wid_i);
    wid -> SetRenderTarget(rt);
    wid -> setParent(this);
    subwidgets.AddWidget(wid);
    wid -> move(getPos());
}

void Widget::unregisterSubWidget(WidgetI* wid) {
    subwidgets.RemoveWidget(dynamic_cast<Widget*>(wid));
    UpdateAllRegsets();
}

void Widget::setSize(Vec2 new_size) {
    bounds.w = new_size.x;
    bounds.h = new_size.y;
    regset.regions.Clear();
    UpdateAllRegsets();
}

void Widget::setPos(Vec2 new_pos) {
    bounds.x = new_pos.x;
    bounds.y = new_pos.y;
    regset.regions.Clear();
    UpdateAllRegsets();
}

void Widget::setParent(WidgetI *root) {
    parent = dynamic_cast<Widget*>(root);
}

void Widget::move(Vec2 shift) {
    Move_noupdate(shift);
    UpdateAllRegsets();
}

void Widget::Move_noupdate(Vec2 shift) {
    bounds.Move(shift);
    regset.Move(shift);
    subwidgets.Move_noupdate(shift);
    if (rt != nullptr) RenderInRegset(*rt, &regset);
}

void Widget::setAvailable(bool avalible_) {
    avalible = avalible_;
    // UpdateAllRegsets();
}

void Widget::SetVisible(bool vis) {
    visible = vis;
    UpdateAllRegsets();
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
    if (!visible || !avalible) return;
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
    if (!avalible) return;
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

bool Widget::onClock(uint64_t delta) {
    if (!avalible) delete this;
    return false;
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
    // if (wid -> IsExtern()) extern_widgets.InsertTail(wid);
    widgets.InsertTail(wid);
}

inline void WidgetManager::RemoveWidget (Widget* wid) {
    // if (wid -> IsExtern()) extern_widgets.Remove(widgets.GetNode(wid));
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
        if (wid -> getAvailable() && wid -> GetVisible()) {
            wid -> RenderInRegset(rt, wid -> GetRegset());
            wid -> RenderSubWidgets(rt);
        }
        widgets.Iterate(node);
    }
}

void WidgetManager::Move_noupdate (const Vec2& vec) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        node -> val -> Move_noupdate(vec);
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
        if (node -> val -> onMousePress(context)) return true;
        node = nextnode;
    }
    return false;
}

bool WidgetManager::onMouseRelease (MouseContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> onMouseRelease(context)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onMouseMove (MouseContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> onMouseMove(context)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardPress (KeyboardContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> onKeyboardPress(context)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onKeyboardRelease (KeyboardContext context) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        if (node -> val -> onKeyboardRelease(context)) return true;
        widgets.Iterate(node);
    }
    return false;
}

bool WidgetManager::onClock (uint64_t delta) {
    ListNode<Widget*>* node = nullptr;
    widgets.Iterate(node);
    while (node != nullptr) {
        ListNode<Widget*>* next = node;
        widgets.Iterate(next);
        node -> val -> onClock(delta);
        node = next;
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
        // regs += extern_regs;
        regs ^= parent_regs;

        // if (node -> val -> IsExtern()) extern_regs += regs;

        ListNode<Widget*>* node2 = node;
        widgets.Iterate(node2);
        while (node2 != nullptr) {
            RegionSet to_sub;
            node2 -> val -> GetMaxRegset(&to_sub);
            regs -= to_sub;
            widgets.Iterate(node2);
        }

        // node -> val -> RenderInRegset (*(node -> val -> GetRendertarget()), &extern_regs);
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
    RenderTarget tmp(rt.GetSize().x, rt.GetSize().y);
    plug_wid -> render(&tmp);
    rt.DrawRenderTarget_rs(tmp, Vec2(0, 0), to_draw);
}

bool ExternWidget::onMousePress(MouseContext context) {
    if (GetSubwidgets() -> onMousePress(context)) return true;
    if (!MouseOnWidget(context.position)) return false;
    if (plug_wid -> onMousePress(context)) {
        Show();
        return true;
    }
    return false;
}

bool ExternWidget::onMouseRelease(MouseContext context) {
    if (GetSubwidgets() -> onMouseRelease(context)) return true;
    // if (!MouseOnWidget(context.position)) return false;
    if (plug_wid -> onMouseRelease(context)) {
        UpdateAllRegsets();
        return true;
    }
    return false;
}

bool ExternWidget::onMouseMove(MouseContext context) {
    if (GetSubwidgets() -> onMouseMove(context)) return true;
    // if (!MouseOnWidget(context.position)) return false;
    if (plug_wid -> onMouseMove(context)) {
        UpdateAllRegsets();
        return true;
    }
    return false;
}

bool ExternWidget::onKeyboardPress(KeyboardContext context) {
    if (GetSubwidgets() -> onKeyboardPress(context)) return true;
    if (plug_wid -> onKeyboardPress(context)) {
        UpdateAllRegsets();
        return true;
    }
    return false;
}

bool ExternWidget::onKeyboardRelease(KeyboardContext context) {
    if (GetSubwidgets() -> onKeyboardRelease(context)) return true;
    if (plug_wid -> onKeyboardRelease(context)) {
        UpdateAllRegsets();
        return true;
    }
    return false;
}

bool ExternWidget::onClock(uint64_t delta) {
    if (getAvailable()) {
        GetSubwidgets() -> onClock(delta);
        bool ret = plug_wid -> onClock(delta);
        RenderInRegset(*GetRendertarget(), GetRegset());
        return ret;
    }
    else {
        delete this;
        return false;
    }
}

uint8_t ExternWidget::getPriority() const {
    return plug_wid -> getPriority();
}