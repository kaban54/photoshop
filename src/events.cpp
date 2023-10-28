#include "events.h"

EventProcessable::EventProcessable():
    priority (0)
    {}

EventManager::EventManager():
    objects (),
    min_priorities {0}
    {}

inline void EventManager::AddObject(EventProcessable* obj) {
    objects.InsertTail(obj);
}

inline void EventManager::RemoveObject (EventProcessable* obj) {
    objects.Remove(objects.GetNode(obj));
}

inline void EventManager::ResetPriorities() {
    memset (min_priorities, 0, NUM_OF_EVENTS);
}

void EventManager::SetPriorities (const std::vector<Events>& events, uint8_t new_priority) {
    for (size_t i = 0; i < events.size(); i++) {
        min_priorities[events[i]] = new_priority;
    }
}

void EventManager::MousePress (const Vec& mousepos, MouseButton mousebtn) {
    ListNode<EventProcessable*>* node = objects.GetHead();
    ListNode<EventProcessable*>* end_of_list = objects.EndOfList();

    while (node != end_of_list) {
        if (node -> val -> GetPriority() >= min_priorities[MOUSE_PRESS]) node -> val -> MousePress (mousepos, mousebtn);

        node = node -> next;
    }
}

void EventManager::MouseRelease (const Vec& mousepos, MouseButton mousebtn) {}

void EventManager::MouseMove (const Vec& mousepos) {}