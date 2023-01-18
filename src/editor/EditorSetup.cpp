////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/EditorSetup.h>
#include <Nero/core/cpp/utility/String.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorSetup::EditorSetup(EditorContext::Ptr editorContext)
    {
        Setting setting   = editorContext->getEditorSetting()->getSetting("environment");

        m_SetupCodeEditor = setting.getString("qt_creator") == StringPool.BLANK &&
                            setting.getString("visual_studio") == StringPool.BLANK;
        m_SetupTexturePacker = setting.getString("texture_packer") == StringPool.BLANK;
        m_SetupWorkspace     = editorContext->getProjectManager()->getWorkspaceTable().empty();
    }

    bool EditorSetup::initiateSetup()
    {
        return m_SetupCodeEditor || m_SetupTexturePacker || m_SetupWorkspace;
    }

    bool EditorSetup::nextSetup()
    {
        return m_NextSetup;
    }

    bool EditorSetup::setupCodeEditor()
    {
        return m_SetupCodeEditor;
    }

    bool EditorSetup::setupTexturePacker()
    {
        return m_SetupTexturePacker;
    }

    bool EditorSetup::setupWorkspace()
    {
        return m_SetupWorkspace;
    }

    void EditorSetup::setNextSetup(const bool& flag)
    {
        m_NextSetup = flag;
    }

    void EditorSetup::setSetupCodeEditor(const bool& flag)
    {
        m_SetupCodeEditor = flag;
    }

    void EditorSetup::setSetupTexturePacker(const bool& flag)
    {
        m_SetupTexturePacker = flag;
    }

    void EditorSetup::setSetupWorkspace(const bool& flag)
    {
        m_SetupWorkspace = flag;
    }
} // namespace nero
