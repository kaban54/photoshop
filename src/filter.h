#ifndef FILTER_H
#define FILTER_H

#include "rendertarget.h"
#include "controller.h"
#include "editbox.h"
#include "interface.h"
#include "canvas.h"


namespace plugin {
    struct FilterI: public Interface {
        virtual void apply(RenderTargetI *data) = 0;
        virtual ~FilterI() = default;
    };
}

using namespace plugin;

class InvFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 0;

    public:

    explicit InvFilter() {}

    virtual void apply(RenderTargetI *data) override;

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};

class ClearFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 3;
    static const char* const PARAM_NAMES[];
    MyVector<const char*> p_names;

    double params[3];

    public:

    explicit ClearFilter();

    virtual void apply(RenderTargetI *data) override;
    
    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};


class FilterManager {
    FilterI* filter;
    ImageManager* image_manager;

    public:

    explicit FilterManager(ImageManager* img_man);

    void setFilter(FilterI *filter);
    void applyFilter();
};



class SetFilterController : public MWController{
    FilterManager* filter_man;
    FilterI* filter;
    std::vector<FloatNumEditBox*> editboxes;
    size_t num_of_params;

    public:

    explicit SetFilterController (FilterManager* fm, FilterI* filt, size_t num_of_params_,
                                  EventManagerI* ev_man_, WidgetI* parent_wid_);

    virtual ~SetFilterController() = default;

    virtual void OkBtnPress() override;
};

struct FilterBtnArgs : public BtnArgs {
    FilterManager* filter_man;
    FilterI* filter;
    EventManagerI* ev_man;
    WidgetI* parent_wid;
    explicit FilterBtnArgs (FilterManager* filter_man_, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_):
        filter_man (filter_man_),
        filter (filter_),
        ev_man (ev_man_),
        parent_wid (parent_wid_) {}
};

class FilterBtn : public TxtButton {
    FilterBtnArgs filter_btn_args;

    public:

    explicit FilterBtn (double x, double y, double w, double h, const char *str, uint16_t char_size_,
                        FilterManager* fm, FilterI* filter_, EventManagerI* ev_man_, WidgetI* parent_wid_);
};

#endif