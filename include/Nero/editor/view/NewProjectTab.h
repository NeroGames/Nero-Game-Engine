////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NEWPROJECTTAB_H
#define NEWPROJECTTAB_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NewProjectTab : public UIComponent
    {
        public:
                                    NewProjectTab(EditorContext::Ptr editorContext,
                                                     TabSelectionHandler::Ptr tabSelectionHandler);
            virtual                ~NewProjectTab() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    clearInput();
            void                    updateInput(const nlohmann::json workspaceTable);

        private:
            struct ProjectInput
            {
                char                        name[100];
                char                        projectLead[100];
                char                        company[100];
                char                        projectNamespace[11];
                char                        description[512];
                const char*                 projectType;
                const char*                 codeEditor;
                const char*					workspace;
                int                         projectTypeIndex    = 0;
                int                         codeEditorIndex     = 0;
                int                         workspaceIndex      = 0;

                bool						startupPack;
                std::string                 lastCreatedProject;

                std::string errorMessage	= StringPool.BLANK;
                std::string redirectLink	= StringPool.BLANK;
                bool error					= true;
            };


        private:
            TabSelectionHandler::Ptr    m_TabSelectionHandler;
            ProjectInput                m_Input;
            unsigned int                m_NewProjectCount;
	};
}
#endif // NEWPROJECTTAB_H
