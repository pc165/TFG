#include "Buffer.h"

VertexBuffer::VertexBuffer() : bufferId_(0) {
    glCreateBuffers(1, &bufferId_);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferId_);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId_);
}

void VertexBuffer::unBind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::create(const void *data, uint32_t size, GLenum usage) {
    LOG_INFO("Create vertex buffer {} with size {}", bufferId_, size);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId_);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}


VertexArray::VertexArray() : vertexId_(0) {
    glGenVertexArrays(1, &vertexId_);
    LOG_INFO("Created vertex array {}", vertexId_);
    glBindVertexArray(vertexId_);
}


GLenum VertexArray::getGLType(TFG::Type elementType) {
    switch (elementType) {
        case TFG::CHAR:
            return GL_BYTE;
        case TFG::INT:
            return GL_INT;
        case TFG::UINT:
            return GL_UNSIGNED_INT;
        case TFG::FLOAT:
            return GL_FLOAT;
        case TFG::DOUBLE:
            return GL_DOUBLE;
    }
    assert(false);
}

void VertexArray::addLayout(VertexBuffer &vb, uint32_t index, TFG::VectorType count, TFG::Type type, bool normalized, uint32_t stride, void *offset) {
    glEnableVertexAttribArray(index);
    vb.bind();
    glVertexAttribPointer(index,
                          (GLint) count,
                          getGLType(type),
                          normalized ? GL_TRUE : GL_FALSE,
                          (GLint) stride,
                          offset);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &vertexId_);
}

void VertexArray::bind() const {
    glBindVertexArray(vertexId_);
}

void VertexArray::unBind() {
    glBindVertexArray(0);
}
