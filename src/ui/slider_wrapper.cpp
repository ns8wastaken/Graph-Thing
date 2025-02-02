#include "slider_wrapper.hpp"


inline void SliderWrapper::addSlider(const char* label, float x, float y, float width, float height, float minValue, float maxValue, float initialValue)
{
    m_slidersIdx[label] = m_sliders.size();
    m_sliders.push_back(Slider(x, y, width, height, minValue, maxValue, initialValue));
}


inline const Slider& SliderWrapper::getSlider(const char* label)
{
    return m_sliders[m_slidersIdx[label]];
}

void SliderWrapper::updateAndRender(const Vector2& mousePos, const Texture& texture)
{
    for (Slider& slider : m_sliders) {
        slider.update(mousePos);
        slider.render(texture);
    }
}
