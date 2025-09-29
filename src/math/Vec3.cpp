#include "AugustEngine/math/Vec3.h"
#include <cmath>

namespace AugustEngine{
    // Constructors
    Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {
        // body is empty because the initializer sets the variables
    }
    Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {
        // initialize the variables with the given 
    }

    // Vector operations
    Vec3& Vec3::add(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this; // return a reference to the modified vector
    }
    Vec3& Vec3::sub(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vec3& Vec3::mult(const Vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    Vec3& Vec3::div(const Vec3& other) {
        // note, there is no check for division by 0
        // the user needs to make sure not to pass a 0
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    // Operator overloads for convenience
    Vec3 operator+(Vec3 left, const Vec3& right) {
        return left.add(right);
    }
    Vec3 operator-(Vec3 left, const Vec3& right) {
        return left.sub(right);
    }
    Vec3 operator*(Vec3 left, const Vec3& right) {
        return left.mult(right);
    }
    Vec3 operator/(Vec3 left, const Vec3& right) {
        return left.div(right);
    }

    Vec3& Vec3::operator+=(const Vec3& other) {
        return add(other);
    }

    Vec3& Vec3::operator-=(const Vec3& other) {
        return sub(other);
    }

    Vec3& Vec3::operator*=(const Vec3& other) {
        return mult(other);
    }

    Vec3& Vec3::operator/=(const Vec3& other) {
        return div(other);
    }

    bool Vec3::operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Vec3::operator!=(const Vec3& other) const {
        return !(*this == other); // Reuse the == operator we just defined
    }

    // Scalar operations
    Vec3& Vec3::multiply(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // other useful vector math
    float Vec3::length() const {
        // pythagorem theorm in 3D
        return sqrtf(x * x + y * y + z * z);
    }

    Vec3& Vec3::normalize() {
        float len = length();
        // avoid division by 0
        if (len != 0) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }

    float Vec3::dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 Vec3::cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
}