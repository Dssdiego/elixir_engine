//
// Created by Diego S. Seabra on 09/05/22.
//

#include <exception>
#include <iostream>
#include "../engine/api/Game.h"

int main()
{
    Game game;
    EngineConfig config {
            "My Game",
            {800, 600},
            {320, 240},
            true
    };
    try {
        game.Init(&config);
        game.Run();
        game.Cleanup();
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
