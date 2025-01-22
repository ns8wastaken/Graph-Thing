#include "slider.hpp"


Slider::Slider(float x, float y, float width, float height)
    : sliderRect(Rectangle{ x, y, width, height })
{}


void Slider::render(const Texture& texture)
{
    const Shader& _sliderShader = *sliderShader;

    SetShaderValue(_sliderShader, GetShaderLocation(_sliderShader, "rectangle"), &sliderRect, SHADER_UNIFORM_VEC4);

    BeginShaderMode(_sliderShader);
    DrawTexture(texture, 0, 0, WHITE);
    EndShaderMode();


    // Vector2 pos = Vector2{ sliderRect.x + sliderCirclePos * sliderRect.width, sliderRect.y + sliderRect.height / 2.0f };
    Vector2 pos = Vector2{ 100.0f, 100.0f };
    SetShaderValue(_sliderShader, GetShaderLocation(_sliderShader, "position"), &pos, SHADER_UNIFORM_VEC2);

    BeginShaderMode(*circleShader);
    DrawTexture(texture, 0, 0, WHITE);
    EndShaderMode();

    DrawCircleV(pos, 5.0f, RED);
}


void Slider::update(const Vector2& mousePos)
{
}