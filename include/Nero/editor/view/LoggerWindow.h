////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LOGGERWINDOW_H
#define LOGGERWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/core/cpp/utility/Logging.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class LoggerWindow : public UIComponent {
      public:
        LoggerWindow(EditorContext::Ptr editorContext);
        virtual ~LoggerWindow() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void   clear();
        void   addLog(const std::string& message);
        void   addLog(const char* fmt, ...) IM_FMTARGS(2);
        ImVec4 getLoggingColor(const logging::LEVEL& level);

      private:
        struct LoggerData
        {
            ImGuiTextBuffer Buf;
            ImGuiTextFilter Filter;
            // Index to lines offset. We maintain this with addLog() calls,
            // Allowing us to have a random access on lines
            ImVector<int>   LineOffsets;
            bool            AutoScroll;
            bool            ScrollToBottom;
        };

      private:
        LoggerData m_LoggerData;
    };
} // namespace nero
#endif // LOGGERWINDOW_H
