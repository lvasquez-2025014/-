#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <string>
#include "imgui.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variables de Selección
inline uint32_t selectedEntity = 0;
inline std::string selectedName = "";
inline float selectedDist = 0.0f;

struct Vector2 {
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(float value) : x(value), y(value) {}

    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }

    float Magnitude() const { return sqrtf(x * x + y * y); }
    static float Distance(const Vector2& a, const Vector2& b) {
        return sqrtf(powf(a.x - b.x, 2.0f) + powf(a.y - b.y, 2.0f));
    }

    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator*(float f) const { return Vector2(x * f, y * f); }
    Vector2 operator/(float f) const { return Vector2(x / f, y / f); }
};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(float value) : x(value), y(value), z(value) {}

    static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }

    float Magnitude() const { return sqrtf(x * x + y * y + z * z); }
    static float Distance(const Vector3& a, const Vector3& b) {
        return sqrtf(powf(a.x - b.x, 2.0f) + powf(a.y - b.y, 2.0f) + powf(a.z - b.z, 2.0f));
    }

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }
    Vector3 operator/(float f) const { return Vector3(x / f, y / f, z / f); }
    
    bool IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }

    static float Dot(const Vector3& lhs, const Vector3& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }
};

struct Vector4 {
    float x, y, z, w;
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

struct Quaternion {
    float x, y, z, w;
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float cosHalfTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
        
        Quaternion q2_correct = q2;
        if (cosHalfTheta < 0.0f) {
            q2_correct.w = -q2.w;
            q2_correct.x = -q2.x;
            q2_correct.y = -q2.y;
            q2_correct.z = -q2.z;
            cosHalfTheta = -cosHalfTheta;
        }

        if (cosHalfTheta >= 1.0f) {
            return q1;
        }

        float halfTheta = acosf(cosHalfTheta);
        float sinHalfTheta = sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

        if (fabsf(sinHalfTheta) < 0.001f) {
            return Quaternion(
                q1.x * (1.0f - t) + q2_correct.x * t,
                q1.y * (1.0f - t) + q2_correct.y * t,
                q1.z * (1.0f - t) + q2_correct.z * t,
                q1.w * (1.0f - t) + q2_correct.w * t
            );
        }

        float ratioA = sinf((1.0f - t) * halfTheta) / sinHalfTheta;
        float ratioB = sinf(t * halfTheta) / sinHalfTheta;

        return Quaternion(
            q1.x * ratioA + q2_correct.x * ratioB,
            q1.y * ratioA + q2_correct.y * ratioB,
            q1.z * ratioA + q2_correct.z * ratioB,
            q1.w * ratioA + q2_correct.w * ratioB
        );
    }
};

struct Matrix4x4 {
    float m[4][4];
};

inline ImVec2 WorldToScreenImVec2(const Matrix4x4& mat, const Vector3& worldPos, int width, int height) {
    float w = worldPos.x * mat.m[0][3] + worldPos.y * mat.m[1][3] + worldPos.z * mat.m[2][3] + mat.m[3][3];
    if (w < 0.001f) return ImVec2(-1.0f, -1.0f);

    float x = worldPos.x * mat.m[0][0] + worldPos.y * mat.m[1][0] + worldPos.z * mat.m[2][0] + mat.m[3][0];
    float y = worldPos.x * mat.m[0][1] + worldPos.y * mat.m[1][1] + worldPos.z * mat.m[2][1] + mat.m[3][1];

    float nx = x / w;
    float ny = y / w;

    return ImVec2((width / 2.0f) + (nx * width / 2.0f), (height / 2.0f) - (ny * height / 2.0f));
}

namespace AimBv2 {
    inline Vector2 GetRotationToLocation(const Vector3& targetPos, float lerp, const Vector3& camPos) {
        Vector3 delta = targetPos - camPos;
        float dist = delta.Magnitude();
        
        float yaw = atan2f(delta.x, delta.z) * (180.0f / (float)M_PI);
        float pitch = -asinf(delta.y / dist) * (180.0f / (float)M_PI);

        return Vector2(pitch, yaw);
    }
}

namespace AimBZv2 {
    inline Quaternion GetRotationToLocation(const Vector3& targetPos, float lerp, const Vector3& camPos) {
        Vector3 delta = targetPos - camPos;
        float dist = delta.Magnitude();
        if (dist < 0.01f) return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        float yaw = atan2f(delta.x, delta.z);
        float pitch = -asinf(delta.y / dist);

        float cy = cosf(yaw * 0.5f);
        float sy = sinf(yaw * 0.5f);
        float cp = cosf(pitch * 0.5f);
        float sp = sinf(pitch * 0.5f);

        Quaternion q;
        q.w = cp * cy;
        q.x = sp * cy;
        q.y = cp * sy;
        q.z = -sp * sy;
        return q;
    }
}
