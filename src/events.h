#ifndef EVENTS_H
#define EVENTS_H

#include "vec.h"
#include "mylist.h"
#include <stdint.h>
#include <string.h>
#include <vector>

enum MouseButton {
    MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1,

    NUM_OF_MBUTONS
};

enum Events {
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE,

    NUM_OF_EVENTS
};


class EventProcessable {
    uint8_t priority;

    public:

    explicit EventProcessable();

    void SetPriority (uint8_t priority_) {priority = priority_;}

    uint8_t GetPriority() {return priority;}

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) = 0;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) = 0;

    virtual void MouseMove (const Vec& mousepos) = 0;
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

    virtual void MousePress (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseRelease (const Vec& mousepos, MouseButton mousebtn) override;

    virtual void MouseMove (const Vec& mousepos) override;
};

#endif