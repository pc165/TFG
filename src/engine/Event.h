#ifndef TFG_EVENT_H
#define TFG_EVENT_H

enum EventType {
    WindowClose = 0, WindowResize, WindowPosition,
    Key,
    MouseMoved, MouseScrolled, MouseButton
};

struct Event {
    EventType type;

    explicit Event(EventType type) : type(type) {};

    virtual ~Event() = default;
};

struct WindowCloseEvent : public Event {
    explicit WindowCloseEvent(EventType type) : Event(type) {};
};

struct WindowResizeEvent : public Event {
    WindowResizeEvent(EventType type, int width, int height) : Event(type), width(width), height(height) {};
    int width;
    int height;
};

struct WindowPositionEvent : public Event {
    WindowPositionEvent(EventType type, int xpos, int ypos) : Event(type), xPos(xpos), yPos(ypos) {};
    int xPos;
    int yPos;
};

struct MouseButtonEvent : public Event {
    MouseButtonEvent(EventType type, int button, int press_release) : Event(type), button(button),
                                                                      press_release(press_release) {};
    int button;
    int press_release; // 0 for press, 1 for release
};

struct KeyEvent : public Event {
    KeyEvent(EventType type, int key, int press_release_repeat) : Event(type), key(key),
                                                                  press_release_repeat(press_release_repeat) {};
    int key;
    int press_release_repeat; // 0 for press, 1 for release, 2 for repeat
};

struct MouseScrollEvent : public Event {
    MouseScrollEvent(EventType type, float xOffset, float yOffset) : Event(type), xOffset(xOffset), yOffset(yOffset) {};
    float xOffset;
    float yOffset;
};

struct MouseMoveEvent : public Event {
    MouseMoveEvent(EventType type, float xPos, float yPos) : Event(type), xPos(xPos), yPos(yPos) {};
    float xPos;
    float yPos;
};

#endif //TFG_EVENT_H
