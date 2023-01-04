////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/ResourceBrowserWindow.h>
#include <Nero/editor/EditorConstant.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    ResourceBrowserWindow::ResourceBrowserWindow(EditorContext::Ptr editorContext):
         UIComponent(editorContext)
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
        auto resourceManager = m_EditorContext->getCurrentResourceManager();

        if(!resourceManager || selectedResourceType == ResourceType::None)
            return;

        ImGui::Begin("Resource Browser", nullptr, ImGuiWindowFlags());

            if(m_EditorContext->getGameProject() &&
               (selectedResourceType == ResourceType::Texture	|| selectedResourceType == ResourceType::Animation ||
               selectedResourceType == ResourceType::Sound		|| selectedResourceType == ResourceType::Music		||
               selectedResourceType == ResourceType::Font		|| selectedResourceType == ResourceType::Particle	||
               selectedResourceType == ResourceType::Lightmap))
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
                        //showSpriteResource();
                    }break;

                    case ResourceType::Animation:
                    {
                        //showAnimationResource();
                    }break;

                    case ResourceType::Mesh:
                    {
                        //showMeshResource();
                    }break;

                    case ResourceType::Font:
                    {
                        //showFontResource();
                    }break;

                    case ResourceType::Lightmap:
                    {
                        //showLightmapResource();
                    }break;

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


    /*void ResourceBrowserWindow::showMeshResource()
    {
        int resource_count		= 3;
        int count				= 0;
        ImGuiStyle& style		= ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        auto printSameLine = [&count, &resource_count, &style, &window_visible_x2]()
        {
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 100.f;
            if (count++ + 1 < resource_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
        };

        if(ImGui::Button("Polygon", ImVec2(100.f, 100.f)))
        {
            if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
                m_WorldBuilder->addObject(Object::Mesh_Object, "Polygon", getAddObjectPosition());
        }

        printSameLine();

        if(ImGui::Button("Circle", ImVec2(100.f, 100.f)))
        {
            if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
                m_WorldBuilder->addObject(Object::Mesh_Object, "Circle", getAddObjectPosition());
        }

        printSameLine();

        if(ImGui::Button("Line", ImVec2(100.f, 100.f)))
        {
            if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
                m_WorldBuilder->addObject(Object::Mesh_Object, "Line", getAddObjectPosition());
        }
    }


    void ResourceBrowserWindow::showSpriteResource()
    {
        auto& spriteTable = m_ResourceManager->getTextureHolder()->getSpriteTable();
        int sprite_count = spriteTable.size();
        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (int n = 0; n < sprite_count; n++)
        {
            sf::Vector2u boo = m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n]).getSize();
            sf::Vector2u zoo = boo;
            if(n < sprite_count-1)
            {
                zoo = m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n+1]).getSize();
            }



            sf::Vector2f sprite_size(boo.x, boo.y);
            sprite_size = formatSize(sprite_size, 250);

            sf::Vector2f next_sprite_size(zoo.x, zoo.y);
            next_sprite_size = formatSize(next_sprite_size, 250);


            ImVec2 button_size(sprite_size.x, sprite_size.y);


            if(ImGui::ImageButton(m_ResourceManager->getTextureHolder()->getSpriteTexture(spriteTable[n]), button_size))
            {
                if(m_WorldBuilder && m_BuilderMode == BuilderMode::OBJECT)
                    m_WorldBuilder->addObject(Object::Sprite_Object, spriteTable[n], getAddObjectPosition());
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Button("Delete"))
                {
                    ImGui::CloseCurrentPopup();
                }

                //if (ImGui::Button("Close"))
                    //ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if(ImGui::IsItemHovered())
            {
                //nero_log("hover animation button");
            }

            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
            if (n + 1 < sprite_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
        }
    }

    void ResourceBrowserWindow::showLightmapResource()
    {
        auto& spriteTable = m_ResourceManager->getLightmapHolder()->getSpriteTable();
        int sprite_count = spriteTable.size();
        ImGuiStyle& style = ImGui::GetStyle();
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (int n = 0; n < sprite_count; n++)
        {
            sf::Vector2u boo = m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n]).getSize();
            sf::Vector2u zoo = boo;
            if(n < sprite_count-1)
            {
                zoo = m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n+1]).getSize();
            }



            sf::Vector2f sprite_size(boo.x, boo.y);
            sprite_size = formatSize(sprite_size, 250);

            sf::Vector2f next_sprite_size(zoo.x, zoo.y);
            next_sprite_size = formatSize(next_sprite_size, 250);


            ImVec2 button_size(sprite_size.x, sprite_size.y);


            if(ImGui::ImageButton(m_ResourceManager->getLightmapHolder()->getSpriteTexture(spriteTable[n]), button_size))
            {
                if(m_AdvancedScene && m_BuilderMode == BuilderMode::OBJECT)
                {
                    //m_AdvancedScene->addObject(Object::Light_Object, spriteTable[n], getAddObjectPosition(), m_EditorMode);
                }
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::Button("Delete"))
                {
                    ImGui::CloseCurrentPopup();
                }

                //(ImGui::Button("Close"))
                    //ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if(ImGui::IsItemHovered())
            {
                //nero_log("hover animation button");
            }

            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + next_sprite_size.x;
            if (n + 1 < sprite_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
        }
    }

    void ResourceBrowserWindow::showAnimationResource()
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
}
