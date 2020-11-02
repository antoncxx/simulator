#include <iostream>

#include "App.hpp"

int main() {
    App app;
    assert(app.IsCreated());
    app.Initialize();
    app.Run();
    app.Finalize();
    return 0;
}
