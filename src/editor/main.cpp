////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/GameEditor.h>
//Profiler
#include <easy/profiler.h>
////////////////////////////////////////////////////////////
int main()
{
	// Start Profiler
	EASY_PROFILER_ENABLE
	EASY_MAIN_THREAD
	profiler::startListen();

	// Instantiate Editor
	nero::GameEditor editor;

	// Launch Editor
	editor.run();

	// Stop Profiler
	profiler::stopListen();

	return 0;
}
