#pragma once

#include "term_engine/data.hpp"

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>

#include <glm/ext/vector_float2.hpp>

#include <memory>

namespace tengine {

/*!
    @brief Минимальное представление сущности.
    @details
    Класс, являющийся минимальной единицой
    объекта внутри игрового движка.

    Для наследования необходимо обязательно переопределить
    методы **init** и **update**.
*/
class Entity {
    friend class Application;
    friend struct World;

  public:
    //! Позиция сущности в мире.
    glm::vec2 position{0.0f};

    /*!
        @brief Определяет слой отрисовки сущности.
        @details Чем ниже число, тем раньше оно будет отрисованно.
    */
    const int depth = 0;

    //! Возможно ли рисовать эту сущность.
    const bool is_drawable;

    //! Конструктор. Создаёт рисуемую сущность.
    Entity(glm::vec2 t_pos, int t_depth)
        : position{t_pos}, depth{t_depth}, is_drawable{true} {}

    //! Конструктор. Создаёт рисуемую сущность.
    Entity(int t_depth) : depth{t_depth}, is_drawable{true} {}

    //! Конструктор. Создаёт **не** рисуемую сущность.
    Entity(glm::vec2 t_pos, bool t_is_drawable = false)
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
    virtual void init() = 0;

    /*!
        @brief Обновление сущности.
        @param[in] delta_time delta time.
    */
    virtual void update(double delta_time) = 0;

    /*!
        @brief Отрисовка сущности.
        @details
        Рисует сущности только в том случае,
        если она является *drawable*.
    */
    virtual const Image render() { return Image{}; }

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
typedef std::shared_ptr<Entity> EntityPointer;

} // namespace tengine
