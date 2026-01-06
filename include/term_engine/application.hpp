#pragma once

// WARN. Как удалять сущности из приложения???

#include "term_engine/entity.hpp"
#include "term_engine/events.hpp"
#include "term_engine/world.hpp"

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
        @throw AnyException любое исключение, вызваное Entity::init.
    */
    template <typename T = Entity>
    inline constexpr void addEntity(std::shared_ptr<T> &entity) {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");
        m_world.addEntity(entity, typeid(T).hash_code());

        if (m_entities_deferred_initialization.should_store_entities) {
            m_entities_deferred_initialization.entities_for_init.push_back(
                entity);
        } else {
            entity->init();
        }
    }

    /*!
        @brief Удаляет сущность.
        @param[in] entity Сущность для удаления.
    */
    template <typename T = Entity>
    inline constexpr void deleteEntity(std::shared_ptr<T> &entity) noexcept {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");
        m_world.deleteEntity(entity);
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
    inline constexpr std::shared_ptr<T> getEntity(size_t idx = 0) const {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");

        const auto &type = typeid(T);
        return std::dynamic_pointer_cast<T>(
            m_world.getEntity(type.hash_code(), type.name(), idx));
    }

    /*!
        @return Массив сущностей.
        @details Возвращает массив сущностей с типом T, или дочерние классы T.
        @note В худшем случает скорость выполнения является O(n), где n - это
        кол-во зарегистрированных сущностей в приложении.
    */
    template <typename T>
    inline constexpr std::vector<std::shared_ptr<T>> getEntities() const {
        static_assert(std::is_base_of<Entity, T>::value,
                      "T must be derived from Entity");
        return m_world.getEntities<T>();
    }

  private:
    //! Структура, хранящая флаг указывающий что инициализация
    //! сущностей должна быть отложена. А также хранащая массив
    //! ссылок на сущностей, инициализация которых была отложена.
    //!
    //! Инициализация откладывается до тех пор, пока
    //! не будет вызванна функция Application::run.
    struct EntitiesDeferredInitialization {
        //! Вектор, хранящий ссылки на сущностей,
        //! инициализация которых была отложена.
        std::vector<EntityPointer> entities_for_init;

        //! Флаг, указывающий инициализировать
        //! сущности при добавлении. Или сохранить
        //! ссылки на сущности для того, чтобы отложить
        //! их инициализацию до вызова метода
        //! Application::run.
        bool should_store_entities = true;
    };

    EntitiesDeferredInitialization m_entities_deferred_initialization;

    //! Мир для хранения сущностей.
    World m_world;

    //! Отрисовка.
    ftxui::Element render();

    Application() {}
};

} // namespace tengine
