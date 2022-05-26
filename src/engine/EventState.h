#ifndef TFG_EVENTSTATE_H
#define TFG_EVENTSTATE_H

#include <utility>
#include <vector>
#include <ctime>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Logger.h"

class EventState {
public:
    typedef std::function<void  (float)> CallBackType;

    EventState() : stateKeys_(KEYBOARD_SIZE, 0),
                   activeKeys_(KEYBOARD_SIZE, 0),
                   stateMouseKeys_(MOUSE_KEY_SIZE, 0),
                   activeMouseKeys_(MOUSE_KEY_SIZE, 0),
                   mousePosition_(0, 0) {
    }

    void onUpdate(float frameTime) {
        callback_(frameTime);
        for (auto &i: stateKeys_) i = 0;
        for (auto &i: stateMouseKeys_) i = 0;
        mouseMoved_ = false;
        mouseScrolled_ = false;
        windowSizeChanged_ = false;
        windowPositionChanged_ = false;
    }

    void mouseHandler(int button, int action) {
        if (action == GLFW_PRESS) {
            activeMouseKeys_[button] = true;
        } else if (action == GLFW_RELEASE) {
            activeMouseKeys_[button] = false;
        }
        stateMouseKeys_[button] = action;
    }

    void mouseMoveHandler(float x, float y) {
        mouseMoved_ = true;
        mousePosition_ = {x, y};
    }

    void mouseScrollHandler(float dx, float dy) {
        mouseScrolled_ = true;
        mouseScroll_ = {dx, dy};
    }

    void windowSizeHandler(int xpos, int ypos) {
        windowSizeChanged_ = true;
        windowSize_ = {xpos, ypos};
    }

    void windowPositionHandler(int x, int y) {
        windowPositionChanged_ = true;
        windowPosition_ = {x, y};
    }

    void keyboardHandler(int key, int action, int mods) {
        if (action == GLFW_PRESS) {
            activeKeys_[key] = true;
        } else if (action == GLFW_RELEASE) {
            activeKeys_[key] = false;
        }
        stateKeys_[key] = action;
    }

    [[nodiscard]] bool keyDown(int key) const {
        return activeKeys_[key];
    }

    [[nodiscard]] bool keyPressed(int key) const {
        return stateKeys_[key] == GLFW_PRESS;
    }

    [[nodiscard]] bool keyReleased(int key) const {
        return stateKeys_[key] == GLFW_RELEASE;
    }

    [[nodiscard]] bool mouseButtonDown(int button) const {
        return activeMouseKeys_[button];
    }

    [[nodiscard]] bool mouseButtonPressed(int button) const {
        return stateMouseKeys_[button] == GLFW_PRESS;
    }

    [[nodiscard]] bool mouseButtonReleased(int button) const {
        return stateMouseKeys_[button] == GLFW_RELEASE;
    }

    [[nodiscard]] const glm::vec2 &getMousePosition() const {
        return mousePosition_;
    }

    [[nodiscard]] const glm::vec2 &getMouseScroll() const {
        return mouseScroll_;
    }

    [[nodiscard]] const glm::vec2 &getWindowSize() const {
        return windowSize_;
    }

    [[nodiscard]] const glm::vec2 &getWindowPosition() const {
        return windowPosition_;
    }

    [[nodiscard]] bool isMouseMoved() const {
        return mouseMoved_;
    }

    [[nodiscard]] bool isMouseScrolled() const {
        return mouseScrolled_;
    }

    [[nodiscard]] bool isWindowSizeChanged() const {
        return windowSizeChanged_;
    }

    [[nodiscard]] bool isWindowPositionChanged() const {
        return windowPositionChanged_;
    }

    void setCallback(CallBackType const &callback) {
        callback_ = callback;
    }

private:
    const int KEYBOARD_SIZE{GLFW_KEY_LAST};
    const int MOUSE_KEY_SIZE{GLFW_MOUSE_BUTTON_LAST};
    std::vector<int> stateKeys_;
    std::vector<int> activeKeys_;
    std::vector<int> stateMouseKeys_;
    std::vector<int> activeMouseKeys_;
    glm::vec2 mousePosition_{};
    glm::vec2 mouseScroll_{};
    glm::vec2 windowSize_{};
    glm::vec2 windowPosition_{};
    bool mouseMoved_{false};
    bool mouseScrolled_{false};
    bool windowSizeChanged_{false};
    bool windowPositionChanged_{false};
    CallBackType callback_{};
};

#endif //TFG_EVENTSTATE_H
