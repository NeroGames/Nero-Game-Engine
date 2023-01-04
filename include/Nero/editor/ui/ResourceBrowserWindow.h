////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCEBROWSERWINDOW_H
#define RESOURCEBROWSERWINDOW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/core/cpp/resource/ResourceUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceBrowserWindow : public UIComponent
    {
        public:
                                    ResourceBrowserWindow(EditorContext::Ptr editorContext);
            virtual                ~ResourceBrowserWindow() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    saveResourceFile(const ResourceType& resourceType,
                                                     const std::vector<std::string> loadedFileTable);
            //TODO remove
            //void                    showSpriteResource();
            //void                    showAnimationResource();
            //void                    showMeshResource();
            //void                    showFontResource();
            //void                    showLightmapResource();
	};
}
#endif // RESOURCEBROWSERWINDOW_H
