#pragma once

#include <jsystem/vertexBuffer.hpp>
#include <memory>

// A mesh that can be drawn. Holds a vertex buffer to draw as well as a material index that is used as a suggestion for a material to be bound.
struct JMesh
{
    JPtr<JVertexBuffer> buffer;
    unsigned int materialIndex;

    // Create a new mesh. Note material index does nothing, it is simply for convenience.
    JMesh(JPtr<JVertexBuffer> buffer, unsigned int materialIndex) : buffer(std::move(buffer)), materialIndex(materialIndex) {}

    // Render a mesh.
    void Render()
    {
        buffer->Bind();
        glDrawElements(GL_TRIANGLES, buffer->IndexCount(), GL_UNSIGNED_SHORT, 0);
    }

};