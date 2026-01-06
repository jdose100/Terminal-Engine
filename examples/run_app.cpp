#include <term_engine/application.hpp>

int main(void) {
    auto app = tengine::Application::singleton();
    app->run();
}
