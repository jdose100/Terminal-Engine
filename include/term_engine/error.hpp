#pragma once

#include <exception>
#include <string>

#include "term_engine/entity.hpp"

namespace tengine {

//! Исключение показывающие, что сущность не была найдена.
template <typename T = Entity> class EntityNotFound : public std::exception {
  public:
    EntityNotFound() {
        message = "Entity with type `" + std::string(typeid(T).name()) +
                  "` not found.";
    }

    //! Создаёт исключение, в сообщение которого используется
    //! type_name.
    EntityNotFound(const char *type_name) {
        message = "Entity with type `" + std::string(type_name) + "` not found";
    }

    //! Сообщение о том, что произошло.
    const char *what() const noexcept override { return message.c_str(); }

  private:
    std::string message;
};

} // namespace tengine
