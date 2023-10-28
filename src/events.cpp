#include "events.h"

MouseState::MouseState():
    pos (0, 0),
    btn (MOUSE_NOBTN)
    {}

MouseState::MouseState (const Vec& pos_, MouseButton btn_):
    pos (pos_),
    btn (btn_)
    {}

EventProcessable::EventProcessable():
    priority (0)
    {}

EventManager::EventManager():
    objects (),
    min_priorities {0}
    {}

void EventManager::AddObject(EventProcessable* obj) {
    objects.InsertTail(obj);
}

void EventManager::RemoveObject (EventProcessable* obj) {
    objects.Remove(objects.GetNode(obj));
}

void EventManager::ResetPriorities() {
    memset (min_priorities, 0, NUM_OF_EVENTS);
}

void EventManager::SetPriorities (const std::vector<Events>& events, uint8_t new_priority) {
    for (size_t i = 0; i < events.size(); i++) {
        min_priorities[events[i]] = new_priority;
    }
}

void EventManager::MousePress (const MouseState& mstate) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_PRESS]) obj -> MousePress (mstate);
        objects.Iterate(node);
    }
}

void EventManager::MouseRelease (const MouseState& mstate) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_RELEASE]) obj -> MouseRelease (mstate);
        objects.Iterate(node);
    }
}

void EventManager::MouseMove (const MouseState& mstate) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_MOVE]) obj -> MouseMove (mstate);
        objects.Iterate(node);
    }
}


EventLogger::EventLogger (FILE* logfile_):
    logfile (logfile_)
    {
        assert (logfile != nullptr);
        SetPriority (UINT8_MAX);
    }

void EventLogger::MousePress (const MouseState& mstate) {
    fprintf (logfile, "MOUSE_PRESS\t\t(%d, %d)\t\t%d\n", (int)mstate.pos.x, (int)mstate.pos.y, mstate.btn);
    fflush (logfile);
}

void EventLogger::MouseRelease (const MouseState& mstate) {
    fprintf (logfile, "MOUSE_RELEASE\t(%d, %d)\t\t%d\n", (int)mstate.pos.x, (int)mstate.pos.y, mstate.btn);
    fflush (logfile);
}

void EventLogger::MouseMove (const MouseState& mstate) {
    fprintf (logfile, "MOUSE_MOVE\t\t(%d, %d)\n", (int)mstate.pos.x, (int)mstate.pos.y);
    fflush (logfile);
}