#pragma once
class VertexBuffer
{
private:
	unsigned int m_RenderedId;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();//<--Destructor, codigo custom al hacer clear

	void Bind();
	void Unbind();
};