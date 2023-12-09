#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "window.h"
#include "buttons.h"


class MWController {
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    ModalWindow* mw;

    public:

    explicit MWController (EventManagerI* event_man, WidgetI* parent, Vec2 mwpos, Vec2 mwsize);

    virtual ~MWController();

    ModalWindow* GetMW() {return mw;}
    WidgetI* GetParent() {return parent_wid;}

    virtual void OkBtnPress();
    virtual void CloseBtnPress();
};

class OkBtn : public TxtButton {
    MWController* ctrl;

    public:

    OkBtn (double x, double y, double w, double h, MWController* ctrl_);

    virtual bool onMousePress (MouseContext context) override;
};

class CloseBtn : public TxtButton {
    MWController* ctrl;

    public:

    CloseBtn (double x, double y, double w, double h, MWController* ctrl_);

    virtual bool onMousePress (MouseContext context) override;
};

#endif