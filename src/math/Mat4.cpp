#include "AugustEngine/math/Mat4.h"
#include <cmath>
#include <cstring>

namespace AugustEngine {
    // Constructors
    Mat4::Mat4() {
        for (int i = 0; i < 4 * 4; i++){
            elements[i] = 0.0f;
        }
        // creates an identity matrix
        elements[0 + 0 * 4] = 1.0f;
        elements[1 + 1 * 4] = 1.0f;
        elements[2 + 2 * 4] = 1.0f;
        elements[3 + 3 * 4] = 1.0f;
    }
    Mat4::Mat4(float diagonal){
        for (int i = 0; i < 4 * 4; i++){
            elements[i] = 0.0f;
        }
        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    Mat4 Mat4::identity() {
        return Mat4(); // returns a default identity matrix
    }

    Mat4& Mat4::multiply(const Mat4& other) {
        float data[16];
        for (int y = 0; y < 4; y++){
            for (int x = 0; x < 4; x++){
                float sum = 0.0f;
                for (int e = 0; e < 4; e++){
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                data[x + y * 4] = sum;
            }
        }
        memcpy(elements, data, 4 * 4 * sizeof(float));
        return *this;
    }

    // Operator overloads
    Mat4 operator*(Mat4 left, const Mat4& right){
        return left.multiply(right);
    }
    Mat4& Mat4::operator*=(const Mat4& other) {
        return multiply(other);
    }

    Mat4 Mat4::translation(const Vec3& translate) {
        Mat4 result(1.0f); // Start with an identity matrix
        result.elements[0 + 3 * 4] = translate.x;
        result.elements[1 + 3 * 4] = translate.y;
        result.elements[2 + 3 * 4] = translate.z;
        return result;
    }

    Mat4 Mat4::scale(const Vec3& scale) {
        Mat4 result(1.0f);
        result.elements[0 + 0 * 4] = scale.x;
        result.elements[1 + 1 * 4] = scale.y;
        result.elements[2 + 2 * 4] = scale.z;
        return result;
    }

    // Helper function needed for rotation
    inline float toRadians(float degrees) {
        return degrees * (3.14159265358979323846f / 180.0f);
    }

    Mat4 Mat4::rotation(float angle, const Vec3& axis) {
        Mat4 result(1.0f);
        float r = toRadians(angle);
        float c = cos(r);
        float s = sin(r);
        float omc = 1.0f - c;
        
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        result.elements[0 + 0 * 4] = x * x * omc + c;
        result.elements[1 + 0 * 4] = y * x * omc + z * s;
        result.elements[2 + 0 * 4] = x * z * omc - y * s;

        result.elements[0 + 1 * 4] = x * y * omc - z * s;
        result.elements[1 + 1 * 4] = y * y * omc + c;
        result.elements[2 + 1 * 4] = y * z * omc + x * s;

        result.elements[0 + 2 * 4] = x * z * omc + y * s;
        result.elements[1 + 2 * 4] = y * z * omc - x * s;
        result.elements[2 + 2 * 4] = z * z * omc + c;
        
        return result;
    }

    Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up){
        Vec3 f = (center - eye).normalize();    // forward direction
        Vec3 s = f.cross(up).normalize();       // side (right) direction
        Vec3 u = s.cross(f);                    // up direction (re-orthogonalized)

        Mat4 result(1.0f); // start with identity matrix

        // First column (right vector)
        result.elements[0 + 0 * 4] = s.x;
        result.elements[1 + 0 * 4] = s.y;
        result.elements[2 + 0 * 4] = s.z;

        // Second column (up vector)
        result.elements[0 + 1 * 4] = u.x;
        result.elements[1 + 1 * 4] = u.y;
        result.elements[2 + 1 * 4] = u.z;

        // Third column (forward vector - inverted for right-handed coordinate system)
        result.elements[0 + 2 * 4] = -f.x;
        result.elements[1 + 2 * 4] = -f.y;
        result.elements[2 + 2 * 4] = -f.z;

        // Translation part (camera position)
        result.elements[0 + 3 * 4] = -s.dot(eye);
        result.elements[1 + 3 * 4] = -u.dot(eye);
        result.elements[2 + 3 * 4] = f.dot(eye); // Note: f.dot(eye) because f is inverted

        return result;
    }

    Mat4 Mat4::perspective(float fov, float aspectRatio, float near, float far) {
        Mat4 result(1.0f);
        float q = 1.0f / tan(toRadians(0.5f * fov));
        float a = q / aspectRatio;
        float b = (near + far) / (near - far);
        float c = (2.0f * near * far) / (near - far);

        result.elements[0 + 0 * 4] = a;
        result.elements[1 + 1 * 4] = q;
        result.elements[2 + 2 * 4] = b;
        result.elements[3 + 2 * 4] = -1.0f;
        result.elements[2 + 3 * 4] = c;
        result.elements[3 + 3 * 4] = 0.0f; // This was missing
        
        return result;
    }

    Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
        Mat4 result(1.0f);
        result.elements[0 + 0 * 4] = 2.0f / (right - left);
        result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
        result.elements[2 + 2 * 4] = -2.0f / (far - near);
        result.elements[0 + 3 * 4] = -(right + left) / (right - left);
        result.elements[1 + 3 * 4] = -(top + bottom) / (top - bottom);
        result.elements[2 + 3 * 4] = -(far + near) / (far - near);
        return result;
    }

}