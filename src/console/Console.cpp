////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/console/Console.h>
#include <windows.h>
////////////////////////////////////////////////////////////
namespace nero
{
	CommandLine::CommandLine():
		CLI::App("Nero game engine command line interface\nLearn more at https://nero-game.com/v2/command-line-interface", "nero")
	{
		//option version
		m_OptionVersion = add_option("-v,--version", "Engine Version");

		//editor command
		m_CommandEditor = add_subcommand("editor", "Interact with the editor");
		m_CommandEditor->require_subcommand(1, 1);
		//start
		m_CommandEditorStart	= m_CommandEditor->add_subcommand("start", "start the editor");
		//restart
		m_CommandEditorRestart	= m_CommandEditor->add_subcommand("restart", "restart the editor");
		auto option1 = m_CommandEditorRestart->add_option("--pid", m_ProcessId, "Process to kill and restart");
		option1->required();
		//stop
		m_CommandEditorStop		= m_CommandEditor->add_subcommand("stop", "stop the editor");
	}

	void CommandLine::handleCommand()
	{
		restartEditor();
	}

	void CommandLine::restartEditor()
	{
		auto& commandEditorRestart = *m_CommandEditorRestart;
		if(commandEditorRestart)
		{
			std::string cmd = "taskkill /F /PID " + std::to_string(m_ProcessId);
			system(cmd.c_str());
			std::string enginePath = std::string(getenv("NERO_GAME_HOME")) + "\\Editor\\\"Nero Game Engine.exe\"";
			system(enginePath.c_str());
		}
	}

}
