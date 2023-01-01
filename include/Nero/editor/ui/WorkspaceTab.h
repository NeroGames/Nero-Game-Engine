////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WORKSPACETAB_H
#define WORKSPACETAB_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/ui/NewWorkspaceView.h>
#include <Nero/editor/ui/ImportWorkspaceView.h>
#include <Nero/editor/ui/WorkspaceListView.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WorkspaceTab : public UIComponent
    {
        public:
                                    WorkspaceTab(EditorContext::Ptr editorContext,
                                                 TabSelectionHandler::Ptr tabSelectionHandler);
            virtual                ~WorkspaceTab() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            TabSelectionHandler::Ptr    m_TabSelectionHandler;
            NewWorkspaceView            m_NewWorkspaceView;
            ImportWorkspaceView         m_ImportWorkspaceView;
            WorkspaceListView           m_WorkspaceListView;
	};
}
#endif // WORKSPACETAB_H
