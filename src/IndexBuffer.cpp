#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    GLCall(glGenBuffers(1, &m_RenderedId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderedId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, count*sizeof*(GLuint), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_RenderedId));
}

void IndexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderedId));
}

void IndexBuffer::Unbind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}