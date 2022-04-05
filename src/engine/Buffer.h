#ifndef TFG_BUFFER_H
#define TFG_BUFFER_H


#include <cctype>
#include <vector>
#include "OpenGL.h"
#include "Logger.h"

enum VectorType {
    Vec1 = 1,
    Vec2 = 2,
    Vec3 = 3,
    Vec4 = 4
};

enum Type {
    CHAR, INT, UINT, FLOAT, DOUBLE
};


class VertexBuffer {
public:
    VertexBuffer() : bufferId_(0) {
        glCreateBuffers(1, &bufferId_);
    }

    void create(const void *data, uint32_t size, GLenum usage = GL_STATIC_DRAW) {
        LOG_INFO("Create vertex buffer {} with size {}", bufferId_, size);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId_);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    ~VertexBuffer() {
        glDeleteBuffers(1, &bufferId_);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, bufferId_);
    }

    static void unBind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    uint32_t bufferId_;
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

class VertexArray {
private:
    uint32_t vertexId_;

public:

    static GLenum getGLType(Type elementType) {
        switch (elementType) {
            case CHAR:
                return GL_BYTE;
            case INT:
                return GL_INT;
            case UINT:
                return GL_UNSIGNED_INT;
            case FLOAT:
                return GL_FLOAT;
            case DOUBLE:
                return GL_DOUBLE;
        }
        assert(false);
    }

    VertexArray() : vertexId_(0) {
        glGenVertexArrays(1, &vertexId_);
        LOG_INFO("Created vertex array {}", vertexId_);
        glBindVertexArray(vertexId_);
    }

    void addLayout(VertexBuffer &vb,
                   uint32_t index,
                   VectorType count,
                   Type type,
                   bool normalized = false,
                   uint32_t stride = 0,
                   void *offset = nullptr) {
        glEnableVertexAttribArray(index);
        vb.bind();
        glVertexAttribPointer(index,
                              (GLint) count,
                              getGLType(type),
                              normalized ? GL_TRUE : GL_FALSE,
                              (GLint) stride,
                              offset);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &vertexId_);
    }

    void bind() const {
        glBindVertexArray(vertexId_);
    }

    static void unBind() {
        glBindVertexArray(0);
    }

};


#endif // TFG_BUFFER_H

