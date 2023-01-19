////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
// Cpp
#include <ctype.h>
#include <stdio.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ConsoleWindow : public UIComponent
    {
      public:
        ConsoleWindow(EditorContext::Ptr editorContext);
        virtual ~ConsoleWindow() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void  clearLog();
        void  addLog(const char* fmt, ...) IM_FMTARGS(2);
        void  execCommand(const char* command_line);
        int   textEditCallback(ImGuiInputTextCallbackData* data);
        // String utility
        int   stringIcmp(const char* str1, const char* str2);
        int   stringNicmp(const char* str1, const char* str2, int n);
        char* stringDup(const char* str);
        void  stringTrim(char* str);

      private:
        struct ConsoleData
        {
            char                  InputBuf[256];
            ImVector<char*>       Items;
            ImVector<const char*> Commands;
            ImVector<char*>       History;
            // -1: new line, 0..History.Size-1 browsing history.
            int                   HistoryPos;
            ImGuiTextFilter       Filter;
            bool                  AutoScroll;
            bool                  ScrollToBottom;
        };

      private:
        ConsoleData m_ConsoleData;
    };
} // namespace nero
#endif // CONSOLEWINDOW_H
