#pragma once
#include <raylib.h>
#include <math.h>


float Vector2Dist(Vector2 v1, Vector2 v2)
{
    return sqrtf(((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y)));
}


template <typename T>
[[nodiscard]] inline constexpr T Clamp(T v, T lo, T hi)
{
    return (v < lo) ? lo
         : (hi < v) ? hi
                    : v;
}

template <typename T>
[[nodiscard]] inline constexpr T Min(T a, T b)
{
    return (a < b) ? a : b;
}

template <typename T>
[[nodiscard]] inline constexpr T Max(T a, T b)
{
    return (b < a) ? a : b;
}
