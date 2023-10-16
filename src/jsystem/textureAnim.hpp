#pragma once

#include <glm/glm.hpp>
#include <vector>

// For animating textures.
class JTextureAnim
{
public:

    // Update properties.
    virtual void Update() = 0;

    // Get the texture coordinate transformation matrix.
    virtual glm::mat3 GetMtx() const = 0;

    // Destructor.
    virtual ~JTextureAnim() {}

};