////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef RECENTPROJECTTAB_H
#define RECENTPROJECTTAB_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/UIComponent.h>
#include <Nero/editor/EditorUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class RecentProjectTab : public UIComponent
    {
        public:
                                    RecentProjectTab(EditorContext::Ptr editorContext,
                                                     TabSelectionHandler::Ptr tabSelectionHandler);
            virtual                ~RecentProjectTab() override;

            virtual void 			destroy() override;
            virtual void            render() override;

        private:
            TabSelectionHandler::Ptr    m_TabSelectionHandler;
	};
}
#endif // RECENTPROJECTTAB_H
