#pragma once
#include <raylib.h>
#include <vector>

#include "utils.hpp"


struct Node
{
    Vector2 pos  = { 0.0f, 0.0f };
    Vector2 vel  = { 0.0f, 0.0f };
    float radius = 0.0f;
    Color color;

    static Vector2* screenRes;

    void updatePos();
    void push(const Vector2& mousePos);
};

Vector2* Node::screenRes = nullptr;
