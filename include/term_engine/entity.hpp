#pragma once

#include "term_engine/data.hpp"
#include "term_engine/math.hpp"

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>

#include <memory>

namespace tengine {

class ITrigger;

/*!
    @brief Минимальное представление сущности.
    @details
    Класс, являющийся минимальной единицой
    объекта внутри игрового движка.
*/
class Entity {
    friend class Application;
    friend struct World;

  public:
    //! Позиция сущности в мире.
    math::vec2 position{0.0f};

    //! Маска, определяющая с какими
    //! триггерами может взаимодействовать
    //! сущность.
    uint64_t trigger_mask = 0;

    /*!
        @brief Определяет слой отрисовки сущности.
        @details Чем ниже число, тем раньше оно будет отрисованно.
    */
    const int draw_depth = 0;

    //! Возможно ли рисовать эту сущность.
    const bool is_drawable;

    //! Конструктор. Создаёт рисуемую сущность.
    Entity(math::vec2 t_pos, int t_depth)
        : position{t_pos}, draw_depth{t_depth}, is_drawable{true} {}

    //! Конструктор. Создаёт рисуемую сущность.
    Entity(int t_depth) : draw_depth{t_depth}, is_drawable{true} {}

    //! Конструктор. Создаёт **не** рисуемую сущность по умолчанию.
    //! Может создать рисуемую сущность если передать *true*.
    Entity(math::vec2 t_pos, bool t_is_drawable = false)
        : position{t_pos}, is_drawable{t_is_drawable} {}

    //! Конструктор по умолчанию. Создаём по умолчанию **не** рисуемую
    //! сущность, но создаст рисуемую если передать *true*.
    Entity(bool t_is_drawable = false) : is_drawable{t_is_drawable} {}

    //! Виртуальный деструктор. Можно переопределить.
    virtual ~Entity() {}

    /*!
        @brief Инициализация сущности.
        @details
        Вызывается для полной инициализации сущности.
        В отличии от конструктора гарантируется наличие
        инициализированного Application.
    */
    virtual void init() {};

    /*!
        @brief Обновление сущности.
        @param[in] delta_time delta time.
    */
    virtual void update([[maybe_unused]] double delta_time) {};

    /*!
        @brief Отрисовка сущности.
        @details
        Рисует сущности только в том случае,
        если она является *drawable*.
    */
    virtual const Image render() { return Image{}; }

    /*!
        @brief Реакция сущности на триггер.
        @param[in] trigger триггер, который сработал.
        @details
        Данная функция вызывается, когда trigger сработал
        уменно на эту сущность (this).
    */
    virtual void
    onTrigger([[maybe_unused]] std::shared_ptr<ITrigger> &trigger) {}

  private:
    //! Проверяет что this и ptr ссылаются на 1 и тот же участок памяти.
    bool operator==(const std::shared_ptr<Entity> &ptr) const {
        return this == &*ptr;
    }

    //! Индекс в основном массиве сущностей.
    unsigned int m_main_index = 0;

    //! Массив индексов в хэш таблице, которые касаются
    //! этой ссылки.
    std::vector<size_t> m_hash_indexes;
};

//! Умная ссылка на Entity.
using EntityPointer = std::shared_ptr<Entity>;

} // namespace tengine
