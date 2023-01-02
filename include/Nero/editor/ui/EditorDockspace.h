////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORDOCKSPACE_H
#define EDITORDOCKSPACE_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/ui/EditorMenubar.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorDockspace : public UIComponent
    {
        public:
                                    EditorDockspace(EditorContext::Ptr editorContext);
            virtual                ~EditorDockspace() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            ImGuiID                 m_DockspaceID;
            bool					m_BuildDockspaceLayout;
            bool					m_SetupDockspaceLayout;
            EditorMenubar           m_EditorMenubar;
	};
}
#endif // EDITORDOCKSPACE_H
