#include "events.h"

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

void EventManager::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_PRESS]) obj -> MousePress (mousepos, mousebtn);
        objects.Iterate(node);
    }
}

void EventManager::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_RELEASE]) obj -> MouseRelease (mousepos, mousebtn);
        objects.Iterate(node);
    }
}

void EventManager::MouseMove (const Vec& mousepos) {
    ListNode<EventProcessable*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessable* obj = node -> val;
        if (obj -> GetPriority() >= min_priorities[MOUSE_MOVE]) obj -> MouseMove (mousepos);
        objects.Iterate(node);
    }
}