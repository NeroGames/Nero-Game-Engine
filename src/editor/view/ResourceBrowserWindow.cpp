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
        :UIComponent(editorContext)
        ,m_ResourceBrowserSpriteView(editorContext)
        ,m_ResourceBrowserMeshView(editorContext)
        ,m_ResourceBrowserLightmapView(editorContext)
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
                    //showAnimationResource();
                }break;

                case ResourceType::Mesh:
                {
                    m_ResourceBrowserMeshView.render();
                }break;

                case ResourceType::Font:
                {
                    //showFontResource();
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
                        file::copyFile(file, file::getPath({resourceFolder, "texture", file::getFileName(jsonHelper, true)}));
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
                        file::copyFile(file, file::getPath({resourceFolder, "animation", file::getFileName(jsonHelper, true)}));
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


    /*void ResourceBrowserWindow::showAnimationResource()
    {
        auto& animationTable = m_ResourceManager->getAnimationHolder()->getAnimationTable();
        int animationCount = animationTable.size();

        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (int n = 0; n < animationCount; n++)
        {
            sf::Texture& texture = m_ResourceManager->getAnimationHolder()->getTexture(animationTable[n]);
            sf::IntRect bound = m_ResourceManager->getAnimationHolder()->getAnimationBound(animationTable[n]);

            sf::Vector2u boo = sf::Vector2u(bound.width, bound.height);
            sf::Vector2u zoo = boo;
            if(n < animationCount-1)
            {
                sf::IntRect bound2 = m_ResourceManager->getAnimationHolder()->getAnimationBound(animationTable[n+1]);

                zoo = sf::Vector2u(bound2.width, bound2.height);
            }

            sf::Vector2f sprite_size(boo.x, boo.y);
            sprite_size = formatSize(sprite_size, 250);

            sf::Vector2f next_sprite_size(zoo.x, zoo.y);
            next_sprite_size = formatSize(next_sprite_size, 250);


            ImVec2 button_size(sprite_size.x, sprite_size.y);

            sf::Sprite sprite(texture, bound);
            sprite.scale(2.f, 2.f);

            if(ImGui::ImageButton(sprite, button_size))
            {
                //if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
                    //m_AdvancedScene->addObject(Object::Animation_Object, animationTable[n], getAddObjectPosition(), m_EditorMode);
            }

            if(ImGui::IsItemHovered())
            {
                //nero_log("hover animation button");
            }


            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
            if (n + 1 < animationCount && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
        }
    }

    void ResourceBrowserWindow::showFontResource()
    {
        auto& fontTable = m_ResourceManager->getFontHolder()->getFontTable();
        int count = fontTable.size();

        for (int i = 0; i < count; i++)
        {
            if(ImGui::ImageButton(getFontTexture(fontTable.at(i))))
            {
                if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
                {
                    //m_AdvancedScene->addObject(Object::Animation_Object, animationTable[n], getAddObjectPosition(), m_EditorMode);
                }
            }
        }
    }*/

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
