////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorContext.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorContext::EditorContext(ProjectManager::Ptr projectManager,
                                 TextureHolder::Ptr textureHolder,
                                 const EditorMode& editorMode,
                                 const BuilderMode& builderMode):
         m_ProjectManager(projectManager)
        ,m_TextureHolder(textureHolder)
        ,m_EditorMode(editorMode)
        ,m_BuilderMode(builderMode)
    {

    }

    EditorContext::~EditorContext()
    {

    }

    GameProject::Ptr EditorContext::getGameProject()
    {
        if(m_ProjectManager)
        {
            return m_ProjectManager->getGameProject();
        }

        return nullptr;
    }

    TextureHolder::Ptr EditorContext::getTextureHolder() const
    {
        return m_TextureHolder;
    }

    EditorMode EditorContext::getEditorMode() const
    {
        return m_EditorMode;
    }

    BuilderMode EditorContext::getBuilderMode() const
    {
        return m_BuilderMode;
    }

    void EditorContext::setEditorMode(const EditorMode& editorMode)
    {
        m_EditorMode = editorMode;
    }

    void EditorContext::setBuilderMode(const BuilderMode& builderMode)
    {
        m_BuilderMode = builderMode;
    }
}
