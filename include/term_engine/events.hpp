#pragma once

#include <ftxui/component/event.hpp>

#include <vector>

namespace tengine {
class Application;

//! Событие.
typedef ftxui::Event Event;

//! Класс, позволяющий получить доступ к событиям.
class EventReader {
    friend class Application;

  public:
    /*!
        @brief Активно ли событие.
        @param[in] event событие для проверки на активность.
        @return Активно событие или нет.
    */
    bool is_active(const Event event) const noexcept {
        for (const auto &key : m_events) {
            if (event == key) {
                return true;
            }
        }
        return false;
    }

    /*!
        @brief Получение массива активных событий.
        @return Массив событий.
    */
    const std::vector<Event> &get_events() const noexcept { return m_events; }

  protected:
    //! Массив событий.
    std::vector<Event> m_events;
};

} // namespace tengine
