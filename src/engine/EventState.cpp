#include "EventState.h"
EventState::EventState() : stateKeys_(KEYBOARD_SIZE, 0),
                           activeKeys_(KEYBOARD_SIZE, 0),
                           stateMouseKeys_(MOUSE_KEY_SIZE, 0),
                           activeMouseKeys_(MOUSE_KEY_SIZE, 0),
                           mousePosition_(0, 0) {
}

void EventState::onUpdate(float frameTime) {
    callback_(frameTime);
    for (auto &i: stateKeys_) i = 0;
    for (auto &i: stateMouseKeys_) i = 0;
    mouseMoved_ = false;
    mouseScrolled_ = false;
    windowSizeChanged_ = false;
    windowPositionChanged_ = false;
}

void EventState::mouseHandler(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        activeMouseKeys_[button] = true;
    } else if (action == GLFW_RELEASE) {
        activeMouseKeys_[button] = false;
    }
    stateMouseKeys_[button] = action;
}

void EventState::mouseEnterHandler(int entered) {
    isMouseInWindow_ = entered;
}

void EventState::mouseMoveHandler(float x, float y) {
    mouseMoved_ = true;
    mousePosition_ = {x, y};
}

void EventState::mouseScrollHandler(float dx, float dy) {
    mouseScrolled_ = true;
    mouseScroll_ = {dx, dy};
}

void EventState::windowSizeHandler(int xpos, int ypos) {
    windowSizeChanged_ = true;
    windowSize_ = {xpos, ypos};
}

void EventState::windowPositionHandler(int x, int y) {
    windowPositionChanged_ = true;
    windowPosition_ = {x, y};
}

void EventState::keyboardHandler(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        activeKeys_[key] = true;
    } else if (action == GLFW_RELEASE) {
        activeKeys_[key] = false;
    }
    stateKeys_[key] = action;
}

bool EventState::keyDown(int key) const {
    return activeKeys_[key];
}

bool EventState::keyPressed(int key) const {
    return stateKeys_[key] == GLFW_PRESS;
}

bool EventState::mouseButtonDown(int button) const {
    return activeMouseKeys_[button];
}

bool EventState::mouseButtonPressed(int button) const {
    return stateMouseKeys_[button] == GLFW_PRESS;
}

bool EventState::keyReleased(int key) const {
    return stateKeys_[key] == GLFW_RELEASE;
}

bool EventState::mouseButtonReleased(int button) const {
    return stateMouseKeys_[button] == GLFW_RELEASE;
}

void EventState::setCallback(const EventState::CallBackType &callback) {
    callback_ = callback;
}

bool EventState::isMouseInWindow() const {
    return isMouseInWindow_;
}

bool EventState::isWindowPositionChanged() const {
    return windowPositionChanged_;
}

bool EventState::isWindowSizeChanged() const {
    return windowSizeChanged_;
}

bool EventState::isMouseScrolled() const {
    return mouseScrolled_;
}

bool EventState::isMouseMoved() const {
    return mouseMoved_;
}

const glm::vec2 &EventState::getWindowSize() const {
    return windowSize_;
}

const glm::vec2 &EventState::getWindowPosition() const {
    return windowPosition_;
}

const glm::vec2 &EventState::getMouseScroll() const {
    return mouseScroll_;
}

const glm::vec2 &EventState::getMousePosition() const {
    return mousePosition_;
}
