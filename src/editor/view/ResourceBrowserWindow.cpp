////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/ResourceBrowserWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceBrowserWindow::ResourceBrowserWindow(EditorContext::Ptr editorContext)
        :UIComponent(std::move(editorContext))
        ,m_ResourceBrowserSpriteView(m_EditorContext)
        ,m_ResourceBrowserAnimationView(m_EditorContext)
        ,m_ResourceBrowserMeshView(m_EditorContext)
        ,m_ResourceBrowserFontView(m_EditorContext)
        ,m_ResourceBrowserLightmapView(m_EditorContext)
    {

    }

    ResourceBrowserWindow::~ResourceBrowserWindow()
    {
        destroy();
    }

    void ResourceBrowserWindow::destroy()
    {

    }

    void ResourceBrowserWindow::render()
    {
        const auto selectedResourceType = m_EditorContext->getSelectedResourceType();
        auto resourceManager            = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager || selectedResourceType == ResourceType::None)
            return;

        ImGui::Begin("Resource Browser", nullptr, ImGuiWindowFlags());
        if(m_EditorContext->getGameProject() && loadableResource(selectedResourceType))
        {
            if(ImGui::Button("Import File##import_resource", ImVec2(100.f, 0.f)))
            {
                file::selectFile([this, selectedResourceType, resourceManager](std::vector<std::string> fileTable)
                {
                    const auto loadedFileTable = resourceManager->loadFile(selectedResourceType, fileTable);

                    if(!loadedFileTable.empty())
                    {
                        saveResourceFile(selectedResourceType, loadedFileTable);
                    }
                });
            }
        }

        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 100.f);

        if(ImGui::Button("Close##close_sprite_resource", ImVec2(100.f, 0.f)))
        {
            m_EditorContext->setSelectedResourceType(ResourceType::None);
        }

        ImGui::Separator();

        ImGui::BeginChild("browser");

            switch (selectedResourceType)
            {
                case ResourceType::Texture:
                {
                    m_ResourceBrowserSpriteView.render();
                }break;

                case ResourceType::Animation:
                {
                    m_ResourceBrowserAnimationView.render();
                }break;

                case ResourceType::Mesh:
                {
                    m_ResourceBrowserMeshView.render();
                }break;

                case ResourceType::Font:
                {
                    m_ResourceBrowserFontView.render();
                }break;

                case ResourceType::Lightmap:
                {
                    m_ResourceBrowserLightmapView.render();
                }break;

                case ResourceType::None:
                case ResourceType::Sound:
                case ResourceType::Music:
                case ResourceType::Shape:
                case ResourceType::Particle:
                case ResourceType::Composite:
                    break;
            }

        ImGui::EndChild();
        ImGui::End();
    }

    void ResourceBrowserWindow::saveResourceFile(const ResourceType& resourceType, const std::vector<std::string> loadedFileTable)
    {
        //TODO manager screen builder and other
        const auto resourceFolder = m_EditorContext->getLevelBuilder()->getResourceDirectory();

        switch (resourceType)
        {
            case ResourceType::Texture:
            {
                //copy texture file and json helper
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "texture", file::getFileName(file, true)}));

                    std::string jsonHelper  = file::replaceExtension(file, "json");
                    if(file::fileExist(jsonHelper))
                    {
                        file::saveFile(file::getPath({resourceFolder, "texture", file::getFileName(jsonHelper, true)}),
                                       file::loadJson(jsonHelper, true).dump(3));
                    }
                }

            }break;

            case ResourceType::Animation:
            {
                //copy texture file and json helper
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "animation", file::getFileName(file, true)}));

                    std::string jsonHelper  = file::replaceExtension(file, "json");
                    if(file::fileExist(jsonHelper))
                    {

                        file::saveFile(file::getPath({resourceFolder, "animation", file::getFileName(jsonHelper, true)}),
                                       file::loadJson(jsonHelper, true).dump(3));
                    }
                }

            }break;

            case ResourceType::Font:
            {
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "font", file::getFileName(file, true)}));
                }

            }break;

            case ResourceType::Sound:
            {
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "sound", file::getFileName(file, true)}));
                }

            }break;

            case ResourceType::Music:
            {
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "music", file::getFileName(file, true)}));
                }

            }break;

            case ResourceType::Lightmap:
            {
                for(std::string file : loadedFileTable)
                {
                    file::copyFile(file, file::getPath({resourceFolder, "lightmap", file::getFileName(file, true)}));
                }

            }break;
            case ResourceType::None:
            case ResourceType::Mesh:
            case ResourceType::Shape:
            case ResourceType::Particle:
            case ResourceType::Composite:
                break;
        }
    }

    bool ResourceBrowserWindow::loadableResource(const ResourceType& resourceType)
    {
        return  resourceType == ResourceType::Texture   ||
                resourceType == ResourceType::Animation ||
                resourceType == ResourceType::Lightmap  ||
                resourceType == ResourceType::Font      ||
                resourceType == ResourceType::Sound     ||
                resourceType == ResourceType::Music     ||
                resourceType == ResourceType::Particle;
    }
}
