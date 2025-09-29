#ifndef VEC3_H
#define VEC3_H

namespace AugustEngine {
    class Vec3 {
        public:
            float x,y,z;

            // Constructors
            Vec3();
            Vec3(float x, float y, float z);

            // Vector Operations
            Vec3& add(const Vec3& other);
            Vec3& sub(const Vec3& other);
            Vec3& mult(const Vec3& other);
            Vec3& div(const Vec3& other);

            // Operator overloads for convenience
            friend Vec3 operator+(Vec3 left, const Vec3& right);
            friend Vec3 operator-(Vec3 left, const Vec3& right);
            friend Vec3 operator*(Vec3 left, const Vec3& right);
            friend Vec3 operator/(Vec3 left, const Vec3& right);

            Vec3& operator+=(const Vec3& other);
            Vec3& operator-=(const Vec3& other);
            Vec3& operator*=(const Vec3& other);
            Vec3& operator/=(const Vec3& other);

            bool operator==(const Vec3& other) const;
            bool operator!=(const Vec3& other) const;

            // Scalar operations
            Vec3& multiply(float scalar);

            // Other useful vector math
            float length() const;
            Vec3& normalize();
            float dot(const Vec3& other) const;
            Vec3 cross(const Vec3& other) const;
    };
}

#endif // VEC3_H