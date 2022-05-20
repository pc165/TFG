#ifndef TFG_BUFFER_H
#define TFG_BUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.h"


class VertexBuffer {
public:
    VertexBuffer();

    void create(const void *data, uint32_t size, GLenum usage = GL_STATIC_DRAW);

    ~VertexBuffer();

    void bind() const;

    static void unBind();

private:
    uint32_t bufferId_;
};

class VertexArray {
private:
    uint32_t vertexId_;

public:

    VertexArray();

    static void addLayout(VertexBuffer &vb,
                          uint32_t index,
                          int vectorComponentCount,
                          int componentType,
                          bool normalized = false,
                          uint32_t stride = 0,
                          void *offset = nullptr);

    ~VertexArray();

    void bind() const;

    static void unBind();

};


template<typename T>
class IndexBuffer {
private:
    uint32_t buffer_id_;
    uint32_t count_;
public:
    IndexBuffer() : buffer_id_(0), count_(0) {
        glCreateBuffers(1, &buffer_id_);
    };

    [[nodiscard]] uint32_t getCount() const { return count_; }

    void create(const T *data, uint32_t count, GLenum usage = GL_STATIC_DRAW) {
        count_ = count;
        LOG_INFO("Create index buffer {} with count {}", buffer_id_, count_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(T), data, usage);
    }

    ~IndexBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

    void bind() const {
        LOG_INFO("Bind index buffer {}", buffer_id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    }


    static void unBind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

};

#endif // TFG_BUFFER_H

