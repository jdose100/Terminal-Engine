//! @extends term_engine/application.hpp

#include "term_engine/application.hpp"
#include "term_engine/entity.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <thread>
#include <vector>

using tengine::Application;
using namespace std;

//! Экран для вывода. Живёт на протяжении всей программы.
static ftxui::ScreenInteractive screen =
    ftxui::ScreenInteractive::TerminalOutput();

Application *Application::singleton() noexcept {
    static Application instance;
    return &instance;
}

void Application::run() {
    using milliseconds = std::chrono::duration<double, milli>;
    static const constexpr milliseconds target_frame_time = 1.0s / 60.0;

    // Инициализация всех сущностей, инициализация которых
    // была отложенна.
    m_entities_deferred_initialization.should_store_entities = false;
    for (auto &entity : m_entities_deferred_initialization.entities_for_init) {
        entity->init();
    }

    // Комнонет отвечающий за отрисовку и обработку событий.
    auto component = ftxui::Renderer([this] { return this->render(); });
    component |= ftxui::CatchEvent([this](ftxui::Event event) {
        this->events.m_events.push_back(event);
        return false;
    });

    ftxui::Loop loop{&screen, component};
    unsigned short frame_counter = 0;

    while (!loop.HasQuitted()) {
        static auto last_tick = std::chrono::steady_clock::now();

        // Получение delta time и времени начала отрисовки с обновлением.
        const auto update_time = std::chrono::steady_clock::now();
        const auto dt =
            std::chrono::duration_cast<milliseconds>(update_time - last_tick);

        // Обновление всех сущностей.
        for (auto &entity : m_world.entities) {
            entity->update(static_cast<double>(dt.count()));
        }

        // Обновление времени тика.
        last_tick = update_time;

        // Отрисовка происходит слишком часто, поэтому замедленна в 5 раз.
        if (frame_counter % 5 == 0) {
            screen.RequestAnimationFrame();
            loop.RunOnce();
        } else if (frame_counter % 8 == 0) {
            // Каждые 8 кадров очищаем события.
            // Так редко, так как новые события появляются
            // только каждые 5 кадров.
            events.m_events.clear();
        }

        // Получение времени.
        auto draw_end_time = std::chrono::steady_clock::now();
        auto draw_duration = std::chrono::duration_cast<milliseconds>(
            draw_end_time - update_time);
        ++frame_counter;

        // Ждём, если нужно.
        if (draw_duration.count() < target_frame_time.count()) {
            std::this_thread::sleep_for(target_frame_time - draw_duration);
        }
    }
}

ftxui::Element Application::render() {
    // Получаем размер терминала.
    const auto size = ftxui::Terminal::Size();

    // Создаём экран, на котором будем рисовать.
    auto canvas = ftxui::Canvas(size.dimx * 2, size.dimy * 4);

    // Рисуем на canvas.
    for (auto &entity : m_world.drawable_entities) {
        const auto pixels = entity->render();

        for (const auto &pixel : pixels) {
            canvas.DrawPixel(
                static_cast<int>(entity->position.x) + (pixel.x * 2),
                static_cast<int>(entity->position.y) + (pixel.y * 4), pixel);
        }
    }

    // Рендерим о возвращаем результат.
    auto result =
        ftxui::Renderer([&] { return ftxui::canvas(std::move(canvas)); });
    return result->Render();
}
