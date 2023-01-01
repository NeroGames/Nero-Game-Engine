////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PROJECTMANAGERPOPUP_H
#define PROJECTMANAGERPOPUP_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/ui/RecentProjectTab.h>
#include <Nero/editor/ui/NewProjectTab.h>
#include <Nero/editor/ui/OpenProjectTab.h>
#include <Nero/editor/ui/WorkspaceTab.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ProjectManagerPopup : public UIComponent
    {
        public:
                                        ProjectManagerPopup(EditorContext::Ptr editorContext);
            virtual                    ~ProjectManagerPopup() override;

            virtual void                destroy() override;
            virtual void                render() override;

            TabSelectionHandler::Ptr    getTabSelectionHandler() const;

        private:
            TabSelectionHandler::Ptr    m_TabSelectionHandler;
            RecentProjectTab            m_RecentProjectTab;
            NewProjectTab               m_NewProjectTab;
            OpenProjectTab              m_OpenProjectTab;
            WorkspaceTab                m_WorkspaceTab;

	};
}
#endif // PROJECTMANAGERPOPUP_H
