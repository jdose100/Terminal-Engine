#include <catch2/catch_test_macros.hpp>

#include <term_engine/application.hpp>
#include <term_engine/entity.hpp>

#include <memory>

using namespace tengine;
using namespace std;

struct AddTestEntity : public Entity {
    void update([[maybe_unused]] double) override {}
    void init() override {}
};

TEST_CASE("Entities are spawned", "[Application::addEntity]") {
    constexpr auto entity_count = 256;
    
    // Получаем приложение и добавляем сущности.
    auto app = Application::singleton();
    for (auto i = 0; i < entity_count; i++) {
        auto entity = make_shared<AddTestEntity>();
        app->addEntity(entity);
    }
    
    // Проверяем кол-во сущностей.
    REQUIRE(app->getEntities<AddTestEntity>().size() == entity_count);
}
