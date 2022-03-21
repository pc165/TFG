#ifndef TFG_BUFFER_H
#define TFG_BUFFER_H

#include <cstdint>
#include <cstdio>
#include "IncludeLibraries.h"

class VertexBuffer {
private:
    uint32_t buffer_id_;
public:
    VertexBuffer(const void *data, uint32_t size, GLenum usage);

    ~VertexBuffer();

    void bind() const;

    static void unBind();
};

VertexBuffer::VertexBuffer(const void *data, uint32_t size, GLenum usage) : buffer_id_(-1) {
    glCreateBuffers(1, &buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &buffer_id_);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
}

void VertexBuffer::unBind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<typename T>
class IndexBuffer {
private:
    uint32_t buffer_id_;
    uint32_t count_;
public:
    IndexBuffer(const T *data, uint32_t count, GLenum usage);

    ~IndexBuffer();

    void bind() const;

    static void unBind();

};


template<typename T>
IndexBuffer<T>::IndexBuffer(const T *data, uint32_t count, GLenum usage) : buffer_id_(-1), count_(count) {
    glCreateBuffers(1, &buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(T), data, usage);
}

template<typename T>
IndexBuffer<T>::~IndexBuffer() {
    glDeleteBuffers(1, &buffer_id_);
}

template<typename T>
void IndexBuffer<T>::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

template<typename T>
void IndexBuffer<T>::unBind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


#endif // TFG_BUFFER_H

