#include <cinttypes>
#include "myvector.h"
#include "vec2.h"
#include "events.h"
#include "rendertarget.h"
#include "widget.h"
#include "tool.h"

namespace plugin {
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