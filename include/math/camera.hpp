#pragma once

#include <common.hpp>
#include <jsystem/uniformBuffer.hpp>
#include <jsystem/window.hpp>
#include <math/angle.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

// Camera structure.
class MCamera
{
    JWindow& window;
    glm::vec3 cameraPos = glm::vec3(0.0f); // Base position.
    glm::vec3 cameraFront; // Facing dir. Generate this.
    glm::vec3 cameraUp; // Up vector. Generate this.
    glm::vec3 cameraSide; // To the right of the camera. Generate this.
    glm::mat4 projection; // Projection matrix. Generated.
    glm::mat4 view; // View matrix. Generated.
    float nearClip = 0.1f; // Closest position to the viewer.
    float farClip = 1000.0f; // Furthest position to the viewer.
    bool needsViewMatrixUpdate = true; // If the camera needs to update its view matrix. Use this in the update function.
    bool needsProjectionMatrixUpdate = true; // If the camera needs to update is projection matrix. User this in the update function.
    bool needsProjectionUniformSet = true; // If there was a change in matrices and the shader needs them set. Generated.
    bool needsViewUniformSet = true; // If there was a change in matrices and the shader needs them set. Generated.
    
    // Make a new camera.
    MCamera(JWindow& window) : window(window) {}

    // Update the projection matrix. TODO: FIGURE OUT HOW TO UPDATE THIS CORRECTLY!!!!
    void UpdateProjectionMatrix(JWindow& window)
    {
        int width, height;
        window.GetSize(width, height);
        projection = glm::ortho(
            0.0f,
            (float)width,
            0.0f,
            (float)height, // Might need to be swapped with parameter above.
            nearClip,
            farClip
        );
        needsProjectionMatrixUpdate = false;
        needsProjectionUniformSet = true;
    }

    // Update view matrix with look at. Offset is the offset for the right eye.
    void UpdateViewMatrixWithParams(const glm::vec3& target, const glm::vec3& up)
    {
        view = glm::lookAt(cameraPos, target, up);
        needsViewMatrixUpdate = false;
        needsViewUniformSet = true;
    }

    // Update view matrix using camera directions.
    void UpdateViewMatrixWithDirections()
    {
        UpdateViewMatrixWithParams(cameraPos + cameraFront, cameraUp);
    }

public:

    // Set camera matrix if required.
    void SetUniforms(JUniformBuffer& uniforms)
    {
        if (needsProjectionUniformSet) uniforms.SetMat4("projection", projection);
        if (needsViewUniformSet) uniforms.SetMat4("view", view);
        needsProjectionUniformSet = false;
        needsViewUniformSet = false;
    }

    // Update the camera directions, projection, and view automatically. Use the protected variables above for this.
    virtual void Update() = 0;
    
    // Get camera position.
    glm::vec3 GetCameraPos()
    {
        return cameraPos;
    }

    // Set camera position.
    void SetCameraPos(const glm::vec3& pos)
    {
        cameraPos = pos;
        needsViewMatrixUpdate = true;
    }

    // Get front direction.
    glm::vec3 GetFront()
    {
        return cameraFront;
    }

    // Get up direction.
    glm::vec3 GetUp()
    {
        return cameraUp;
    }

    // Get size.
    glm::vec3 GetSide()
    {
        return cameraSide;
    }

};