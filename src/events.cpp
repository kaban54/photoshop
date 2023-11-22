#include "events.h"

using namespace plugin;

EventManager::EventManager():
    objects (),
    min_priorities {0}
    {}

void EventManager::registerObject(EventProcessableI* object) {
    objects.InsertTail(object);
}

void EventManager::unregisterObject(EventProcessableI* object) {
    objects.Remove(objects.GetNode(object));
}

void EventManager::setPriority(EventType event, uint8_t priority) {
    min_priorities[(int)event] = priority;
}

void EventManager::ResetPriorities() {
    memset (min_priorities, 0, (int)EventType::NumOfEvents * sizeof(min_priorities[0]));
}

void EventManager::SetMinPriorities (const MyVector<EventType>& events, uint8_t new_priority) {
    for (size_t i = 0; i < events.GetSize(); i++) {
        min_priorities[(int)events[i]] = new_priority;
    }
}

inline uint8_t EventManager::getPriority() const {
    return UINT8_MAX;
}

bool EventManager::onMousePress (MouseContext context) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        ListNode<EventProcessableI*>* nextnode = node;
        objects.Iterate (nextnode);
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::MousePress]) obj -> onMousePress (context);
        node = nextnode;
    }
    return false;
}

bool EventManager::onMouseRelease (MouseContext context) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::MouseRelease]) obj -> onMouseRelease (context);
        objects.Iterate(node);
    }
    return false;
}

bool EventManager::onMouseMove (MouseContext context) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::MouseMove]) obj -> onMouseMove (context);
        objects.Iterate(node);
    }
    return false;
}

bool EventManager::onKeyboardPress (KeyboardContext context) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        ListNode<EventProcessableI*>* nextnode = node;
        objects.Iterate (nextnode);
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::KeyPress]) obj -> onKeyboardPress (context);
        node = nextnode;
    }
    return false;
}

bool EventManager::onKeyboardRelease (KeyboardContext context) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        ListNode<EventProcessableI*>* nextnode = node;
        objects.Iterate (nextnode);
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::KeyRelease]) obj -> onKeyboardRelease (context);
        node = nextnode;
    }
    return false;
}

bool EventManager::onClock (uint64_t delta) {
    ListNode<EventProcessableI*>* node = nullptr;
    objects.Iterate (node);
    while (node != nullptr) {
        ListNode<EventProcessableI*>* nextnode = node;
        objects.Iterate (nextnode);
        EventProcessableI* obj = node -> val;
        if (obj -> getPriority() >= min_priorities[(int)EventType::Clock]) obj -> onClock (delta);
        node = nextnode;
    }
    return false;
}



EventLogger::EventLogger (FILE* logfile_):
    logfile (logfile_)
    {
        assert (logfile != nullptr);
    }

inline uint8_t EventLogger::getPriority() const {
    return UINT8_MAX;
}

bool EventLogger::onMousePress (MouseContext context) {
    fprintf (logfile, "MOUSE_PRESS\t\t(%d, %d)\t\t%d\n", (int)context.position.x, (int)context.position.y, (int)context.button);
    fflush (logfile);
    return false;
}

bool EventLogger::onMouseRelease (MouseContext context) {
    fprintf (logfile, "MOUSE_RELEASE\t(%d, %d)\t\t%d\n", (int)context.position.x, (int)context.position.y, (int)context.button);
    fflush (logfile);
    return false;
}

bool EventLogger::onMouseMove (MouseContext context) {
    fprintf (logfile, "MOUSE_MOVE\t\t(%d, %d)\n", (int)context.position.x, (int)context.position.y);
    fflush (logfile);
    return false;
}

bool EventLogger::onKeyboardPress (KeyboardContext context) {
    fprintf (logfile, "KEYBOARD_PRESS\t\t%d\n", (int)context.key);
    fflush (logfile);
    return false;
}

bool EventLogger::onKeyboardRelease (KeyboardContext context) {
    fprintf (logfile, "KEYBOARD_RELEASE\t\t%d\n", (int)context.key);
    fflush (logfile);
    return false;
}

bool EventLogger::onClock (uint64_t delta) {
    fprintf (logfile, "delta = %lu\n", delta);
    fflush (logfile);
    return false;
}