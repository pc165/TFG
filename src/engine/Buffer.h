#ifndef TFG_BUFFER_H
#define TFG_BUFFER_H


#include "OpenGL.h"
#include "IncludeLibraries.h"

class VertexBuffer {
private:
    uint32_t buffer_id_;
public:
    VertexBuffer() : buffer_id_(0) {}

    void create(const void *data, uint32_t size, GLenum usage) {
        glCreateBuffers(1, &buffer_id_);
        LOG_INFO("Create vertex buffer {} with size {}", buffer_id_, size);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    ~VertexBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    }

    static void unBind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

};


template<typename T>
class IndexBuffer {
private:
    uint32_t buffer_id_;
    uint32_t count_;
public:
    IndexBuffer() : buffer_id_(0), count_(0) {};

    void create(const T *data, uint32_t count, GLenum usage) {
        count_ = count;
        glCreateBuffers(1, &buffer_id_);
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

