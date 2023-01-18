////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef OPENPROJECTTAB_H
#define OPENPROJECTTAB_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class OpenProjectTab : public UIComponent {
      public:
        OpenProjectTab(EditorContext::Ptr       editorContext,
                       TabSelectionHandler::Ptr tabSelectionHandler);
        virtual ~OpenProjectTab() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        TabSelectionHandler::Ptr m_TabSelectionHandler;
    };
} // namespace nero
#endif // OPENPROJECTTAB_H
