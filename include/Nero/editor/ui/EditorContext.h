////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORCONTEXT_H
#define EDITORCONTEXT_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/project/ProjectManager.h>
#include <Nero/editor/EditorUtility.h>
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
                                    EditorContext(ProjectManager::Ptr projectManager,
                                                  TextureHolder::Ptr textureHolder,
                                                  const EditorMode& editorMode = EditorMode::WORLD_BUILDER,
                                                  const BuilderMode& builderMode = BuilderMode::OBJECT);
            virtual                ~EditorContext();

            // Getter
            GameProject::Ptr        getGameProject();
            EditorMode              getEditorMode()     const;
            BuilderMode             getBuilderMode()    const;
            TextureHolder::Ptr      getTextureHolder()  const;
            //Setter
            void                    setEditorMode(const EditorMode& editorMode);
            void                    setBuilderMode(const BuilderMode& builderMode);

        private:
            ProjectManager::Ptr     m_ProjectManager;
            TextureHolder::Ptr      m_TextureHolder;
            EditorMode              m_EditorMode;
            BuilderMode             m_BuilderMode;
	};
}

#endif // EDITORCONTEXT_H
