// ВАЖНО! Для подробностей об сущности смотрите `simple_entity.cpp`.

#include "term_engine/data.hpp"
#include <term_engine/application.hpp>
#include <term_engine/entity.hpp>

#include <memory>

// Для создание рисуемой сущности необходимо
// в конструктор Entity передать либо только
// слой, на котором будет рисоваться сущность.
// Как в этом классе. А также обязательно перегрузить
// функцию render(). Так как хоть сущность и рисуется,
// но стандартная функция возвращает пустое изображение,
// поэтому её необходимо перегрузить.
class DrawableEntity1 : public tengine::Entity {
  public:
    // Сущность рисуется на нулевом слое.
    DrawableEntity1() : tengine::Entity{0} {}

    // Это перегрузка функции render. Теперь
    // она возвращает изображение, в котором
    // пиксель зелёного цвета и символ '1'.
    const tengine::Image render() override {
        tengine::Pixel px;
        px.background_color = tengine::Color::Green;
        px.character = "1";

        return tengine::Image{px};
    }

    void update([[maybe_unused]] double) override {}
    void init() override {}
};

class DrawableEntity2 : public tengine::Entity {
  public:
    // Для того, чтобы сущность была рисуемой есть ещё 1 конструктор.
    // Он принимает координаты сущности, а также слой отрисовки.
    DrawableEntity2() : tengine::Entity{glm::vec2{5.f}, 0} {}

    // Это изображение уже красное и с символом '2'.
    const tengine::Image render() override {
        tengine::Pixel px;
        px.background_color = tengine::Color::Red;
        px.character = "2";

        return tengine::Image{px};
    }

    void update([[maybe_unused]] double) override {}
    void init() override {}
};

int main() {
    auto app = tengine::Application::singleton();

    // Первая рисуемая сущность.
    auto entity1 = std::make_shared<DrawableEntity1>();
    app->addEntity(entity1);
    
    // Вторая рисуемая сущность.
    auto entity2 = std::make_shared<DrawableEntity2>();
    app->addEntity(entity2);

    app->run();
}
