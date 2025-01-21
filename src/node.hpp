#pragma once
#include <raylib.h>
#include <vector>


struct Node
{
    Vector2 pos  = {0.0f, 0.0f};
    Vector2 vel  = {0.0f, 0.0f};
    float radius = 0.0f;
    Color color;

    void updatePos(const Vector2& screenRes)
    {
        Vector2 newPos = Vector2Add(pos, vel);

        if (newPos.x < radius || newPos.x >= screenRes.x - radius)
            vel.x = -vel.x;

        if (newPos.y < radius || newPos.y >= screenRes.y - radius)
            vel.y = -vel.y;

        pos.x += vel.x;
        pos.y += vel.y;
    }
};
