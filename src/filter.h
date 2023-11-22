#ifndef FILTER_H
#define FILTER_H

#include "rendertarget.h"
#include "buttons.h"
#include "window.h"
#include "editbox.h"
#include "interface.h"


namespace plugin {
    struct FilterI: public Interface {
        virtual void apply(RenderTargetI *data) = 0;
    };

    struct FilterManagerI {
        virtual void setRenderTarget(RenderTargetI *target) = 0;
        virtual void setFilter(FilterI *filter) = 0;
        virtual void applyFilter() = 0;
    };
}

using namespace plugin;

class TestFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 1;
    static const char* const PARAM_NAMES[];

    double params[1];

    public:

    explicit TestFilter();

    virtual void apply(RenderTargetI *data) override;

    virtual Array<const char *> getParamNames() override;
    virtual Array<double> getParams() override;
    virtual void setParams(Array<double> new_params) override;
};

class ClearFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 3;
    static const char* const PARAM_NAMES[];

    double params[3];

    public:

    explicit ClearFilter();

    virtual void apply(RenderTargetI *data) override;
    
    virtual Array<const char *> getParamNames() override;
    virtual Array<double> getParams() override;
    virtual void setParams(Array<double> new_params) override;

};


class FilterManager : public FilterManagerI {
    FilterI* filter;
    RenderTargetI* rt;

    public:

    explicit FilterManager();

    virtual void setRenderTarget(RenderTargetI *target) override;
    virtual void setFilter(FilterI *filter) override;
    virtual void applyFilter() override;
};


class SetFilterController;

class SetFilterOkBtn : public TxtButton {
    SetFilterController* sfc;

    public:

    SetFilterOkBtn (double x, double y, double w, double h, SetFilterController* sfc_);

    virtual bool onMousePress (MouseContext context) override;
};

class SetFilterController {
    FilterManagerI* filter_man;
    FilterI* filter;
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    ModalWindow* mw;
    std::vector<FloatNumEditBox*> editboxes;
    size_t num_of_params;

    public:

    explicit SetFilterController (FilterManagerI* fm, FilterI* filt, EventManagerI* ev_man_, WidgetI* parent_wid_);

    ~SetFilterController();

    void OkBtnPress ();
};

struct FilterBtnArgs : public BtnArgs {
    FilterManagerI* filter_man;
    FilterI* filter;
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    explicit FilterBtnArgs (FilterManagerI* filter_man_, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_):
        filter_man (filter_man_),
        filter (filter_),
        ev_man (ev_man_),
        parent_wid (parent_wid_) {}
};

class FilterBtn : public TxtButton {
    FilterBtnArgs filter_btn_args;

    public:

    explicit FilterBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                        FilterManagerI* fm, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_);
};

#endif