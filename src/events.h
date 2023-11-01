#ifndef EVENTS_H
#define EVENTS_H

#include "vec.h"
#include "mylist.h"
#include <stdint.h>
#include <string.h>
#include <vector>

enum MouseButton {
    MOUSE_NOBTN = -1,
    MOUSE_LEFT  = 0,
    MOUSE_RIGHT = 1,

    NUM_OF_MBUTONS
};

enum KeyboardKey {
    A_KEY = 0,        ///< The A key
    B_KEY,            ///< The B key
    C_KEY,            ///< The C key
    D_KEY,            ///< The D key
    E_KEY,            ///< The E key
    F_KEY,            ///< The F key
    G_KEY,            ///< The G key
    H_KEY,            ///< The H key
    I_KEY,            ///< The I key
    J_KEY,            ///< The J key
    K_KEY,            ///< The K key
    L_KEY,            ///< The L key
    M_KEY,            ///< The M key
    N_KEY,            ///< The N key
    O_KEY,            ///< The O key
    P_KEY,            ///< The P key
    Q_KEY,            ///< The Q key
    R_KEY,            ///< The R key
    S_KEY,            ///< The S key
    T_KEY,            ///< The T key
    U_KEY,            ///< The U key
    V_KEY,            ///< The V key
    W_KEY,            ///< The W key
    X_KEY,            ///< The X key
    Y_KEY,            ///< The Y key
    Z_KEY,            ///< The Z key
    NUM0_KEY,         ///< The 0 key
    NUM1_KEY,         ///< The 1 key
    NUM2_KEY,         ///< The 2 key
    NUM3_KEY,         ///< The 3 key
    NUM4_KEY,         ///< The 4 key
    NUM5_KEY,         ///< The 5 key
    NUM6_KEY,         ///< The 6 key
    NUM7_KEY,         ///< The 7 key
    NUM8_KEY,         ///< The 8 key
    NUM9_KEY,         ///< The 9 key
    Escape_KEY,       ///< The Escape keyarrow,
    LControl_KEY,     ///< The left Control key
    LShift_KEY,       ///< The left Shift key
    LAlt_KEY,         ///< The left Alt key
    LSystem_KEY,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
    RControl_KEY,     ///< The right Control key
    RShift_KEY,       ///< The right Shift key
    RAlt_KEY,         ///< The right Alt key
    RSystem_KEY,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
    Menu_KEY,         ///< The Menu key
    LBracket_KEY,     ///< The [ key
    RBracket_KEY,     ///< The ] key
    Semicolon_KEY,    ///< The ; key
    Comma_KEY,        ///< The , key
    Period_KEY,       ///< The . key
    Quote_KEY,        ///< The ' key
    Slash_KEY,        ///< The / key
    Backslash_KEY,    ///< The \ key
    Tilde_KEY,        ///< The ~ key
    Equal_KEY,        ///< The = key
    Hyphen_KEY,       ///< The - key (hyphen)
    Space_KEY,        ///< The Space key
    Enter_KEY,        ///< The Enter/Return keys
    Backspace_KEY,    ///< The Backspace key
    Tab_KEY,          ///< The Tabulation key
    PageUp_KEY,       ///< The Page up key
    PageDown_KEY,     ///< The Page down key
    End_KEY,          ///< The End key
    Home_KEY,         ///< The Home key
    Insert_KEY,       ///< The Insert key
    Delete_KEY,       ///< The Delete key
    Add_KEY,          ///< The + key
    Subtract_KEY,     ///< The - key (minus, usually from numpad)
    Multiply_KEY,     ///< The * key
    Divide_KEY,       ///< The / key
    Left_KEY,         ///< Left arrow
    Right_KEY,        ///< Right arrow
    Up_KEY,           ///< Up arrow
    Down_KEY,         ///< Down arrow
    NUMPAD0_KEY,      ///< The numpad 0 key
    NUMPAD1_KEY,      ///< The numpad 1 key
    NUMPAD2_KEY,      ///< The numpad 2 key
    NUMPAD3_KEY,      ///< The numpad 3 key
    NUMPAD4_KEY,      ///< The numpad 4 key
    NUMPAD5_KEY,      ///< The numpad 5 key
    NUMPAD6_KEY,      ///< The numpad 6 key
    NUMPAD7_KEY,      ///< The numpad 7 key
    NUMPAD8_KEY,      ///< The numpad 8 key
    NUMPAD9_KEY,      ///< The numpad 9 key
    F1_KEY,           ///< The F1 key
    F2_KEY,           ///< The F2 key
    F3_KEY,           ///< The F3 key
    F4_KEY,           ///< The F4 key
    F5_KEY,           ///< The F5 key
    F6_KEY,           ///< The F6 key
    F7_KEY,           ///< The F7 key
    F8_KEY,           ///< The F8 key
    F9_KEY,           ///< The F9 key
    F10_KEY,          ///< The F10 key
    F11_KEY,          ///< The F11 key
    F12_KEY,          ///< The F12 key
    F13_KEY,          ///< The F13 key
    F14_KEY,          ///< The F14 key
    F15_KEY,          ///< The F15 key
    PAUSE_KEY,        ///< The Pause key

    NUM_OF_KEYS
};

enum Events {
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE,
    KEYBOARD_PRESS,
    KEYBOARD_RELEASE,

    NUM_OF_EVENTS
};

struct MouseState {
    Vec pos;
    MouseButton btn;

    explicit MouseState();

    explicit MouseState (const Vec& pos_, MouseButton btn_ = MOUSE_NOBTN);
};

class EventProcessable {
    uint8_t priority;

    public:

    explicit EventProcessable();

    void SetPriority (uint8_t priority_) {priority = priority_;}

    uint8_t GetPriority() {return priority;}

    virtual void MousePress (const MouseState& mstate) {}

    virtual void MouseRelease (const MouseState& mstate) {}

    virtual void MouseMove (const MouseState& mstate) {}

    virtual void KeyboardPress (KeyboardKey key) {}

    virtual void KeyboardRelease (KeyboardKey key) {}
};


class EventManager : public EventProcessable {
    MyList<EventProcessable*> objects;
    uint8_t min_priorities[NUM_OF_EVENTS];
    
    public:

    explicit EventManager();

    void AddObject (EventProcessable* obj);

    void RemoveObject (EventProcessable* obj);

    void SetMinPriorities (const std::vector<Events>& events, uint8_t new_priority);

    void ResetPriorities();

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void KeyboardPress (KeyboardKey key) override;

    virtual void KeyboardRelease (KeyboardKey key) override;
};


class EventLogger : public EventProcessable {
    FILE* logfile;

    public:

    explicit EventLogger (FILE* logfile_);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;

    virtual void KeyboardPress (KeyboardKey key) override;

    virtual void KeyboardRelease (KeyboardKey key) override;
};

#endif