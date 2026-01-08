#include <term_engine/triggers.hpp>
#include <term_engine/entity.hpp>
#include <term_engine/math.hpp>

#include <catch2/catch_test_macros.hpp>

#include <memory>

using namespace tengine;
using namespace std;
using math::vec2;

TEST_CASE("Check PositionTrigger", "[PositionTrigger]") {
    SECTION("Test 'check' function to find entity") {
        auto trigger = PositionTrigger(0, vec2{0.f}, vec2{10.f});
        auto entity = make_shared<Entity>(vec2{4.f});
        
        REQUIRE(trigger.check(entity) == true);
    }
    
    SECTION("Test 'check' function to not find entity") {
        auto trigger = PositionTrigger(0, vec2{40.f}, vec2{40.f});
        auto entity = make_shared<Entity>(vec2{0.f});
        
        REQUIRE(trigger.check(entity) == false);
    }
}
