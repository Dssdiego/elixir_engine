#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#include <exception>
#include <iostream>
#include "core/GameEngine.h"

#endif

int main()
{
    CGameEngine engine;
    SEngineConfig config {
        "Elixir Game Engine",
        {800, 600},
        {320, 240},
        true
    };
    try {
        engine.Init(&config);
        engine.Run();
        engine.Cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//#ifdef WIN32
//int APIENTRY WinMain(HINSTANCE hInstance,
//HINSTANCE hPrevInstance,
//        LPSTR lpCmdLine, int nCmdShow)
//{
//return main();
//}
//#endif