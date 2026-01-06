#include <catch2/catch_test_macros.hpp>

#include <term_engine/application.hpp>
#include <term_engine/entity.hpp>

#include <memory>

using namespace tengine;
using namespace std;

class GetEntity : public Entity {
    void update([[maybe_unused]] double) override {}
    void init() override {}
};

class NotGetEntity : public Entity {
    void update([[maybe_unused]] double) override {}
    void init() override {}
};

TEST_CASE("Entities are geted", "Application::getEntity") {
    auto app = Application::singleton();

    auto get_entity = make_shared<GetEntity>();
    app->addEntity(get_entity);

    for (auto i = 0; i < 64; i++) {
        auto entity = make_shared<NotGetEntity>();
        app->addEntity(entity);
    }

    REQUIRE(get_entity == app->getEntity<GetEntity>());
}
