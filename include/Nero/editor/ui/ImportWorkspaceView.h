////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef IMPORTWORKSPACEVIEW_H
#define IMPORTWORKSPACEVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ImportWorkspaceView : public UIComponent
    {
        public:
                                    ImportWorkspaceView(EditorContext::Ptr editorContext);
            virtual                ~ImportWorkspaceView() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    clearInput();
            bool                    workspaceDocumentValid(const std::string&  workspaceDirectory);

        private:
            struct WorkspaceInput
            {
                char						locationImport[256];
                std::string errorMessage	= StringPool.BLANK;
                std::string redirectLink	= StringPool.BLANK;
                bool error					= true;
            };

        private:
            WorkspaceInput          m_Input;
	};
}
#endif // IMPORTWORKSPACEVIEW_H
