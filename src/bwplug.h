#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "app.h"


using namespace plugin;

class BlackAndWhiteFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 1;
    static const char* const PARAM_NAMES[];
    MyVector<const char*> p_names;
    double params[1];

    public:

    explicit BlackAndWhiteFilter();

    virtual void apply(RenderTargetI *data) override;

    virtual Array<const char *> getParamNames() const override;
    virtual Array<double> getParams() const override;
    virtual void setParams(Array<double> new_params) override;
};

class TestPlugin : public Plugin {
    BlackAndWhiteFilter* filter;

    public:

    TestPlugin();

    ~TestPlugin();

    virtual Interface *getInterface() const override {return filter;}

    virtual void selectPlugin() override {};
};


#endif