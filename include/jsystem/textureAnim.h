#pragma once

#include <constants.h>
#include <glm/glm.hpp>
#include <vector>

// For animating textures.
struct JTextureAnim
{

    // Update properties.
    virtual void Update() = 0;

    // Get the texture coordinate transformation matrix.
    virtual glm::mat3 GetMtx() = 0;

    // Destructor.
    virtual ~JTextureAnim() {}

};