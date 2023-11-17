#pragma once

#include "mesh.hpp"
#include <glm/glm.hpp>

// Render batch mode.
enum class JRenderBatchMode
{
    Lines,
    Triangles,
    Quads
};

#define VERTICES_PER_TRI 3
#define VERTICES_PER_QUAD 4
#define INDICES_PER_TRI 3
#define INDICES_PER_QUAD 6

// For rendering multiple items that can change a lot each frame. Implementation in header since it's generic to the vertex. Note you can NOT use a render batch with a render graph, since a renderbatch requires its contents to be draw immediately. It's ok, render batch users take advantage of little state changes to begin with.
template <typename T>
class JRenderBatch
{
    size_t maxNumPrimitives;
    JRenderBatchMode mode;
    JPtr<JMesh> mesh;
    unsigned int currVertex = 0;
    T* vtxMemory = nullptr;
    bool active = false;

public:

    // Create a new render batch.
    JRenderBatch(size_t maxNumPrimitives, JRenderBatchMode mode) : maxNumPrimitives(maxNumPrimitives), mode(mode)
    {
        ZoneScopedN("JRenderBatch::JRenderBatch"); // Exception of these in header files since code can take longer here.
        if (mode == JRenderBatchMode::Triangles)
        {
            mesh = JPtrMake(JMesh,
                JPtrMake(JVertexBuffer, sizeof(T), maxNumPrimitives * VERTICES_PER_TRI, maxNumPrimitives * INDICES_PER_TRI, false),
                0
            );
        }
        else if (mode == JRenderBatchMode::Lines || mode == JRenderBatchMode::Quads)
        {
            mesh = JPtrMake(JMesh,
                JPtrMake(JVertexBuffer, sizeof(T), maxNumPrimitives * VERTICES_PER_QUAD, maxNumPrimitives * INDICES_PER_QUAD, true),
                0
            );
        }
        else
        {
            DBG_PRINT("JRENDERBATCH@JUNO: Invalid primitive type!");
        }
        T::SetBufferAttributes(*mesh->buffer);
    }

    // Begin writing to the render batch.
    void Begin()
    {
        ZoneScopedN("JRenderBatch::Begin");
        assert(!active);
        void* vtxTmp = nullptr;
        void* indTmp = nullptr;
        mesh->buffer->Map(vtxTmp, indTmp);
        vtxMemory = (T*)vtxTmp;
        active = true;
    }

    // Add a vertex to the batch. Returns false if overflow. Only use this for triangles and quads.
    bool AddVertex(const T& vtx)
    {
        ZoneScopedN("JRenderBatch::AddVertex");
        assert(active);
        if (vtxMemory && currVertex < mesh->buffer->VertexCount())
        {
            *vtxMemory = vtx;
            currVertex++;
            vtxMemory++;
            return true;
        }
        else return false;
    }

    // Add a vertex pair to the batch. Returns false if overflow. Only use this for lines. Source: https://prideout.net/blog/old/blog/index.html@tag=opengl-silhouette.html
    bool AddLine(const T& a, const T& b, const glm::vec3& cameraPos, float thickness)
    {
        ZoneScopedN("DScenarioEmitEdge");
        assert(mode == JRenderBatchMode::Lines || mode == JRenderBatchMode::Quads);
        assert(active);

        /*
            Gota's Guide To Silhoutte Lines:
                - By Gota7 (Obviously)

            Despite giving a source, that only covers the case for 2d. How 3d works is slightly different.
            Imagine we have a line that faces the camera:

                  /
                 /
                /

            If we want to give it "thickness", we need to draw a quad where the thickness is in the direction ortho-normal to the point to the camera and to the line.

                \
                 \

            We want the thickness to go this way basically.

                  /\
                 / /
                / /
                \/

            Putting this thickness together leads to a line that looks like this (note that the lines are not truly orthonormal like they should be).

        */
        glm::vec3 n = glm::normalize(glm::cross(    // Need to find orthonormal direction to line.
            a.pos - b.pos,                          // Direction of the line.
            (a.pos + b.pos) / 2.0f - cameraPos)     // Direction of the camera to midpoint of line.
        ) * thickness / 2.0f;
        T a0, a1;
        a0 = a;
        a1 = a;
        a0.pos = a.pos - n;
        a1.pos = a.pos + n;
        T b0, b1;
        b0 = b;
        b1 = b;
        b0.pos = b.pos - n;
        b1.pos = b.pos + n;
        AddVertex(a0); // Bottom left.
        AddVertex(a1); // Top left.
        AddVertex(b1); // Top right.
        return AddVertex(b0); // Bottom right.

    }

    // Render and flush the batch.
    void RenderAndFlush()
    {
        ZoneScopedN("JRenderBatch::RenderAndFlush");
        assert(active);
        mesh->buffer->Unmap();
        if (mode == JRenderBatchMode::Triangles)
        {
            mesh->buffer->SetIndexCount((currVertex / VERTICES_PER_TRI) * INDICES_PER_TRI);
        }
        else if (mode == JRenderBatchMode::Lines || mode == JRenderBatchMode::Quads)
        {
            mesh->buffer->SetIndexCount((currVertex / VERTICES_PER_QUAD) * INDICES_PER_QUAD);
        }
        if (mesh->buffer->IndexCount() > 0) mesh->Render();
        currVertex = 0;
        vtxMemory = nullptr;
        active = false;
    }

};