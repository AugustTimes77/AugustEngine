#include "AugustEngine/Camera.h"
#include <cmath>

namespace AugustEngine {
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    Camera::Camera(Vec3 position, Vec3 up, float yaw, float pitch)
        : front(Vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
            this->position = position;
            this->worldUp = up;
            this->yaw = yaw;
            this->pitch = pitch;
            updateCameraVectors();
    }
    // Overloaded constructor with individual float components
    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : front(Vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = Vec3(posX, posY, posZ);
        this->worldUp = Vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors(); // Calculate front, right, up vectors
    }
    void Camera::updateCameraVectors() {
        // calculate the new front vector
        Vec3 newFront;
        newFront.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
        newFront.y = sin(toRadians(pitch));
        newFront.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
        front = newFront.normalize();

        // also re-calc the Right and Up vector
        // normalize the vectors because their length gets closer to 0 the more you look up or down
        // this results in slower movement
        right = front.cross(worldUp).normalize();
        up = right.cross(front).normalize();
    }
    inline float toRadians(float degrees) {
        return degrees * (3.1415926735 / 180.0f);
    }
}