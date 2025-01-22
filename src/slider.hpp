#pragma once
#include <raylib.h>


struct Slider
{
    Slider(float x, float y, float width, float height);
    Rectangle sliderRect;
    float sliderCirclePos;

    static Vector2* sliderArea;
    static Shader* sliderShader;
    static Shader* circleShader;

    void render(const Texture& texture);
    void update(const Vector2& mousePos);
};

Vector2* Slider::sliderArea  = nullptr;
Shader* Slider::sliderShader = nullptr;
Shader* Slider::circleShader = nullptr;
