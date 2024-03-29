////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef CONSOLE_H
#define CONSOLE_H
///////////////////////////HEADERS//////////////////////////
// Cli11
#include <CLI11/CLI11.hpp>
// Cpp
#include <iostream>
////////////////////////////////////////////////////////////
namespace nero
{
    class CommandLine : public CLI::App
    {
      public:
        CommandLine();
        void handleCommand();

      private:
        void restartEditor();

      private:
        CLI::App*    m_CommandEditor;
        CLI::App*    m_CommandEditorStart;
        CLI::App*    m_CommandEditorRestart;
        CLI::App*    m_CommandEditorStop;
        CLI::Option* m_OptionVersion;
        unsigned int m_ProcessId;
    };
} // namespace nero

#endif // CONSOLE_H
