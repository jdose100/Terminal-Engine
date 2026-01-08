//! @namespace tengine Содержит всё, что касается реализации игрового движка
//! Terminal Engine.

#pragma once

#include <ftxui/screen/color.hpp>
#include <ftxui/screen/pixel.hpp>

#include <vector>
// #include <vect>

namespace tengine {

//! Минимально отрисовываемый пиксель.
struct Pixel : public ftxui::Pixel {
    //! Координата x, является относительной от сущности.
    int x = 0;

    //! Координата y, является относительной от сущности.
    int y = 0;

    /*!
        @brief Создание пикселя в координатах t_x и t_y.
        @param[in] t_x координата x.
        @param[in] t_y координата y.
    */
    Pixel(int t_x, int t_y) : x(t_x), y(t_y) {}
    Pixel() {}
};

//! Изображение для отрисовки.
using Image = std::vector<Pixel>;

//! Цвет.
using Color = ftxui::Color;

} // namespace tengine
