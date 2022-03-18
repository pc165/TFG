#include "VertexBuffer.h"


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

