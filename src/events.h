#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <string.h>
#include "vec2.h"
#include "mylist.h"
#include "myvector.h"

namespace plugin {
    enum class MouseButton {
        Left,
        Right
    };

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

    enum class EventType {
        MousePress,
        MouseRelease,
        MouseMove,
        KeyPress,
        KeyRelease, 
        Clock,

        NumOfEvents
    };

    struct EventProcessableI {
        virtual uint8_t getPriority() const = 0;
        virtual bool onMouseMove(MouseContext context) = 0;
        virtual bool onMouseRelease(MouseContext context) = 0;
        virtual bool onMousePress(MouseContext context) = 0;
        virtual bool onKeyboardPress(KeyboardContext context) = 0;
        virtual bool onKeyboardRelease(KeyboardContext context) = 0;
        virtual bool onClock(uint64_t delta) = 0;
    };

    struct EventManagerI {
        virtual void registerObject(EventProcessableI *object)   = 0;
        virtual void setPriority(EventType, uint8_t priority)    = 0;
        virtual void unregisterObject(EventProcessableI *object) = 0;
    };
}

using namespace plugin;

class EventManager : public EventProcessableI, public EventManagerI {
    MyList<EventProcessableI*> objects;
    uint8_t min_priorities[(int)EventType::NumOfEvents];
    
    public:

    explicit EventManager();

    virtual void registerObject(EventProcessableI *object) override;

    virtual void setPriority(EventType event, uint8_t priority) override;

    virtual void unregisterObject(EventProcessableI *object) override;

    void SetMinPriorities (const MyVector<EventType>& events, uint8_t new_priority);

    void ResetPriorities();

    virtual uint8_t getPriority() const override;

    virtual bool onMouseMove(MouseContext context) override;

    virtual bool onMouseRelease(MouseContext context) override;

    virtual bool onMousePress(MouseContext context) override;

    virtual bool onKeyboardPress(KeyboardContext context) override;

    virtual bool onKeyboardRelease(KeyboardContext context) override;

    virtual bool onClock(uint64_t delta) override;
};


class EventLogger : public plugin::EventProcessableI {
    FILE* logfile;

    public:

    explicit EventLogger(FILE* logfile_);

    virtual bool onMouseMove(MouseContext context) override;

    virtual bool onMouseRelease(MouseContext context) override;

    virtual bool onMousePress(MouseContext context) override;

    virtual bool onKeyboardPress(KeyboardContext context) override;

    virtual bool onKeyboardRelease(KeyboardContext context) override;

    virtual bool onClock(uint64_t delta) override;
};

#endif