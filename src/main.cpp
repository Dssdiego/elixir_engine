#ifdef WIN32
#include <windows.h>
#endif
//#include "DirectXApplication.h"
//#include "VulkanApplication.h"
//#include "OpenGLApplication.h"
#include "LuaApplication.h"

int main()
{
    LuaApplication app{};
//    VulkanApplication app{};
//    OpenGLApplication app{};
//    DirectXApplication app{};
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
return main();
}
#endif