////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORSETUPTEXTUREPACKERVIEW_H
#define EDITORSETUPTEXTUREPACKERVIEW_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/EditorSetup.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorSetupTexturePackerView : public UIComponent
    {
        public:
                                    EditorSetupTexturePackerView(EditorContext::Ptr editorContext,
                                                                 EditorSetup::Ptr editorSetup);
            virtual                ~EditorSetupTexturePackerView() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            EditorSetup::Ptr        m_EditorSetup;
	};
}
#endif // EDITORSETUPTEXTUREPACKERVIEW_H
