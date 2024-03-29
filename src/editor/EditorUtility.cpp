////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/editor/EditorUtility.h>
#include <Nero/core/cpp/utility/File.h>
// File Diaglog
#include <nativefiledialog/include/nfd.h>
// Std
#include <unistd.h>
/////////////////////////////////////////////////////////////

namespace nero
{

    std::string AppLauncher::qtCreatorProcessId     = StringPool.BLANK;
    std::string AppLauncher::visaulStudioProcessId  = StringPool.BLANK;
    std::string AppLauncher::profilerProcessId      = StringPool.BLANK;
    std::string AppLauncher::texturePackerProcessId = StringPool.BLANK;
    std::string AppLauncher::NERO_GAME_HOME =
        getenv("NERO_GAME_HOME") ? std::string(getenv("NERO_GAME_HOME")) : StringPool.BLANK;
    std::string AppLauncher::TEXTURE_PACKER =
        getenv("NERO_GAME_TP") ? std::string(getenv("NERO_GAME_TP")) : StringPool.BLANK;

    void TabSelectionHandler::registerTab(const std::vector<std::string>& tabNameTable)
    {
        if(tabNameTable.size() <= 0)
        {
            return;
        }

        for(const std::string& tab : tabNameTable)
        {
            m_TabMap[tab] = false;
        }

        // select the first tab
        m_TabMap[tabNameTable.front()] = true;
    }

    void TabSelectionHandler::selectTab(const std::string& tabName)
    {
        reset();

        m_TabMap[tabName] = true;
    }

    ImGuiTabItemFlags TabSelectionHandler::getTabStatus(const std::string& tabName)
    {
        if(m_TabMap[tabName])
        {
            return ImGuiTabItemFlags_SetSelected;
        }

        return ImGuiTabItemFlags_None;
    }

    void TabSelectionHandler::reset()
    {
        for(auto it = m_TabMap.begin(); it != m_TabMap.end(); it++)
        {
            it->second = false;
        }
    }

    sf::Vector2f formatTexturetSize(sf::Vector2f original, float size)
    {
        if(original.x > size)
        {
            float scale = size / original.x;
            original.x  = size;
            original.y  = original.y * scale;
        }
        if(original.y > size)
        {
            float scale = size / original.y;
            original.y  = size;
            original.x  = original.x * scale;
        }

        return original;
    };

    void pushToolbarStyle(bool selected)
    {
        ImVec4 color = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 7.f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.000f, 1.000f, 1.000f, .950f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.000f, 1.000f, 1.000f, .900f));
        ImGui::PushStyleColor(ImGuiCol_Text, color);

        if(selected)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.f);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.595f, 0.308f, 1.000f));
        }
        else
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
            ImGui::PushStyleColor(ImGuiCol_Border, color);
        }

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    }

    void popToolbarStyle()
    {
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);
        ImGui::PopFont();
    }

    void pushResourceStyle(bool selected)
    {
        if(selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.929f, 0.596f, 0.310f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.929f, 0.596f, 0.310f, .950f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.929f, 0.596f, 0.310f, .900f));
        }
    }

    void popResourceStyle(bool selected)
    {
        if(selected)
        {
            ImGui::PopStyleColor(3);
        }
    }

    void pushGameLevelStyle(bool selected, bool opened)
    {
        ImVec4 color = ImVec4(0.000f, 0.000f, 0.000f, 1.000f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 7.f));

        if(opened)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.929f, 0.595f, 0.308f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.929f, 0.595f, 0.308f, .950f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.929f, 0.595f, 0.308f, .900f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.000f, 1.000f, 1.000f, .950f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.000f, 1.000f, 1.000f, .900f));
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        }

        if(selected)
        {
            if(opened)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.000f, 1.000f, 1.000f, 1.000f));
            }
            else
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.595f, 0.308f, 1.000f));
            }
        }
        else
        {
            if(opened)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 4.f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.929f, 0.595f, 0.308f, 1.000f));
            }
            else
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
                ImGui::PushStyleColor(ImGuiCol_Border, color);
            }
        }

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    }

    void popGameLevelStyle()
    {
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);
        ImGui::PopFont();
    }

    void AppLauncher::launchTexturePacker()
    {
        if(cmd::processRunning(AppLauncher::texturePackerProcessId))
        {
            cmd::showApplication("TexturePacker"); //, AppLauncher::texturePackerProcessId);
        }
        else
        {
            AppLauncher::texturePackerProcessId =
                cmd::launchApplication(AppLauncher::TEXTURE_PACKER);

            usleep(100);

            cmd::showApplication("TexturePacker"); //, AppLauncher::texturePackerProcessId);
        }
    }

    void AppLauncher::launchProfiler()
    {
        if(cmd::processRunning(AppLauncher::profilerProcessId))
        {
            cmd::showApplication("EasyProfiler"); //, AppLauncher::profilerProcessId);
        }
        else
        {
            std::string profiler = AppLauncher::NERO_GAME_HOME + "/Tools/Profiler/profiler_gui.exe";
            AppLauncher::profilerProcessId = cmd::launchApplication(profiler);

            usleep(100);

            cmd::showApplication("EasyProfiler"); //, AppLauncher::profilerProcessId);
        }
    }

    namespace file
    {
        void selectDirectory(std::function<void(std::string)> callback)
        {
            nfdchar_t*  outPath = nullptr;
            nfdresult_t result  = NFD_PickFolder(nullptr, &outPath);

            if(result == NFD_OKAY)
            {
                callback(std::string(outPath));

                free(outPath);
            }
            else if(result == NFD_CANCEL)
            {
                nero_log("select directory canceled");
            }
            else
            {
                nero_log("failed to select directory : " + nero_s(NFD_GetError()));
            }
        }

        void selectFile(std::function<void(std::string)> callback)
        {
            nfdchar_t*  outPath = nullptr;
            nfdresult_t result  = NFD_OpenDialog(nullptr, nullptr, &outPath);

            if(result == NFD_OKAY)
            {
                callback(std::string(outPath));

                free(outPath);
            }
            else if(result == NFD_CANCEL)
            {
                nero_log("select file canceled");
            }
            else
            {
                nero_log("failed to select file : " + nero_s(NFD_GetError()));
            }
        }

        void selectFile(std::function<void(std::vector<std::string>)> callback)
        {
            nfdpathset_t pathSet;
            nfdresult_t  result = NFD_OpenDialogMultiple(nullptr, nullptr, &pathSet);

            if(result == NFD_OKAY)
            {
                std::vector<std::string> fileTable;

                size_t                   i;
                for(i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i)
                {
                    nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);

                    fileTable.push_back(toString(path));
                }

                callback(fileTable);

                NFD_PathSet_Free(&pathSet);
            }
            else if(result == NFD_CANCEL)
            {
                nero_log("select files canceled");
            }
            else
            {
                nero_log("failed to select files : " + nero_s(NFD_GetError()));
            }
        }
    } // namespace file
} // namespace nero
