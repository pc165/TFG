#ifndef TFG_INDEXBUFFERIMP_H
#define  TFG_INDEXBUFFERIMP_H

#include "IndexBuffer.h"

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

#endif //TFG_INDEXBUFFERIMP_H