#include <cinttypes>
#include "myvector.h"
#include "vec2.h"


namespace plugin {
    enum class InterfaceType {
        Tool,
        Filter
    };

    template<class T>
    struct Array {
        uint64_t size;
        T* data;

        explicit Array (uint64_t size_, T* data_):
            size (size_),
            data (data_) {}

        explicit Array (MyVector<T>& myvec):
            size (myvec.GetSize()),
            data (myvec.GetData()) {}
    };

    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    struct Texture {
        uint64_t height;
        uint64_t width;

        Color *pixels;
    };

    enum class MouseButton {
        Left,
        Right
    };

    /// @note см про относительность координат
    struct MouseContext {
        Vec2 position;
        MouseButton button;
    };

    enum class Key {
        Unknown = -1, 
        A = 0,        
        B,            
        C,            
        D,            
        E,            
        F,            
        G,            
        H,            
        I,            
        J,            
        K,            
        L,            
        M,            
        N,            
        O,            
        P,            
        Q,            
        R,            
        S,            
        T,            
        U,            
        V,            
        W,            
        X,            
        Y,            
        Z,            
        Num0,         
        Num1,         
        Num2,         
        Num3,         
        Num4,         
        Num5,         
        Num6,         
        Num7,         
        Num8,         
        Num9,         
        Escape,       
        LControl,     
        LShift,       
        LAlt,         
        LSystem,      
        RControl,     
        RShift,       
        RAlt,         
        RSystem,      
        Menu,         
        LBracket,     
        RBracket,     
        Semicolon,    
        Comma,        
        Period,       
        Apostrophe,   
        Slash,        
        Backslash,    
        Grave,        
        Equal,        
        Hyphen,       
        Space,        
        Enter,        
        Backspace,    
        Tab,          
        PageUp,       
        PageDown,     
        End,          
        Home,         
        Insert,       
        Delete,       
        Add,          
        Subtract,     
        Multiply,     
        Divide,       
        Left,         
        Right,        
        Up,           
        Down,         
        Numpad0,      
        Numpad1,      
        Numpad2,      
        Numpad3,      
        Numpad4,      
        Numpad5,      
        Numpad6,      
        Numpad7,      
        Numpad8,      
        Numpad9,      
        F1,           
        F2,           
        F3,           
        F4,           
        F5,           
        F6,           
        F7,           
        F8,           
        F9,           
        F10,          
        F11,          
        F12,          
        F13,          
        F14,          
        F15,          
        Pause,        
        
        KeyCount,     
    };

    struct KeyboardContext {
        bool alt;
        bool shift;
        bool ctrl;

        Key key;
    };

    struct RenderTargetI {
        /**
         * point -- левый верхний угол
         * size  -- размер ограничивающего прямоугольника
         * */

        virtual void setPixel(Vec2 pos, Color color) = 0;
        virtual void drawLine(Vec2 pos, Vec2 point1, Color color) = 0;
        virtual void drawRect(Vec2 pos, Vec2 size, Color color) = 0;
        virtual void drawEllipse(Vec2 pos, Vec2 size, Color color) = 0;
        virtual void drawTexture(Vec2 pos, Vec2 size, const Texture *texture) = 0;
        virtual void drawText(Vec2 pos, const char *content, uint16_t char_size, Color color) = 0;

        virtual Texture *getTexture() = 0;

        /// как в RenderTexture::display
        virtual void display() = 0;

        /// clear
        virtual void clear() = 0;
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

    enum class EventType {
        MousePress,
        MouseRelease,
        MouseMove,
        KeyPress,
        KeyRelease
    };

    struct EventProcessableI {
        // MouseContext хранит в себе координаты относительно позиции RT из GuiI::getRenderTarget.
        // Мотивация: если RT это не весь экран, а RT в каком-то окне (как идейно и планировалось), то, 
        // строго говоря, плагин не знает где в реальном мире находится RT (его могли перетаскивать и проч)
        // и не может пересчитать их в локальные.
        
        /// @warning aka proposal: тогда вызов этих функций без предварительного вызова getRenderTarget UB.

        virtual bool onMouseMove(MouseContext context) = 0;
        virtual bool onMouseRelease(MouseContext context) = 0;
        virtual bool onMousePress(MouseContext context) = 0;
        virtual bool onKeyboardPress(KeyboardContext context) = 0;
        virtual bool onKeyboardRelease(KeyboardContext context) = 0;

        /// @brief clock event
        /// @param context microseconds
        virtual bool onClock(uint64_t delta) = 0;
    };

    struct EventManagerI {
        virtual void registerObject(EventProcessableI *object)   = 0;

        // 0 минимальный, ивенты приходят только объектам с их priority >= установленной по этому типу
        // 0 -- default
        virtual void setPriority(EventType, uint8_t priority)    = 0;
        virtual void unregisterObject(EventProcessableI *object) = 0;
    };

    struct WidgetI: public EventProcessableI {
        virtual void registerSubWidget(WidgetI* object) = 0;
        virtual void unregisterSubWidget(WidgetI* object) = 0;

        virtual Vec2 getSize() = 0;
        virtual void setSize(Vec2) = 0;

        virtual Vec2 getPos() = 0;
        virtual void setPos(Vec2) = 0;

        /// Нужно для обновления регинов.
        /// верно тогда и только тогда, когда виджет принадлежит плагину.
        /// В таком случае вызов getDefaultRegion невалиден (поэтому тут его и нет), и нужно 
        virtual bool isExtern() = 0;

        virtual void setParent(WidgetI *root) = 0;
        virtual WidgetI *getParent() = 0;

        virtual void move(Vec2 shift) = 0;

        // Жив ли виджет
        // Если true, то это идейно равносильно вызову деструктору, то есть его не надо рендерить, ему не надо передавать 
        // ивенты и тд и тп
        virtual bool getAvailable() = 0;
        virtual void setAvailable(bool) = 0;

        virtual void render(RenderTargetI* ) = 0;
        virtual void recalcRegion() = 0;

        virtual ~WidgetI() = 0;
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