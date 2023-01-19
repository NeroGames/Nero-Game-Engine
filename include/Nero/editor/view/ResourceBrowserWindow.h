////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERWINDOW_H
#define RESOURCEBROWSERWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/ResourceBrowserSpriteView.h>
#include <Nero/editor/view/ResourceBrowserMeshView.h>
#include <Nero/editor/view/ResourceBrowserLightmapView.h>
#include <Nero/editor/view/ResourceBrowserAnimationView.h>
#include <Nero/editor/view/ResourceBrowserFontView.h>
#include <Nero/core/cpp/resource/ResourceUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserWindow : public UIComponent
    {
      public:
        ResourceBrowserWindow(EditorContext::Ptr editorContext);
        virtual ~ResourceBrowserWindow() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        void saveResourceFile(const ResourceType&            resourceType,
                              const std::vector<std::string> loadedFileTable);
        bool loadableResource(const ResourceType& resourceType);

      private:
        ResourceBrowserSpriteView    m_ResourceBrowserSpriteView;
        ResourceBrowserAnimationView m_ResourceBrowserAnimationView;
        ResourceBrowserMeshView      m_ResourceBrowserMeshView;
        ResourceBrowserFontView      m_ResourceBrowserFontView;
        ResourceBrowserLightmapView  m_ResourceBrowserLightmapView;
    };
} // namespace nero
#endif // RESOURCEBROWSERWINDOW_H
