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

enum Events {
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE,

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

    virtual void MousePress (const MouseState& mstate) = 0;

    virtual void MouseRelease (const MouseState& mstate) = 0;

    virtual void MouseMove (const MouseState& mstate) = 0;
};


class EventManager : public EventProcessable {
    MyList<EventProcessable*> objects;
    uint8_t min_priorities[NUM_OF_EVENTS];
    
    public:

    explicit EventManager();

    void AddObject (EventProcessable* obj);

    void RemoveObject (EventProcessable* obj);

    void SetPriorities (const std::vector<Events>& events, uint8_t new_priority);

    void ResetPriorities();

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;
};


class EventLogger : public EventProcessable {
    FILE* logfile;

    public:

    explicit EventLogger (FILE* logfile_);

    virtual void MousePress (const MouseState& mstate) override;

    virtual void MouseRelease (const MouseState& mstate) override;

    virtual void MouseMove (const MouseState& mstate) override;
};

#endif