#ifndef OS_H
#define OS_H

#include <iostream>
#include <cstring>

//Apple products
#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

class Search_os
{
public:
    int check_running_os()
    {
        #ifdef __linux__
            // Linux
            return 0;
        #elif _WIN32
            // Windows
            return 10;
        #elif __APPLE__
            // Apple products
            #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
                // iPhone
                return 11;
            #elif TARGET_OS_MAC
                // Mac OS
                return 12;
            #else
                // Unknown Apple platform
                return 13;
            #endif
        #elif __ANDROID__
            // Android OS
            return 14;
        #else
            // Not in list
            return 1;
        #endif
    }
};

#endif // OS_H
