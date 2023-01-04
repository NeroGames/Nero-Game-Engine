////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef NEWWORKSPACEVIEW_H
#define NEWWORKSPACEVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class NewWorkspaceView : public UIComponent
    {
        public:
                                    NewWorkspaceView(EditorContext::Ptr editorContext);
            virtual                ~NewWorkspaceView() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    clearInput();

        private:
            struct WorkspaceInput
            {
                //create
                char            location[256];
                char            name[100];
                char            company[100];
                char            projectLead[100];
                char            projectNamespace[11];
                std::string     errorMessage;
                std::string     redirectLink;
                bool            error;
            };

        private:
            WorkspaceInput          m_Input;
	};
}
#endif // NEWWORKSPACEVIEW_H
