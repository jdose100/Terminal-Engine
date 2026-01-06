//! @extends term_engine/application.hpp

#include "term_engine/application.hpp"
#include "term_engine/entity.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <algorithm>
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
    typedef std::chrono::duration<double, milli> milliseconds;
    static constexpr milliseconds target_frame_time = 1.0s / 60.0;

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
        auto update_time = std::chrono::steady_clock::now();
        auto dt =
            std::chrono::duration_cast<milliseconds>(update_time - last_tick);

        // Обновление всех сущностей.
        for (auto &entity : m_entities) {
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
    auto size = ftxui::Terminal::Size();

    // Создаём экран, на котором будем рисовать.
    auto canvas = ftxui::Canvas(size.dimx * 2, size.dimy * 4);

    // Рисуем на canvas.
    for (auto &entity : m_drawable_entities) {
        const auto pixels = entity->render();

        for (const auto &pixel : pixels) {
            canvas.DrawPixel(static_cast<int>(entity->position.x) + pixel.x * 2,
                             static_cast<int>(entity->position.y) + pixel.y * 4,
                             pixel);
        }
    }

    // Рендерим о возвращаем результат.
    auto result =
        ftxui::Renderer([&] { return ftxui::canvas(std::move(canvas)); });
    return result->Render();
}

void Application::_addEntity(tengine::EntityPointer entity,
                             size_t hash) noexcept {
    // Отправляем entity в общий список.
    m_entities.push_back(entity);
    entity->m_main_index = static_cast<unsigned int>(m_entities.size() - 1);

    // Отправляет entity в список сущностей, которых
    // можно отрисоовать (если его можно рисовать).
    if (entity->is_drawable) {
        m_drawable_entities.push_back(entity);

        std::stable_sort(m_drawable_entities.begin(), m_drawable_entities.end(),
                         [](EntityPointer entity1, EntityPointer entity2) {
                             return entity1->depth < entity2->depth;
                         });
    }

    // Отправляем entity в cловарь по типу.
    // То есть "хэшируем" для более быстрого способа
    // получения сущности известного типа.
    entity->m_hash_indexes.push_back(hash);
    if (m_hashed_entities.count(hash)) {
        m_hashed_entities.at(hash).push_back(entity);
    } else {
        m_hashed_entities.insert({hash, std::vector{entity}});
    }

    entity->init();
}

void Application::_deleteEntity(const EntityPointer entity) noexcept {
    // Удаление из массива рисуемых сущностей.
    if (entity->is_drawable) {
        m_drawable_entities.erase(find(m_drawable_entities.begin(),
                                       m_drawable_entities.end(), entity));
    }

    // Удаление хэшированных сущностей.
    for (const auto &idx : entity->m_hash_indexes) {
        auto &entities = m_hashed_entities.at(idx);
        entities.erase(find(entities.begin(), entities.end(), entity));
    }

    // Удаление сущности.
    m_entities.erase(find(m_entities.begin(), m_entities.end(), entity));
}
