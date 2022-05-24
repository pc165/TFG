#ifndef TFG_EVENTSTATE_H
#define TFG_EVENTSTATE_H

#include <utility>
#include <vector>
#include <ctime>
#include <GLFW/glfw3.h>
#include "Logger.h"
#include "Event.h"

class EventState {
public:
    enum class InputType {
        Keyboard = 0, Mouse
    };

    struct CallBack {
        InputType inputType{-1};
        int key = -1;
        int press_release_repeat = -1;
        std::function<void()> callback = []() {
            LOG_DEBUG("No callback set!");
        };
    };

    EventState() : stateKeys(KEYBOARD_SIZE, NONE), activeKeys(KEYBOARD_SIZE, NONE),
                   stateMouseKeys(MOUSE_KEY_SIZE, NONE), activeMouseKeys(MOUSE_KEY_SIZE, NONE) {
    }

    void subscribe(InputType type, int key, int press_release_repeat, std::function<void()> &callback) {
        CallBack d;
        d.inputType = type;
        d.key = key;
        d.press_release_repeat = press_release_repeat;
        d.callback = std::move(callback);
        callbackList.emplace_back(d);
    }

    void dispatch() {
        for (auto &i: callbackList) {
            switch (i.inputType) {
                case InputType::Keyboard: {
                    if (stateKeys[i.key] == i.press_release_repeat) {
                        i.callback();
                        break;
                    }
                }
                case InputType::Mouse: {
                    if (stateMouseKeys[i.key] == i.press_release_repeat) {
                        i.callback();
                    }
                    break;
                }
            }
        }
    }

    void reset() {
        for (auto &i: stateKeys)
            i = NONE;
        for (auto &i: stateMouseKeys)
            i = NONE;
    }

    bool onEvent(const Event &event) {
        switch (event.type) {
            case Key: {
                auto key = dynamic_cast<const KeyEvent *>(&event);
                activeKeys[key->key] = key->press_release_repeat == 0; // active if key is pressed
                stateKeys[key->key] = key->press_release_repeat;
                break;
            }
            case MouseButton: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                activeMouseKeys[mouse->button] = mouse->press_release == 0; // active if mouse button is pressed
                stateMouseKeys[mouse->button] = mouse->press_release;
                break;
            }

            case MouseMoved: {
                auto mouse = dynamic_cast<const MouseButtonEvent *>(&event);
                activeMouseKeys[mouse->button] = mouse->press_release == 0; // active if mouse button is pressed
                stateMouseKeys[mouse->button] = mouse->press_release;
                break;
            }
            default:
                break;
        }
        return false;
    }

private:
    const int KEYBOARD_SIZE{GLFW_KEY_LAST};
    const int MOUSE_KEY_SIZE{GLFW_MOUSE_BUTTON_LAST};
    const int NONE{-1};
    std::vector<int> stateKeys;
    std::vector<int> activeKeys;
    std::vector<int> stateMouseKeys;
    std::vector<int> activeMouseKeys;
    float mouseX{0};
    float mouseY{0};
    std::vector<CallBack> callbackList{};
};

#endif //TFG_EVENTSTATE_H
