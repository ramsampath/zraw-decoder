#pragma once
#include <varargs.h>

class IConsoleOutput
{
public:
    virtual void printf(char* format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args); // Use vprintf to handle the variable argument list
        va_end(args);
    }
};