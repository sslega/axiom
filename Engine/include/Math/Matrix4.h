#pragma once
#include "Math/Vector.h"
#include <cmath>

namespace axiom
{
    // Column-major 4x4 matrix. data[col * 4 + row].
    // Matches OpenGL's expected memory layout — pass data directly to glUniformMatrix4fv with transpose=GL_FALSE.
    // Coordinate convention: right-handed, NDC z: [-1, 1] (OpenGL).
    struct Matrix4
    {
        float data[16];

        Matrix4()
        {
            for (int i = 0; i < 16; ++i) data[i] = 0.0f;
        }

        float& operator()(int row, int col)       { return data[col * 4 + row]; }
        float  operator()(int row, int col) const  { return data[col * 4 + row]; }

        Matrix4 operator*(const Matrix4& b) const
        {
            Matrix4 c;
            for (int col = 0; col < 4; ++col)
                for (int row = 0; row < 4; ++row)
                    for (int k = 0; k < 4; ++k)
                        c(row, col) += (*this)(row, k) * b(k, col);
            return c;
        }

        // -----------------------------------------------------------------------
        // Factory methods
        // -----------------------------------------------------------------------

        static Matrix4 Identity()
        {
            Matrix4 m;
            m(0,0) = m(1,1) = m(2,2) = m(3,3) = 1.0f;
            return m;
        }

        // Orthographic projection (right-handed, NDC z: [-1, 1])
        static Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far)
        {
            Matrix4 m;
            m(0,0) =  2.0f / (right - left);
            m(1,1) =  2.0f / (top - bottom);
            m(2,2) = -2.0f / (far - near);
            m(0,3) = -(right + left) / (right - left);
            m(1,3) = -(top + bottom) / (top - bottom);
            m(2,3) = -(far + near)   / (far - near);
            m(3,3) =  1.0f;
            return m;
        }

        // Perspective projection (right-handed, NDC z: [-1, 1])
        // fovY: vertical field of view in radians
        static Matrix4 Perspective(float fovY, float aspect, float near, float far)
        {
            float f = 1.0f / std::tan(fovY * 0.5f);
            Matrix4 m;
            m(0,0) =  f / aspect;
            m(1,1) =  f;
            m(2,2) =  (far + near) / (near - far);
            m(2,3) =  (2.0f * far * near) / (near - far);
            m(3,2) = -1.0f;
            return m;
        }

        // View matrix: positions the camera at eye, looking toward center, with worldUp as the up hint
        static Matrix4 LookAt(Vec3 eye, Vec3 center, Vec3 worldUp)
        {
            Vec3 forward = Normalize(Vec3(center.x - eye.x, center.y - eye.y, center.z - eye.z));
            Vec3 right   = Normalize(Cross(forward, Normalize(worldUp)));
            Vec3 up      = Cross(right, forward);

            Matrix4 m;
            m(0,0) =  right.x;   m(0,1) =  right.y;   m(0,2) =  right.z;   m(0,3) = -Dot(right, eye);
            m(1,0) =  up.x;      m(1,1) =  up.y;      m(1,2) =  up.z;      m(1,3) = -Dot(up, eye);
            m(2,0) = -forward.x; m(2,1) = -forward.y; m(2,2) = -forward.z; m(2,3) =  Dot(forward, eye);
            m(3,3) =  1.0f;
            return m;
        }

        static Matrix4 Translate(Vec3 t)
        {
            Matrix4 m = Identity();
            m(0,3) = t.x;
            m(1,3) = t.y;
            m(2,3) = t.z;
            return m;
        }

        static Matrix4 Scale(Vec3 s)
        {
            Matrix4 m;
            m(0,0) = s.x;
            m(1,1) = s.y;
            m(2,2) = s.z;
            m(3,3) = 1.0f;
            return m;
        }

        // Rotate around an arbitrary axis by angle (radians)
        static Matrix4 Rotate(Vec3 axis, float angle)
        {
            Vec3  a = Normalize(axis);
            float c = std::cos(angle);
            float s = std::sin(angle);
            float t = 1.0f - c;
            float x = a.x, y = a.y, z = a.z;

            Matrix4 m;
            m(0,0) = t*x*x + c;     m(0,1) = t*x*y - s*z;  m(0,2) = t*x*z + s*y;  m(0,3) = 0;
            m(1,0) = t*x*y + s*z;   m(1,1) = t*y*y + c;     m(1,2) = t*y*z - s*x;  m(1,3) = 0;
            m(2,0) = t*x*z - s*y;   m(2,1) = t*y*z + s*x;   m(2,2) = t*z*z + c;    m(2,3) = 0;
            m(3,3) = 1.0f;
            return m;
        }
    };
}
