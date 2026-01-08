#pragma once

#include "term_engine/entity.hpp"
#include "term_engine/math.hpp"

namespace tengine {

/*!
    @interface ITrigger
    @brief Простейшее придставление триггера.
*/
class ITrigger {
  public:
    //! Маска триггера, определяющая с кем он может реагировать.
    const uint64_t mask;

    //! Триггер нельзя создать без указания маски.
    ITrigger() = delete;
    virtual ~ITrigger() {}

    /*!
        @brief Создание триггера.
        @param[in] t_mask маска триггера.
        @details Создаёт триггер с маской t_mask.
    */
    ITrigger(const uint64_t t_mask) : mask{t_mask} {}

    /*!
        @brief Проверка триггера.
        @return true если триггер сработал.
        @return false если триггер не сработал.
        @details
        В данной функции триггер должен проверить, должен ли он
        сработать или нет, относительно полученной сущности.
    */
    virtual bool check(const EntityPointer) const = 0;
};

//! Триггер, срабатывающий если находит сущность
//! внутри себя. Является триггером прямоугольной формы.
struct PositionTrigger : public ITrigger {
    //! Точка, где начинается триггер.
    math::vec2 pos_start;
    
    //! Точка, где заканчивается триггер.
    math::vec2 pos_end;

    /*!
        @brief Создаёт триггер.
        @param[in] mask маска триггера.
        @param[in] t_pos_start точка, где начинается область действия триггера.
        @param[in] t_pos_end точка, где заканчивается область действия триггера.
    */
    PositionTrigger(const uint64_t mask, math::vec2 t_pos_start,
                    math::vec2 t_pos_end)
        : ITrigger{mask}, pos_start{t_pos_start}, pos_end{t_pos_end} {}

    //! Проверяет, что сущность не находится внутри триггера.
    bool check(const EntityPointer) const override;
};

} // namespace tengine
