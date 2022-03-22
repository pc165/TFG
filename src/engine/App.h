#ifndef TFG_APP_H
#define TFG_APP_H

#include "Gui.h"
#include "Layer.h"

class App {
public:
    explicit App(const std::string &title = "Default App", uint32_t witdh = 1080, uint32_t heigth = 720);

    ~App();

    void run();

    void pushLayer(Layer *layer);

    void removeLayer(Layer const *layer);

private:
    static App *instance_;
    GLFWwindow *window_;
    Gui guiLayer;
    std::vector<Layer *> layers_;
    double lastFrameTime;
    bool shouldClose;
    bool enableGui;

public:
    bool isShouldClose() const;

    void setShouldClose(bool close);

    GLFWwindow *getWindow() { return window_; }

    static App &getInstance() { return *instance_; }
};


#endif //TFG_APP_H
