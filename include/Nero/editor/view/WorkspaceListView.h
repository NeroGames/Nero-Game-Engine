////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WORKSPACELISTVIEW_H
#define WORKSPACELISTVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WorkspaceListView : public UIComponent
    {
      public:
        WorkspaceListView(EditorContext::Ptr editorContext);
        virtual ~WorkspaceListView() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // WORKSPACELISTVIEW_H
