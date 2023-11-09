////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2024 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/GameEditor.h>
// Profiler
#include <easy/profiler.h>
////////////////////////////////////////////////////////////
int main()
{
    // Start profiler
    EASY_PROFILER_ENABLE
    EASY_MAIN_THREAD
    profiler::startListen();

    // Create game editor
    nero::GameEditor gameEditor;

    // Run editor
    gameEditor.run();

    // Stop profiler
    profiler::stopCapture();
    profiler::stopListen();

    return 0;
}
