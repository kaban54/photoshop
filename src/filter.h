#ifndef FILTER_H
#define FILTER_H

#include "drawing.h"

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

#endif