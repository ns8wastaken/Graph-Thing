#pragma once
#include <raylib.h>

#include "utils.hpp"


class Slider
{
public:
    Slider(float x, float y, float width, float height, float minValue, float maxValue);
    Rectangle sliderRect;

    float getValue();

    static Vector2* sliderArea;
    static Shader* sliderShader;
    static Shader* circleShader;

    void render(const Texture& texture);
    void update(const Vector2& mousePos);

private:
    float m_sliderCirclePos = 0.0f;

    bool m_moving = false;

    float m_min = 0.0f;
    float m_max = 0.0f;
};

Vector2* Slider::sliderArea  = nullptr;
Shader* Slider::sliderShader = nullptr;
Shader* Slider::circleShader = nullptr;
