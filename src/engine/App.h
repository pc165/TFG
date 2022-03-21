#ifndef TFG_APP_H
#define TFG_APP_H

#include <vector>
#include <string>
#include <memory>
#include "Layer.h"
#include "IncludeLibraries.h"
#include "Gui.h"

class App {
public:
    explicit App(const std::string &title = "Default App", uint32_t witdh = 1080, uint32_t heigth = 720);

    ~App();

    void run();

    void pushLayer(Layer *layer);

    void removeLayer(Layer const *layer);

private:
    std::vector<Layer *> layers_;
    GLFWwindow *window_;
    Gui guiLayer;
    double lastFrameTime;
    bool shouldClose;
    static App *instance_;

public:
    bool isShouldClose() const;

    void setShouldClose(bool close);

    GLFWwindow *getWindow() { return window_; }

    static App &getInstance() { return *instance_; }
};


#endif //TFG_APP_H
