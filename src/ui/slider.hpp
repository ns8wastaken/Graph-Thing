#pragma once
#include <raylib.h>

#include "../utils.hpp"


class Slider
{
public:
    Slider(float x, float y, float width, float height, float minValue, float maxValue, float initialValue);
    Rectangle sliderRect;

    float getValue() const;

    static Vector2* sliderArea;
    static Shader* sliderShader;
    static Shader* circleShader;

    void update(const Vector2& mousePos);
    void render(const Texture& texture);

private:
    bool m_moving = false;

    float m_min = 0.0f;
    float m_max = 0.0f;

    float m_sliderCirclePos;
};

Vector2* Slider::sliderArea  = nullptr;
Shader* Slider::sliderShader = nullptr;
Shader* Slider::circleShader = nullptr;
