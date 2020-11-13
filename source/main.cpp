#include <iostream>
#include "App.hpp"

int main() {
    App app;
    assert(app.IsCreated());

    try {
        app.Initialize();
        app.Run();
        app.Finalize();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        system("pause");
    }
    return 0;
}
