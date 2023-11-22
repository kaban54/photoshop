#ifndef INTERFACE_H
#define INTERFACE_H

#include "myvector.h"

namespace plugin {
    enum class InterfaceType {
        Tool,
        Filter
    };

    struct Interface {
        virtual Array<const char *> getParamNames() = 0;
        
        // в том же порядке, что getParamNames 
        virtual Array<double> getParams() = 0;
        virtual void setParams(Array<double> params) = 0;
    };
}

#endif