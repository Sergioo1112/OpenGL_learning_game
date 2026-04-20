#pragma once
class IndexBuffer
{
private:
	unsigned int m_RenderedId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();//<--Destructor, codigo custom al hacer clear

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
};