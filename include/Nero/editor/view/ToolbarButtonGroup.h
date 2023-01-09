////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef TOOLBARBUTTONGROUP_H
#define TOOLBARBUTTONGROUP_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/view/UIComponent.h>
#include <Nero/editor/view/ToolbarScrollButton.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ToolbarButtonGroup : public UIComponent
    {
        public:
                                    ToolbarButtonGroup(EditorContext::Ptr editorContext);
            virtual                ~ToolbarButtonGroup() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            void                    renderLeftButtonGroup();
            void                    renderMiddleButtonGroup();
            void                    renderRightButtonGroup();
            void                    renderWorldButton();
            void                    renderScreenButton();
            void                    renderFactoryButton();
	};
}
#endif // TOOLBARBUTTONGROUP_H
