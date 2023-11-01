#ifndef FILTER_H
#define FILTER_H

#include "rendertarget.h"
#include "buttons.h"

class Filter {
    public:

    virtual void Apply (RenderTarget &rt) const = 0;

    virtual void SetParams (const std::vector<double>& params) = 0;

    virtual std::vector<const char*> GetParamNames() const = 0;
};

class TestFilter : public Filter {
    public:

    virtual void Apply (RenderTarget &rt) const override;

    virtual void SetParams (const std::vector<double>& params) override {}

    virtual std::vector<const char*> GetParamNames() const override {return std::vector<const char*> {};}
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


struct FilterBtnArgs : public BtnArgs{
    FilterManager* filter_man;
    Filter* filter;
    explicit FilterBtnArgs (FilterManager* filter_man_, Filter* filter_):
        filter_man (filter_man_),
        filter (filter_) {}
};

void filter_btn_action (BtnArgs* filter_btn_args);

class FilterBtn : public TxtButton {
    FilterBtnArgs filter_btn_args;

    public:

    explicit FilterBtn (double x, double y, double w, double h, const Text& txt_, FilterManager* fm, Filter* filter_);
};

#endif