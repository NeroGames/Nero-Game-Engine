////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMEOBJECTPROPERTYVIEW_H
#define GAMEOBJECTPROPERTYVIEW_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GameObjectPropertyView : public UIComponent {
      public:
        GameObjectPropertyView(EditorContext::Ptr editorContext);
        virtual ~GameObjectPropertyView() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        std::vector<Object::Ptr> getComponentTable(Object::Ptr root);
        void                     getComponentTable(Object::Ptr               object,
                                                   std::vector<Object::Ptr>& result);
    };
} // namespace nero
#endif // GAMEOBJECTPROPERTYVIEW_H
