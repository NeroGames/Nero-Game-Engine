////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/renderer/RenderEngine.h>
// Profiler
#include <easy/profiler.h>
////////////////////////////////////////////////////////////
int main()
{
    // Start the profiler
    EASY_PROFILER_ENABLE;
    EASY_MAIN_THREAD;
    profiler::startListen();

    // Instantiate the Game
    // nero::RenderEngine game;

    // Launch the editor
    // game.run();

    return 0;
}
