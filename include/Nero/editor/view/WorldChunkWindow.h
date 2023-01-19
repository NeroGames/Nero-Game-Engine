////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WORLDCHUNKWINDOW_H
#define WORLDCHUNKWINDOW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WorldChunkWindow : public UIComponent
    {
      public:
        WorldChunkWindow(EditorContext::Ptr editorContext);
        virtual ~WorldChunkWindow() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        int m_SelectedChunkId;
    };
} // namespace nero
#endif // WORLDCHUNKWINDOW_H
