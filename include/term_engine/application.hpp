#pragma once

// WARN. Как удалять сущности из приложения???

#include "term_engine/entity.hpp"
#include "term_engine/error.hpp"
#include "term_engine/events.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace tengine {

/*!
    @brief Приложение.
    @warning Не является патока-безопасным.
    @important В 1 момент может быть активно только 1 приложение.
    @details
    Является *Singleton*'ом. Отвечает за
    логику игрового движка.
*/
class Application final {
  public:
    // glm::vec2 camera_postion;

    //! События приложения.
    EventReader events;

    /*!
        @brief Функция для получения активного приложения.
        @return Ссылка на приложение.
        @details Если активного приложения не существует, то создаёт его
        автоматически.
    */
    static Application *singleton() noexcept;

    /*!
        @brief Запуск приложения.
        @throw AnyException Может бросать любые исключения, возникшие во время
        работы приложения.
    */
    void run();

    /*!
        @brief Добавление новой сущности в приложение.
        @param[in] entity Сущность для добавления.
    */
    template <typename T = Entity>
    inline constexpr void addEntity(std::shared_ptr<T> &entity) noexcept {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");
        _addEntity(static_cast<EntityPointer>(entity), typeid(T).hash_code());
    }

    /*!
        @brief Удаляет сущность.
        @param[in] entity Сущность для удаления.
    */
    template <typename T = Entity>
    inline constexpr void
    deleteEntity(std::shared_ptr<T> &entity) noexcept {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");
        _deleteEntity(static_cast<EntityPointer>(entity));
    }

    /*!
        @brief Получение сущности с типом T.
        @details Тип T обязательно должен наследоваться от Entity.
        @param[in] idx индекс сущности в списке.
        @throw EntityNotFound если не найдена сущность T.
        @throw std::out_of_range если idx слишком большой.
        @return Ссылка на сущность.
    */
    template <typename T>
    inline std::shared_ptr<T> getEntity(size_t idx = 0) const {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");

        const auto hash = typeid(T).hash_code();
        if (!m_hashed_entities.count(hash)) {
            throw EntityNotFound<T>{};
        }

        auto value = m_hashed_entities.at(hash).at(idx);
        return std::dynamic_pointer_cast<T>(value);
    }

    /*!
        @return Массив сущностей.
        @details Возвращает массив сущностей с типом T, или дочерние классы T.
        @note В худшем случает скорость выполнения является O(n), где n - это
        кол-во зарегистрированных сущностей в приложении.
    */
    template <typename T>
    inline std::vector<std::shared_ptr<T>> getEntities() const {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");

        // Возвращение массива сущностей, где можно искать сущность с типом T,
        // или сущность которая наследуется от T.
        const auto init_value = [this]() -> const std::vector<EntityPointer> {
            // Получение хеша.
            const auto hash = typeid(T).hash_code();

            // Если тип T хэширован, то возвращаем
            // хэшированный массив.
            if (m_hashed_entities.count(hash)) {
                return m_hashed_entities.at(hash);
            }

            // Тип T не хэширован, возвращаем общий массив
            // всех сущностей.
            return m_entities;
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
        // `m_hashed_entities`.
    }

  private:
    //! Список сущностей, все они обновляются каждый кадр.
    std::vector<EntityPointer> m_entities;

    //! Массив сущностей, которых можно отрисовать.
    std::vector<EntityPointer> m_drawable_entities;

    //! Хэш мапа сущностей, позволяет получить сущностей по хешу типа.
    std::unordered_map<size_t, std::vector<EntityPointer>> m_hashed_entities;

    //! Отрисовка.
    ftxui::Element render();

    void _addEntity(EntityPointer entity, size_t hash) noexcept;

    void _deleteEntity(const EntityPointer entity) noexcept;

    Application() {}
};

} // namespace tengine
