#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "app.h"


using namespace plugin;

class BlackAndWhiteFilter : public FilterI {
    static const size_t NUM_OF_PARAMS = 0;
    public:

    explicit BlackAndWhiteFilter() = default;

    virtual void apply(RenderTargetI *data) override;

    virtual Array<const char *> getParamNames() override;

    virtual Array<double> getParams() override;

    virtual void setParams(Array<double> new_params) override;
};

class TestPlugin : public Plugin {
    BlackAndWhiteFilter* filter;

    public:

    TestPlugin();

    ~TestPlugin();

    virtual Interface *getInterface() override {return filter;}
};


#endif