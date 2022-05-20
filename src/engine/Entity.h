#ifndef TFG_ENTITY_H
#define TFG_ENTITY_H

#include <glm/glm.hpp>

struct Transform {
    glm::vec3 position{0};
    glm::vec3 scale{1};
    glm::vec3 rotationAxis{1, 0, 0};
    float rotationDegress{0};
    glm::vec3 color{};

    [[nodiscard]] std::string to_string() const {
        return fmt::format("position {} {} {}\nscale {} {} {}\n",
                           position.x, position.y, position.z,
                           scale.x, scale.y, scale.z);
    }
};

struct Entity {
    int entityId{-1};
    bool isActive{true};
    glm::vec3 colorPick{1, 1, 1};

    [[nodiscard]] std::string to_string() const {
        return fmt::format("id {} {}\ncolorPick {} {} {}\n", entityId, isActive, colorPick.x, colorPick.y, colorPick.z);
    }
};

#endif //TFG_ENTITY_H
