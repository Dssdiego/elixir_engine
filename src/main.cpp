#include <windows.h>
#include "VulkanApplication.h"
//#include "OpenGLApplication.h"

int main()
{
    VulkanApplication app{};
//    OpenGLApplication app{};
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