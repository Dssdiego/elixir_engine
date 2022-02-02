#include <windows.h>
#include "TriangleApplication.h"

int main()
{
    TriangleApplication app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int APIENTRY WinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
return main();
}