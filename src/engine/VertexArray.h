#ifndef TFG_VERTEXARRAY_H
#define TFG_VERTEXARRAY_H

#include "OpenGL.h"
#include "Logger.h"
#include "Buffer.h"

class Layout {
public:
    enum ElementCount {
        Point1 = 1,
        Point2 = 2,
        Point3 = 3,
        Point4 = 4
    };
    enum Type {
        CHAR, INT, UINT, FLOAT, DOUBLE
    };

    struct LayoutElement {
        Type elementType;
        ElementCount elementCount;
        bool normalized;

        GLenum getGLType() const {
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

        int32_t getSize() const {
            switch (elementType) {
                case CHAR:
                    return sizeof(GLchar);
                case INT:
                    return sizeof(GLint);
                case UINT:
                    return sizeof(GLuint);
                case FLOAT:
                    return sizeof(GLfloat);
                case DOUBLE:
                    return sizeof(GLdouble);
            }
            assert(false);
        }
    };

    Layout() : stride_(0), layout_() {}


    void push_back(LayoutElement element) {
        layout_.push_back(element);
        stride_ = element.elementCount * element.getSize();
        LOG_TRACE("Add layout with type {}, count {}, normalized {}", element.elementType,
                  element.elementCount, element.normalized);
        LOG_TRACE("Layout stride is {}", stride_);
    }

    void pop_back() {
        if (layout_.empty()) {
            LOG_WARN("Vector is empty");
        } else {
            LOG_TRACE("Remove layout with type {}, count {}, normalized {}",
                      layout_[layout_.size() - 1].elementType,
                      layout_[layout_.size() - 1].elementCount,
                      layout_[layout_.size() - 1].normalized);
            layout_.pop_back();
        }
    }

    std::vector<LayoutElement> &getLayout() { return layout_; }

    uint32_t getStride() const { return stride_; }

private:
    uint32_t stride_;
    std::vector<LayoutElement> layout_;
};

class VertexArray {
private:
    uint32_t vertexId_;
    uint32_t vertexAttribIndex_;
    uint32_t offset_;
public:
    VertexArray() : vertexId_(0), vertexAttribIndex_(0), offset_(0) {
        glGenVertexArrays(1, &vertexId_);
        glBindVertexArray(vertexId_);
    }

    void addLayout(VertexBuffer &vb, Layout &data) {
        bind();
        vb.bind();
        for (auto element: data.getLayout()) {
            LOG_TRACE("glVertexAttribPointer count {}, type {}, normalized {}, stride {} bytes, pointer {}",
                      element.elementCount,
                      element.elementType,
                      element.normalized,
                      element.getSize() * element.elementCount,
                      offset_);
            glEnableVertexAttribArray(vertexAttribIndex_);
            glVertexAttribPointer(vertexAttribIndex_,
                                  element.elementCount,
                                  element.getGLType(),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  element.getSize() * element.elementCount,
                                  reinterpret_cast<const void *>(offset_));
            vertexAttribIndex_++;
            offset_ += element.elementCount * element.getSize();
        }
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


#endif //TFG_VERTEXARRAY_H
