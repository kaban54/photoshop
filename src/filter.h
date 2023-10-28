#ifndef FILTER_H
#define FILTER_H

#include "rendertarget.h"

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

#endif