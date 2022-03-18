#ifndef TFG_INDEXBUFFER_H
#define TFG_INDEXBUFFER_H

#include <cstdint>
#include <cstdio>
#include <GL/glew.h>


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


#endif // TFG_INDEXBUFFER_H

#include "IndexBufferImp.h"
