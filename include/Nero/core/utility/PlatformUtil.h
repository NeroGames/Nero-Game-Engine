////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////

//check OS
#ifdef _WIN32
    #define NERO_OS_WINDOW32
#endif

#ifdef _WIN64
    #define NERO_OS_WINDOW64
#endif

#ifdef __linux__
    #define NERO_OS_LINUX
#endif

#ifdef __ANDROID__
    #define NERO_OS_ANDROID
#endif

#ifdef __APPLE__
    #define NERO_OS_APPLE
#endif

//check compiler
#ifdef _MSC_VER
    #define NERO_COMPILER_VISUAL_STUDIO
#endif

#ifdef __GNUC__
    #define NERO_COMPILER_GCC
#endif

#ifdef __clang__
    #define NERO_COMPILER_CLANG
#endif
