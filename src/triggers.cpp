#include "term_engine/triggers.hpp"
#include "glm/vector_relational.hpp"
#include "term_engine/entity.hpp"

using namespace tengine;

bool PositionTrigger::check(const EntityPointer entity) const {
    return math::all(math::lessThanEqual(pos_start, entity->position) &&
                     math::lessThan(entity->position, pos_end));
}
