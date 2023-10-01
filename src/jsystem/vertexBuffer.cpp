#include <jsystem/vertexBuffer.hpp>

#include <cassert>
#include <vector>

JVertexBuffer::JVertexBuffer(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t numVertices) : vertexSize(vertexSize), indexSize(indexSize), numVertices(numVertices)
{
    ZoneScopedN("JVertexBuffer::JVertexBuffer");
    numIndices = indexSize / 2;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexData, GL_STATIC_DRAW);
}

JVertexBuffer::JVertexBuffer(size_t singleVertexSize, size_t numVertices, size_t numIndices, bool quads) : vertexSize(singleVertexSize * numVertices), indexSize(numIndices * sizeof(unsigned short)), numVertices(numVertices), numIndices(numIndices)
{
    ZoneScopedN("JVertexBuffer::JVertexBuffer");
    isRW = true;
    vertexData = JPtrMake(uint8_t[], vertexSize);
    std::vector<unsigned short> indexData;
    if (quads)
    {
        unsigned short currVertex = 0;
        for (unsigned int i = 0; i < numIndices / 6; i++)
        {
            indexData.push_back(currVertex);
            indexData.push_back(currVertex + 1);
            indexData.push_back(currVertex + 2);
            indexData.push_back(currVertex);
            indexData.push_back(currVertex + 2);
            indexData.push_back(currVertex + 3);
            currVertex += 4;
        }
    }
    else
    {
        for (unsigned int i = 0; i < numIndices; i++)
        {
            indexData.push_back(i);
        }
    }
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData.get(), GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexData.data(), GL_STATIC_DRAW);
}

void JVertexBuffer::Bind()
{
    ZoneScopedN("JVertexBuffer::Bind");
    glBindVertexArray(vertexArray);
}

void JVertexBuffer::FetchData(void* vertexDataOut, void* indexDataOut)
{
    ZoneScopedN("JVertexBuffer::FetchData");
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertexDataOut);
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, indexDataOut);
}

void JVertexBuffer::Map(void*& vertexPtr, void*& indexPtr)
{
    ZoneScopedN("JVertexBuffer::Map");
    if (!isRW || mapped) return;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    vertexPtr = vertexData.get();
    indexPtr = nullptr;
    mapped = true;
}

void JVertexBuffer::Unmap() // TODO: ONLY UPLOAD WHAT WE NEED, NOT ALL!
{
    ZoneScopedN("JVertexBuffer::Unmap");
    if (!mapped) return;
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData.get(), GL_STREAM_DRAW); // TODO: Look into double buffering the uploads?
    mapped = false;
}

size_t JVertexBuffer::VertexSize() const
{
    ZoneScopedN("JVertexBuffer::VertexSize");
    return vertexSize;
}

size_t JVertexBuffer::IndexSize() const
{
    ZoneScopedN("JVertexBuffer::IndexSize");
    return indexSize;
}

size_t JVertexBuffer::VertexCount() const
{
    ZoneScopedN("JVertexBuffer::VertexCount");
    return numVertices;
}

size_t JVertexBuffer::IndexCount() const
{
    ZoneScopedN("JVertexBuffer::IndexCount");
    return numIndices;
}

void JVertexBuffer::SetIndexCount(size_t num)
{
    ZoneScopedN("JVertexBuffer::SetIndexCount");
    assert(isRW);
    numIndices = num;
}

JVertexBuffer::~JVertexBuffer()
{
    ZoneScopedN("JVertexBuffer::~JVertexBuffer");
    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &elementBuffer);
}