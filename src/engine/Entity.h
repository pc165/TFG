#ifndef TFG_ENTITY_H
#define TFG_ENTITY_H

#include <glm/glm.hpp>

struct {
    glm::vec3 position;
    glm::vec3 rotationAxis;
    float rotationDegress;
    glm::vec3 scale;
} Transform;

struct {
    int objectId;
    glm::vec3 colorPick;
    glm::vec3 color;
} Entity;

#endif //TFG_ENTITY_H
