#include "slider.hpp"


Slider::Slider(float x, float y, float width, float height, float minValue, float maxValue)
    : sliderRect(Rectangle{x, y, width, height}), m_min(minValue), m_max(maxValue)
{}


void Slider::render(const Texture& texture)
{
    const Shader& _sliderShader = *sliderShader;

    SetShaderValue(_sliderShader, GetShaderLocation(_sliderShader, "rectangle"), &sliderRect, SHADER_UNIFORM_VEC4);

    // Draw slider
    BeginShaderMode(_sliderShader);
    DrawTexture(texture, 0, 0, WHITE);
    EndShaderMode();

    const Shader& _circleShader = *circleShader;

    Vector2 pos = Vector2{sliderRect.x + m_sliderCirclePos, sliderRect.y + sliderRect.height / 2.0f};

    SetShaderValue(_circleShader, GetShaderLocation(_circleShader, "position"), &pos, SHADER_UNIFORM_VEC2);
    SetShaderValue(_circleShader, GetShaderLocation(_circleShader, "radius"), &sliderRect.height, SHADER_UNIFORM_FLOAT);

    // Draw slider circle
    BeginShaderMode(_circleShader);
    DrawTexture(texture, 0, 0, RED);
    EndShaderMode();

    // Draw slider value
    const char* text = TextFormat("%i", static_cast<int>(getValue()));
    const Font font  = GetFontDefault();
    DrawText(
        text,
        sliderRect.x + sliderRect.width + 35,
        sliderRect.y + (sliderRect.height - MeasureTextEx(font, text, 20.0f, font.glyphPadding).y) / 2.0f,
        20,
        WHITE
    );
}


void Slider::update(const Vector2& mousePos)
{
    Vector2 pos = Vector2{sliderRect.x + m_sliderCirclePos, sliderRect.y + sliderRect.height / 2.0f};
    float dist  = Vector2Dist(mousePos, pos);

    if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && dist < sliderRect.height)
        m_moving = true;
    if (IsMouseButtonReleased(MouseButton::MOUSE_BUTTON_LEFT))
        m_moving = false;

    if (m_moving)
        m_sliderCirclePos = Clamp(mousePos.x - sliderRect.x, 0.0f, sliderRect.width);
}


float Slider::getValue()
{
    return (m_sliderCirclePos / sliderRect.width) * m_max + m_min;
}
