////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/LoggerWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LoggerWindow::LoggerWindow(EditorContext::Ptr editorContext)
        : UIComponent(editorContext)
    {
        m_LoggerData.AutoScroll     = true;
        m_LoggerData.ScrollToBottom = false;
        clear();
    }

    LoggerWindow::~LoggerWindow()
    {
    }

    void LoggerWindow::destroy()
    {
        clear();
    }

    void LoggerWindow::render()
    {
        std::string logData = nero::logging::Logger::getString();

        if(logData != StringPool.BLANK)
        {
            addLog(logData);
            nero::logging::Logger::clearStringStream();
        }

        if(!ImGui::Begin(EditorConstant.WINDOW_LOGGING.c_str(),
                         nullptr,
                         ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if(ImGui::BeginPopup("Options"))
        {
            if(ImGui::Checkbox("Auto-scroll", &m_LoggerData.AutoScroll))
                if(m_LoggerData.AutoScroll)
                    m_LoggerData.ScrollToBottom = true;
            ImGui::EndPopup();
        }

        // Main window
        if(ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool onClearButton = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        m_LoggerData.Filter.Draw("Filter", -100.0f);

        ImGui::Dummy(ImVec2(0.f, 5.f));

        ImGui::BeginChild("scrolling", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        if(onClearButton)
            clear();
        if(copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf     = m_LoggerData.Buf.begin();
        const char* buf_end = m_LoggerData.Buf.end();
        if(m_LoggerData.Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
            // especially if the filtering function is not trivial (e.g. reg-exp).
            for(int line_no = 0; line_no < m_LoggerData.LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + m_LoggerData.LineOffsets[line_no];
                const char* line_end   = (line_no + 1 < m_LoggerData.LineOffsets.Size) ? (buf + m_LoggerData.LineOffsets[line_no + 1] - 1) : buf_end;
                if(m_LoggerData.Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
            // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
            // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
            // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
            ImGuiListClipper clipper;
            clipper.Begin(m_LoggerData.LineOffsets.Size);
            while(clipper.Step())
            {
                for(int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + m_LoggerData.LineOffsets[line_no];
                    const char* line_end   = (line_no + 1 < m_LoggerData.LineOffsets.Size) ? (buf + m_LoggerData.LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if(m_LoggerData.ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        m_LoggerData.ScrollToBottom = false;
        ImGui::EndChild();
        ImGui::Dummy(ImVec2(0.f, 5.f));
        ImGui::End();
    }

    void LoggerWindow::clear()
    {
        m_LoggerData.Buf.clear();
        m_LoggerData.LineOffsets.clear();
        m_LoggerData.LineOffsets.push_back(0);
    }

    void LoggerWindow::addLog(const std::string& message)
    {
        addLog(message.c_str());
    }

    void LoggerWindow::addLog(const char* fmt, ...)
    {
        int     old_size = m_LoggerData.Buf.size();
        va_list args;
        va_start(args, fmt);
        m_LoggerData.Buf.appendfv(fmt, args);
        va_end(args);
        for(int new_size = m_LoggerData.Buf.size(); old_size < new_size; old_size++)
            if(m_LoggerData.Buf[old_size] == '\n')
                m_LoggerData.LineOffsets.push_back(old_size + 1);
        if(m_LoggerData.AutoScroll)
            m_LoggerData.ScrollToBottom = true;
    }

    ImVec4 LoggerWindow::getLoggingColor(const logging::LEVEL& level)
    {
        switch(level)
        {
        case logging::LOG_INFO: {
            return ImVec4();
        }
        break;

        case logging::LOG_DEBUG: {
            return ImVec4();
        }
        break;

        case logging::LOG_TRACE: {
            return ImVec4();
        }
        break;

        case logging::LOG_WARNING: {
            return ImVec4();
        }
        break;

        case logging::LOG_ERROR: {
            return ImVec4();
        }
        break;

        case logging::LOG_NOTICE: {
            return ImVec4();
        }
        break;

        case logging::LOG_CRITICAL: {
            return ImVec4();
        }
        break;

        case logging::LOG_FATAL: {
            return ImVec4();
        }
        break;

            return ImGui::GetStyle().Colors[ImGuiCol_Text];
        }
    }
} // namespace nero
