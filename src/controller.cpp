#include "controller.h"


MWController::MWController (EventManagerI* event_man, WidgetI* parent, Vec2 mwpos, Vec2 mwsize):
    ev_man (event_man),
    parent_wid (parent)
    {
        mw = new ModalWindow (mwpos.x, mwpos.y, mwsize.x, mwsize.y, ev_man);
        mw -> registerSubWidget (new    OkBtn (mwsize.x / 2 - 125, mwsize.y - 75, 100, 50, this));
        mw -> registerSubWidget (new CloseBtn (mwsize.x / 2 +  25, mwsize.y - 75, 100, 50, this));
        parent_wid -> registerSubWidget (mw);
    }

MWController::~MWController() {
    mw -> need_to_close = true;
}

void MWController::OkBtnPress() {
    delete this;
}

void MWController::CloseBtnPress() {
    delete this;
}



OkBtn::OkBtn (double x, double y, double w, double h, MWController* ctrl_):
    TxtButton (x, y, w, h, nullptr, nullptr, "OK", 20),
    ctrl (ctrl_)
    {}

bool OkBtn::onMousePress (MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        state = BTN_PRESSED;
        ctrl -> OkBtnPress();
    }
    return false;
}

CloseBtn::CloseBtn (double x, double y, double w, double h, MWController* ctrl_):
    TxtButton (x, y, w, h, nullptr, nullptr, "Close", 20),
    ctrl (ctrl_)
    {}

bool CloseBtn::onMousePress (MouseContext context) {
    if (state == BTN_DISABLED) return false;

    if (MouseOnWidget (context.position)) {
        state = BTN_PRESSED;
        ctrl -> CloseBtnPress();
    }
    return false;
}