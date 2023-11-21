#include <cinttypes>
#include "myvector.h"
#include "vec2.h"
#include "events.h"
#include "rendertarget.h"
#include "widget.h"

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

    struct Plugin {
        /* где-то тут лежит App*, но это дело автора плагина */
        uint64_t id;
        const char *name;
        InterfaceType type;

        virtual Interface *getInterface() = 0;
        virtual ~Plugin() = 0;
    };

    struct ToolI: public Interface {
        virtual const Texture *getIcon() = 0;

        virtual void paintOnPress(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnRelease(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void paintOnMove(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
        virtual void disable(RenderTargetI *data, RenderTargetI *tmp, MouseContext context, Color color) = 0;
    };

    struct ToolManagerI {
        virtual void setColor(Color color) = 0;
        virtual void setTool(ToolI *tool) = 0;

        virtual ToolI *getTool() = 0;
        virtual Color  getColor() = 0;

        virtual void paintOnMove(RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnPress(RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void paintOnRelease(RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
        virtual void disableTool(RenderTargetI *data, RenderTargetI *tmp, MouseContext context) = 0;
    };

    struct FilterI: public Interface {
        virtual void apply(RenderTargetI *data) = 0;
    };

    struct FilterManagerI {
        virtual void setRenderTarget(RenderTargetI *target) = 0;
        virtual void setFilter(FilterI *filter) = 0;
        virtual void applyFilter() = 0;
    };

    struct GuiI {
        virtual Vec2 getSize() = 0; // размер доступной для рисования площади (которую можно запросить)

        /// @brief запросить RT.
        /// Хост создает новое окно / отдает какое-то, абсолютно пустое, с единственным RT на все окно.
        /// @param size -- размер запрашиваемой области
        /// @param pos  -- (относительное [относительно предоставленной области]) смещение запрашиваемой области
        virtual RenderTargetI* getRenderTarget(Vec2 size, Vec2 pos, Plugin *self) = 0;

        /// @brief Создает окно с параметрами, каким-то образом узнает у пользователя 
        ///     значения параметров и потом возвращает их интерфейсу через Interface::set_params
        /// @note окно не обязательно модальное, да и вообще implementation defined. Мем в том, что плагин находится в 
        ///     неопределенном/дефолтном состоянии между createParamWindow и Interface::set_params и взаимодействие с ним UB
        virtual void createParamWindow(Array<const char *> param_names, Interface * self) = 0;

        /**
         * @brief Get the root widget of widget tree
         * 
         * @return WidgetI* root
         */
        virtual WidgetI* getRoot();
    };

    struct App {
        GuiI *root;
        EventManagerI *event_manager; 
        ToolManagerI *tool_manager;
        FilterManagerI *filter_manager; 
    };
}

extern "C" plugin::Plugin* getInstance(plugin::App *app);