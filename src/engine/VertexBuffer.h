#ifndef TFG_VERTEXBUFFER_H
#define TFG_VERTEXBUFFER_H

#include <cstdint>
#include <cstdio>
#include <GL/glew.h>

class VertexBuffer {
private:
    uint32_t buffer_id_;
public:
    VertexBuffer(const void *data, uint32_t size, GLenum usage);

    ~VertexBuffer();

    void bind() const;

    static void unBind();
};


#endif //TFG_VERTEXBUFFER_H
