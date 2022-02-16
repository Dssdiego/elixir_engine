//
// Created by Diego S. Seabra on 15/02/22.
//

#ifndef VULKAN_ENGINE_LUAAPPLICATION_H
#define VULKAN_ENGINE_LUAAPPLICATION_H

#include <iostream>
#include <fstream>
#include <lua/lua.hpp>

class LuaApplication
{
public:
    void run()
    {
        initLua();
        readFile("someFunction.lua");
        callingFunction();
//        pushToStack();
//        getNumberFromScript();
        cleanup();
    }

private:
    lua_State* L = nullptr;
    std::string luaScript;

    void initLua()
    {
        L = luaL_newstate();
    }

    void readFile(std::string fileName)
    {
        std::fstream luaFile("scripts/" + fileName);
        std::string luaSource((std::istreambuf_iterator<char>(luaFile) ),
                               (std::istreambuf_iterator<char>()));
        luaScript = luaSource;
//        std::cout << luaScript << std::endl;
    }

    void callingFunction()
    {
        // read script
        luaL_dostring(L, luaScript.c_str());
        lua_getglobal(L, "Return4");
        if (lua_isfunction(L, -1))
        {
            // push the return of the function to the stack
            lua_pcall(L, 0, 1, 0);
        }
        // get the last value from the stack
        std::cout << lua_tonumber(L, -1) << std::endl;
    }

    // push info to the lua stack
    void pushToStack()
    {
        lua_pushnumber(L, 42);  // stack = 1
        lua_pushnumber(L, 125); // stack = 2
        lua_pushnumber(L, 99);  // stack = 3

        std::cout << lua_tonumber(L, 3) << std::endl;
        std::cout << lua_tonumber(L, 2) << std::endl;
        std::cout << lua_tonumber(L, 1) << std::endl;

        std::cout << lua_tonumber(L, -1) << std::endl; // just like python, negative indexes

        lua_remove(L, 1);
        lua_remove(L, 2);
        lua_remove(L, 3);

        // if we remove stuff from the stack, it will be clean! :)
        std::cout << lua_tonumber(L, 3) << std::endl;
        std::cout << lua_tonumber(L, 2) << std::endl;
        std::cout << lua_tonumber(L, 1) << std::endl;
    }

    // calling a number from the global stack (script variable 'x = 42')
    void getNumberFromScript()
    {
        std::string script = "x = 42";
        luaL_dostring(L, script.c_str());
        lua_getglobal(L, "x");
        lua_Number x = lua_tonumber(L, 1);
        std::cout << x << std::endl;
    }

    void cleanup()
    {
        lua_close(L);
    }
};

#endif //VULKAN_ENGINE_LUAAPPLICATION_H
