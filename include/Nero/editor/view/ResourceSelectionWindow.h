////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RESOURCESELECTIONWINDOW_H
#define RESOURCESELECTIONWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceSelectionWindow : public UIComponent
    {
      public:
        ResourceSelectionWindow(EditorContext::Ptr editorContext);
        virtual ~ResourceSelectionWindow() override;

        virtual void destroy() override;
        virtual void render() override;
    };
} // namespace nero
#endif // WORKSPACELISTVIEW_H
