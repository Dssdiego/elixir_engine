//
// Created by Diego S. Seabra on 09/05/22.
//

#include <exception>
#include <iostream>
#include "../engine/api/Game.h"
#include "../engine/common/Config.h"

int main()
{
    // REVIEW: Pass engine config directly to the "Game" class?
//    std::cout << Config::GetSingleton() << std::endl;
    Game game;
    EngineConfig config {
            "Card Game",
            {Config::GetWindowWidth(),
             Config::GetWindowHeight()},
            {320, 240},
            Config::GetSaveToLogFile() // FIXME: Logger saves automatically so this config is useless right now!
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
