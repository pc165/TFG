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
    typedef std::function<void(float)> CallBackType;

    EventState();

    void onUpdate(float frameTime);

    void mouseHandler(int button, int action, int mods);

    void mouseEnterHandler(int entered);

    void mouseMoveHandler(float x, float y);

    void mouseScrollHandler(float dx, float dy);

    void windowSizeHandler(int xpos, int ypos);

    void windowPositionHandler(int x, int y);

    void keyboardHandler(int key, int action, int mods);

    [[nodiscard]] bool keyDown(int key) const;

    [[nodiscard]] bool keyPressed(int key) const;

    [[nodiscard]] bool keyReleased(int key) const;

    [[nodiscard]] bool mouseButtonDown(int button) const;

    [[nodiscard]] bool mouseButtonPressed(int button) const;

    [[nodiscard]] bool mouseButtonReleased(int button) const;

    [[nodiscard]] const glm::vec2 &getMousePosition() const;

    [[nodiscard]] const glm::vec2 &getMouseScroll() const;

    [[nodiscard]] const glm::vec2 &getWindowSize() const;

    [[nodiscard]] const glm::vec2 &getWindowPosition() const;

    [[nodiscard]] bool isMouseMoved() const;

    [[nodiscard]] bool isMouseScrolled() const;

    [[nodiscard]] bool isWindowSizeChanged() const;

    [[nodiscard]] bool isWindowPositionChanged() const;

    [[nodiscard]] bool isMouseInWindow() const;

    void setCallback(CallBackType const &callback);

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
    bool isMouseInWindow_{false};
    CallBackType callback_{};
};

#endif //TFG_EVENTSTATE_H
