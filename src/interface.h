#ifndef INTERFACE_H
#define INTERFACE_H

#include "myvector.h"

namespace plugin {
    enum class InterfaceType {
        Tool,
        Filter,
        System // системный плагин, мб обертка
    };

    struct Interface {
        virtual Array<const char *> getParamNames() const = 0;
        
        // в том же порядке, что getParamNames 
        virtual Array<double> getParams() const = 0;
        virtual void setParams(Array<double> params) = 0;

        virtual ~Interface() = default;
    };
}

#endif