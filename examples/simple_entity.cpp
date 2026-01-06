#include <term_engine/application.hpp>
#include <term_engine/entity.hpp>

#include <memory>

// Создание простой сущности. ВАЖНО! Данная сущность
// не будет рисоваться, для создание сущности которая
// будет рисоваться смотрите `drawable_entity.cpp`.
class SimpleEntity : public tengine::Entity {
  public:
    // Обновление сущности в каждом кадре, в качестве аргумента
    // предоставляется delta time. Здесь есть [[maybe_unused]],
    // так как он не используется внутри функции.
    void update([[maybe_unused]] double delta_time) override {}

    // Данный метод гарантирует наличие инициализированного Application,
    // в отличии от конструктора. Поэтому здесь можно произвести
    // дополнительную инициализацию, которая зависит от ваших сущностей.
    //
    // Например у игры есть AIManager, который наследуется от tengine::Entity.
    // И если этот мэнеджер добавляется до вызова app->run().
    void init() override {}
};

int main() {
    auto app = tengine::Application::singleton();

    // Добавление сущности в приложение происходит
    // через addEntity, которая принимает shared_ptr.
    auto entity = std::make_shared<SimpleEntity>();
    app->addEntity(entity);

    app->run();
}
