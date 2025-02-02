#pragma once
#include <vector>
#include <unordered_map>

#include "slider.hpp"


class SliderWrapper
{
public:
    inline void addSlider(const char* label, float x, float y, float width, float height, float minValue, float maxValue, float initialValue);
    inline const Slider& getSlider(const char* label);

    void updateAndRender(const Vector2& mousePos, const Texture& texture);

private:
    std::vector<Slider> m_sliders;
    std::unordered_map<const char*, int> m_slidersIdx;
};
