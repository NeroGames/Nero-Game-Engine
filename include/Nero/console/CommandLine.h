////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef COMMANDLINE_H
#define COMMANDLINE_H
///////////////////////////HEADERS//////////////////////////
//Cli11
#include <CLI11/CLI11.hpp>
//Cpp
#include <iostream>
////////////////////////////////////////////////////////////
namespace nero
{
	class CommandLine : public CLI::App
	{
		public:
							CommandLine();
			void			handleCommand();

		private:
			void			startEditor();

		private:
			CLI::App*		m_CommandEditor;
			CLI::App*		m_CommandEditorStart;
			CLI::App*		m_CommandEditorRestart;
			CLI::App*		m_CommandEditorStop;
			CLI::Option*	m_OptionVersion;
	};
}

#endif // COMMANDLINE_H
