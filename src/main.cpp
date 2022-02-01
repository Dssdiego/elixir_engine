#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
return main();
}
#endif