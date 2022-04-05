//#ifndef TFG_EVENTSTATE_H
//#define TFG_EVENTSTATE_H
//
//#include <vector>
//#include <ctime>
//#include "OpenGL.h"
//#include "Logger.h"
//
//class EventState {
//private:
//    const int KEYBOARD_SIZE{GLFW_KEY_LAST};
//    const int MOUSE_KEY_SIZE{GLFW_MOUSE_BUTTON_LAST};
//    const int NONE{-1};
//    std::vector<int> stateKeys;
//    std::vector<int> activeKeys;
//
//    std::vector<int> stateMouseKeys;
//    std::vector<int> activeMouseKeys;
//
//public:
//
//    EventState() : stateKeys(KEYBOARD_SIZE, NONE), activeKeys(KEYBOARD_SIZE, NONE),
//                   stateMouseKeys(MOUSE_KEY_SIZE, NONE), activeMouseKeys(MOUSE_KEY_SIZE, NONE) {
//    }
//
//    void reset() {
//        for (auto &i: stateKeys)
//            i = NONE;
//        for (auto &i: stateMouseKeys)
//            i = NONE;
//    }
//
//
//    void mouseHandler(int button, int action) {
//        activeMouseKeys[button] = action != GLFW_PRESS;
//        stateMouseKeys[button] = action;
//    }
//
//    void keyboardHandler(int key, int action) {
//        activeKeys[key] = action != GLFW_PRESS;
//        stateKeys[key] = action;
//    }
//
//    bool keyDown(int key) const {
//        LOG_DEBUG("{} {}", activeKeys[key], stateKeys[key]);
//        return activeKeys[key];
//    }
//
//    bool keyPressed(int key) const {
//        return stateKeys[key] == GLFW_PRESS;
//    }
//
//    bool keyReleased(int key) const {
//        return stateKeys[key] == GLFW_RELEASE;
//    }
//
//    bool mouseButtonDown(int button) const {
//        return activeMouseKeys[button];
//    }
//
//    bool mouseButtonPressed(int button) const {
//        return stateMouseKeys[button] == GLFW_RELEASE;
//    }
//
//    bool mouseButtonReleased(int button) const {
//        return stateMouseKeys[button] == GLFW_RELEASE;
//    }
//};
//
//#endif //TFG_EVENTSTATE_H
