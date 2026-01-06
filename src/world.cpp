#include "term_engine/world.hpp"
#include "term_engine/entity.hpp"
#include "term_engine/error.hpp"

using tengine::EntityPointer;
using tengine::World;
using namespace std;

void World::addEntity(EntityPointer entity, size_t hash) noexcept {
    // Отправляем entity в общий список.
    entities.push_back(entity);
    entity->m_main_index = static_cast<unsigned int>(entities.size() - 1);

    // Отправляет entity в список сущностей, которых
    // можно отрисоовать (если его можно рисовать).
    if (entity->is_drawable) {
        drawable_entities.push_back(entity);

        std::stable_sort(drawable_entities.begin(), drawable_entities.end(),
                         [](EntityPointer entity1, EntityPointer entity2) {
                             return entity1->depth < entity2->depth;
                         });
    }

    // Отправляем entity в cловарь по типу.
    // То есть "хэшируем" для более быстрого способа
    // получения сущности известного типа.
    entity->m_hash_indexes.push_back(hash);
    if (hashed_entities.count(hash)) {
        hashed_entities.at(hash).push_back(entity);
    } else {
        hashed_entities.insert({hash, std::vector{entity}});
    }
}

void World::deleteEntity(const EntityPointer entity) noexcept {
    // Удаление из массива рисуемых сущностей.
    if (entity->is_drawable) {
        drawable_entities.erase(
            find(drawable_entities.begin(), drawable_entities.end(), entity));
    }

    // Удаление хэшированных сущностей.
    for (const auto &idx : entity->m_hash_indexes) {
        auto &entities = hashed_entities.at(idx);
        entities.erase(find(entities.begin(), entities.end(), entity));
    }

    // Удаление сущности.
    entities.erase(find(entities.begin(), entities.end(), entity));
}

EntityPointer World::getEntity(size_t hash, const char *name,
                               size_t idx) const {
    if (!hashed_entities.count(hash)) {
        throw EntityNotFound(name);
    }

    return hashed_entities.at(hash).at(idx);
}
