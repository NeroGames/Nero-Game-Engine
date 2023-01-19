////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUP_H
#define EDITORSETUP_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/EditorContext.h>
#include <Nero/editor/view/UIComponent.h>
// Std
#include <functional>
#include <vector>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetup
    {
      public:
        using Ptr = std::shared_ptr<EditorSetup>;

      public:
        EditorSetup(EditorContext::Ptr editorContext);

      public:
        bool initiateSetup();
        bool nextSetup();
        bool setupCodeEditor();
        bool setupTexturePacker();
        bool setupWorkspace();
        void setNextSetup(const bool& flag);
        void setSetupCodeEditor(const bool& flag);
        void setSetupTexturePacker(const bool& flag);
        void setSetupWorkspace(const bool& flag);

      private:
        bool m_SetupCodeEditor;
        bool m_SetupTexturePacker;
        bool m_SetupWorkspace;
        bool m_SelectQtCreator;
        bool m_SelectVisualStudio;
        bool m_CreateWorkspace;
        bool m_ImportWorkspace;
        bool m_NextSetup;
    };
} // namespace nero

#endif // EDITORSETUP_H
