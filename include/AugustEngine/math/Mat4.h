#ifndef MAT4_H
#define MAT4_H

#include "Vec3.h"

namespace AugustEngine {

    class Mat4 {
    public:
        float elements[4*4];

        Mat4(); // constructor initializes to identity matrix
        Mat4(float diagonal);

        static Mat4 identity();

        Mat4& multiply(const Mat4& other);
        friend Mat4 operator*(Mat4 left, const Mat4& right);
        Mat4& operator*=(const Mat4& other);

        // Transformation matricies
        static Mat4 translation(const Vec3& translate);
        static Mat4 rotation(float angle, const Vec3& axis);
        static Mat4 scale(const Vec3& scale);

        // Projection matricies
        static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
        static Mat4 perspective(float fov, float aspectRatio, float near, float far);
    };
}

#endif