#ifndef CAMERA_H
#define CAMERA_H

#include "math/Mat4.h"
#include "math/Vec3.h"

namespace AugustEngine {
    class Camera{
    public:
        Vec3 position;
        Vec3 front;
        Vec3 up;
        Vec3 right;
        Vec3 worldUp;

        float yaw;
        float pitch;

        float movementSpeed;
        float mouseSensitivity;
        float zoom;

        Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
        Camera(float posX, float poxY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        Mat4 getViewMatrix() const;
        Mat4 getProjectionMatrix(float aspectRatio) const;

        void processKeyboard(int direction, float deltaTime); // Placeholder for input
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void processMouseScroll(float yoffset);
    private:
        void updateCameraVectors();
    };
}

#endif // CAMERA_H