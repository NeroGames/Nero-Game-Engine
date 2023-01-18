////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUPFINISHVIEW_H
#define EDITORSETUPFINISHVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/EditorSetup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetupFinishView : public UIComponent {
      public:
        EditorSetupFinishView(EditorContext::Ptr editorContext, EditorSetup::Ptr editorSetup);
        virtual ~EditorSetupFinishView() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        EditorSetup::Ptr m_EditorSetup;
    };
} // namespace nero
#endif // EDITORSETUPFINISHVIEW_H
