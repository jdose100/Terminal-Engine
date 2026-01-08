#pragma once

#include "term_engine/triggers.hpp"
#include "term_engine/entity.hpp"

#include <memory>
#include <vector>

namespace tengine {

/*!
    @brief Мир, хранящий данные об сущностях.
    @details
    Задача мира хранить данные об сущностях, а также
    предоставить функции для удобного манимпулирования
    всеми сущностями.
*/
struct World {
    //! Список сущностей, все они обновляются каждый кадр.
    std::vector<EntityPointer> entities;

    //! Массив сущностей, которых можно отрисовать.
    std::vector<EntityPointer> drawable_entities;

    //! Хэш мапа сущностей, позволяет получить сущностей по хешу типа.
    std::unordered_map<size_t, std::vector<EntityPointer>> hashed_entities;
    
    //! Триггеры мира.
    std::vector<std::shared_ptr<ITrigger>> triggers;
    
    /*!
        @brief Добавляет триггер в мир.
        @param[in] trigger триггер для добавления.
    */
    inline constexpr void addTrigger(std::shared_ptr<ITrigger> &trigger) {
        triggers.push_back(trigger);
    }

    /*!
        @brief Добавление сущности в мир.
        @param[in] entity ссылка на сущность, которую нужно добавить.
        @param[in] hash Хэш сущности.
    */
    void addEntity(EntityPointer entity, size_t hash) noexcept;

    /*!
        @brief Удаление сущности из мира.
        @param[in] entity сущность, которую нужно удалить.
    */
    void deleteEntity(const EntityPointer entity) noexcept;

    /*!
        @brief Получение ссылки на сущность.
        @param[in] hash хэш искомой сущности.
        @param[in] name имя типа искомой сущности.
        @param[in] idx индекс искомой сущности.
        @throw EntityNotFound если не найдена сущность.
        @throw std::out_of_range если индекс не существует.
        @return Ссылка на сущность.
    */
    EntityPointer getEntity(size_t hash, const char *name, size_t idx) const;

    /*!
        @brief Получение массива ссылок на сущностей.
        @details Возвращает массив сущностей с типом T, или дочерние классы T.
        @return Массив ссылок на сущностей.
    */
    template <typename T>
    inline std::vector<std::shared_ptr<T>> getEntities() const {
        // Возвращение массива сущностей, где можно искать сущность с типом T,
        // или сущность которая наследуется от T.
        const auto init_value = [this]() -> const std::vector<EntityPointer> {
            // Получение хеша.
            const auto hash = typeid(T).hash_code();

            // Если тип T хэширован, то возвращаем
            // хэшированный массив.
            if (hashed_entities.count(hash)) {
                return hashed_entities.at(hash);
            }

            // Тип T не хэширован, возвращаем общий массив
            // всех сущностей.
            return entities;
        };
        const auto value = init_value();

        // Инициализация вектора.
        std::vector<std::shared_ptr<T>> return_value;

        // Цикл, который ищет все возможные сущности, которые являются T
        // или наследуются от T. Все найденные сущности сохраняет в
        // return_value.
        for (const auto &item : value) {
            // Получение ссылки.
            std::shared_ptr<T> pointer = std::dynamic_pointer_cast<T>(item);

            // Если не nullptr, то значит что ссылка ссылается
            // либо на объект с типом T, либо на дочерний класс T.
            if (pointer != nullptr) {
                return_value.push_back(pointer);
            }
        }
        return return_value;
        
        // Вариант оптимизации по скорости поиска (с доп. затратами на память).
        // Это создать ещё 1 хэш таблицу и засовывает в неё новые, не
        // хэшированные типы. Или использовать уже существующую таблицу
        // `hashed_entities`.
    }
};

} // namespace tengine
