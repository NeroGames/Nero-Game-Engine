////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/console/CommandLine.h>
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
		m_CommandEditorStart	= m_CommandEditor->add_subcommand("start", "start the editor");
		m_CommandEditorRestart	= m_CommandEditor->add_subcommand("restart", "restart the editor");
		m_CommandEditorStop		= m_CommandEditor->add_subcommand("stop", "stop the editor");
	}

	void CommandLine::handleCommand()
	{
		startEditor();
	}

	void CommandLine::startEditor()
	{
		auto& commandEditorStart = *m_CommandEditorStart;
		if(commandEditorStart)
		{
			std::cout << "start the engine " << std::endl;
		}
	}

}
