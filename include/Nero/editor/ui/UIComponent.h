////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef UICOMPONENT_H
#define UICOMPONENT_H
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////
namespace nero
{
    class UIComponent
    {
        public:
            struct EditorContext
            {

            };

        public:
                                    UIComponent(EditorContext editorContext);
            virtual                ~UIComponent();

            virtual void 			destroy()   = 0;
            virtual void            render()    = 0;

        protected:
            EditorContext           m_EditorContext;
	};
}

#endif // UICOMPONENT_H
