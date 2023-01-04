////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORCONTEXT_H
#define EDITORCONTEXT_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/EditorProxy.h>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/project/ProjectManager.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorContext
    {
        public:
            using Ptr = std::shared_ptr<EditorContext>;

        public:
                                    EditorContext(EditorProxy::Ptr editorProxy,
                                                  ProjectManager::Ptr projectManager,
                                                  TextureHolder::Ptr textureHolder,
                                                  Setting::Ptr editorSetting,
                                                  const EditorMode& editorMode = EditorMode::WORLD_BUILDER,
                                                  const BuilderMode& builderMode = BuilderMode::OBJECT);
            virtual                ~EditorContext();

            // Getter
            EditorProxy::Ptr        getEditorProxy()            const;
            ProjectManager::Ptr     getProjectManager()         const;
            GameProject::Ptr        getGameProject()            const;
            EditorMode              getEditorMode()             const;
            BuilderMode             getBuilderMode()            const;
            TextureHolder::Ptr      getTextureHolder()          const;
            Setting::Ptr            getEditorSetting()          const;
            std::string             getSelectedGameLevelName()  const;
            std::string             getOpengedGameLevelName()   const;
            // Setter
            void                    setEditorMode(const EditorMode& editorMode);
            void                    setBuilderMode(const BuilderMode& builderMode);
            void                    setSelectedGameLevelName(const std::string& levelName);
            void                    setOpenedGameLevelName(const std::string& levelName);

        private:
            EditorProxy::Ptr        m_EditorProxy;
            ProjectManager::Ptr     m_ProjectManager;
            TextureHolder::Ptr      m_TextureHolder;
            Setting::Ptr            m_EditorSetting;
            EditorMode              m_EditorMode;
            BuilderMode             m_BuilderMode;
            //Game Level
            std::string             m_SelectedGameLevelName;
            std::string             m_OpenedGameLevelName;
	};
}

#endif // EDITORCONTEXT_H
