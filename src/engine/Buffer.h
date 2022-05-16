#ifndef TFG_BUFFER_H
#define TFG_BUFFER_H


#include <cctype>
#include <vector>
#include "OpenGL.h"
#include "Logger.h"
namespace TFG{
enum VectorType {
    Vec1 = 1,
    Vec2 = 2,
    Vec3 = 3,
    Vec4 = 4
};

enum Type {
    CHAR, INT, UINT, FLOAT, DOUBLE
};
}


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

    static GLenum getGLType(TFG::Type elementType);

    VertexArray();

    static void addLayout(VertexBuffer &vb,
                          uint32_t index,
                          TFG::VectorType count,
                          TFG::Type type,
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

