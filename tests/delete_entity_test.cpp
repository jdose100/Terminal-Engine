#include "catch2/catch_test_macros.hpp"

#include "term_engine/application.hpp"
#include "term_engine/entity.hpp"

#include <memory>

using namespace tengine;
using namespace std;

struct DeleteTestEntity : public Entity {
    bool should_be_deleted;
    
    DeleteTestEntity(bool t_should_be_deleted)
        : Entity{}, should_be_deleted{t_should_be_deleted} {}

    bool operator==(const DeleteTestEntity &other) const {
        return this->should_be_deleted == other.should_be_deleted;
    }

    void update([[maybe_unused]] double) override {}
    void init() override {}
};

TEST_CASE("Entities are deleted", "[Application::deleteEntity]") {
    // Сущность, которая будет должна удалена.
    auto delete_entity = make_shared<DeleteTestEntity>(true);

    // Получение приложения и добавление сущности.
    auto app = Application::singleton();
    app->addEntity(delete_entity);

    // Добавление сущностей, которые не должны быть удалены.
    for (auto i = 0; i < 256; i++) {
        auto entity = make_shared<DeleteTestEntity>(false);
        app->addEntity(entity);
    }

    // Удаление нужной сущности.
    app->deleteEntity(delete_entity);

    // Проверка что удалённая сущность отсутствует в движке.
    for (const auto &entity : app->getEntities<DeleteTestEntity>()) {
        REQUIRE(entity != delete_entity);
    }
}
