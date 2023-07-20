#pragma once

#include <jsystem/mem.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using JVertexBufferID = GLuint;

// Buffer for storing vertex information. Despite the name, includes elements as well.
class JVertexBuffer
{
    JVertexBufferID vertexBuffer;
    JVertexBufferID vertexArray;
    JVertexBufferID elementBuffer;
    JPtr<uint8_t[]> vertexData; // For RW buffers.
    size_t vertexSize; // Total size of the vertex data in bytes.
    size_t indexSize; // Total size of the index data in bytes.
    size_t numVertices; // How many vertices are contained in the buffer.
    size_t numIndices; // How many indices are contained in the buffer.
    bool isRW = false; // If to allow RW access.
    bool mapped = false; // If memory is currently being mapped.

public:

    // Generate a new vertex buffer. Make sure the indices are u16s.
    JVertexBuffer(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t numVertices);

    // Create a vertex buffer that can be read/written to. Automatically generates indices. Set the quad indices to generate indices for quads.
    JVertexBuffer(size_t singleVertexSize, size_t numVertices, size_t numIndices, bool quadIndices);

    // Bind the vertex buffer.
    void Bind();

    // Fetch data from a vertex buffer. Note that this may bind the vertex buffer.
    void FetchData(void* vertexDataOut, void* indexDataOut);

    // Map buffer data to local memory. Note that this must be an RW buffer for this and the buffer may be bound.
    void Map(void*& vertexPtr, void*& indexPtr);

    // Unmap memory. Note that memory must be mapped first.
    void Unmap();

    // Get size of vertex data in bytes.
    size_t VertexSize() const;

    // Get size of index data in bytes.
    size_t IndexSize() const;

    // Get number of vertices.
    size_t VertexCount() const;

    // Get number of indices.
    size_t IndexCount() const;

    // Set the index size for RW buffers.
    void SetIndexCount(size_t num);

    // Destroy buffer.
    ~JVertexBuffer();

};