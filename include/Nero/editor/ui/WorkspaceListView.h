////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WORKSPACELISTVIEW_H
#define WORKSPACELISTVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WorkspaceListView : public UIComponent
    {
        public:
                                    WorkspaceListView(EditorContext::Ptr editorContext);
            virtual                ~WorkspaceListView() override;

            virtual void 			destroy() override;
            virtual void            render() override;
	};
}
#endif // WORKSPACELISTVIEW_H
