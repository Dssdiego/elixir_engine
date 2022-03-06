# Coding Naming Conventions

## General

- All code and comments should use U.S. English spelling and grammar

## Classes

- ```"Default"``` Classes are prefixed by "C"
- Template classes are prefixed by "T"
- Classes that are abstract interfaces are prefixed by "I"
- Enums are prefixed by "E"

## Methods/Functions

- All functions that return a bool should ask a true/false question, such as ```IsVisible()``` or ```ShouldClearBuffer()```.
- Functions/methods parameters should be prefixed by its type

```c++
    (String) sFileName
    (Vector) vPosition
    (Enum) eMode
    (int) nChannelId
    (float) fValue
    (bool) bImmediate
    (others) pSomething
```

## Variables

- Private (aka member) variables are prefixed with "m"
- Boolean variables must be prefixed by "b" (for example, ```bPendingDestruction```, or ```bHasFadedIn```).