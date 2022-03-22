#ifndef TFG_LAYER_H
#define TFG_LAYER_H

#include "IncludeLibraries.h"
#include "Event.h"

class Layer {
public:
    explicit Layer(std::string name = "Layer") : name_(std::move(name)) {}

    virtual ~Layer() = default;

    virtual void onAttach() {}

    virtual void onDetach() {}

    virtual void onUpdate(double deltaTimeSeconds) {}

    virtual void onGuiRender() {}

    virtual void onEvent(Event &event) {}

    const std::string &getName() const { return name_; }

protected:
    std::string name_;
};

#endif //TFG_LAYER_H
