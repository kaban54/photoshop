#ifndef FILTER_H
#define FILTER_H

#include "rendertarget.h"
#include "buttons.h"
#include "window.h"
#include "editbox.h"

class Filter {
    public:
    const unsigned int num_of_params;

    Filter (unsigned int num_of_params_):
        num_of_params (num_of_params_) {}

    virtual void Apply (RenderTarget &rt) const = 0;

    virtual void SetParams (const std::vector<double>& params) = 0;

    virtual std::vector<const char*> GetParamNames() const = 0;
};

class TestFilter : public Filter {
    double test_param;

    public:

    explicit TestFilter():
        Filter (1),
        test_param (0) {}

    virtual void Apply (RenderTarget &rt) const override;

    virtual void SetParams (const std::vector<double>& params) override;

    virtual std::vector<const char*> GetParamNames() const override {return std::vector<const char*> {"test param"};}
};


class FilterManager {
    Filter* current;
    bool active;

    public:

    explicit FilterManager();

    void SetFilter (Filter* filter) {current = filter;}

    void Apply (RenderTarget &rt);

    void Activate() {active = true;}

    bool IsActive() {return active;}
};


class SetFilterController;

class SetFilterOkBtn : public TxtButton {
    SetFilterController* sfc;

    public:

    SetFilterOkBtn (double x, double y, double w, double h, SetFilterController* sfc_);

    virtual void MousePress (const MouseState& mstate) override;
};

class SetFilterController {
    FilterManager* filter_man;
    Filter* filter;
    EventManager* ev_man;
    Widget* parent_wid;
    ModalWindow* mw;
    std::vector<FloatNumEditBox*> editboxes;

    public:

    explicit SetFilterController (FilterManager* fm, Filter* filt, EventManager* ev_man_, Widget* parent_wid_);

    ~SetFilterController();

    void OkBtnPress ();
};

struct FilterBtnArgs : public BtnArgs {
    FilterManager* filter_man;
    Filter* filter;
    EventManager* ev_man;
    Widget* parent_wid;
    explicit FilterBtnArgs (FilterManager* filter_man_, Filter* filter_, EventManager* ev_man_, Widget* parent_wid_):
        filter_man (filter_man_),
        filter (filter_),
        ev_man (ev_man_),
        parent_wid (parent_wid_) {}
};

class FilterBtn : public TxtButton {
    FilterBtnArgs filter_btn_args;

    public:

    explicit FilterBtn (double x, double y, double w, double h, const Text& txt_,
                        FilterManager* fm, Filter* filter_, EventManager* ev_man_, Widget* parent_wid_);
};

#endif