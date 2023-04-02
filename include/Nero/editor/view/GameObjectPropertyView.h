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
    class GameObjectPropertyView : public UIComponent
    {
      public:
        GameObjectPropertyView(EditorContext::Ptr editorContext);
        virtual ~GameObjectPropertyView() override;

        virtual void destroy() override;
        virtual void render() override;

      private:
        std::vector<Object::Ptr> getComponentTable(Object::Ptr root);
        void getComponentTable(Object::Ptr object, std::vector<Object::Ptr>& result);

        void renderGeneralProperty(Object::Ptr selectedObject);
        void renderTransformProperty(Object::Ptr selectedObject);
        void renderColorProperty(Object::Ptr selectedObject);
        void renderComponentProperty(Object::Ptr selectedObject);
        void renderSpriteProperty(SpriteObject::Ptr spriteObject);
        void renderTextProperty(TextObject::Ptr textObject);
        void renderAnimationProperty(AnimationObject::Ptr animationObject);
        void renderLightProperty(LightIcon::Ptr lightIconObject);

      private:
        ImVec4 m_GameObjectColor;
        ImVec4 m_TextOutlineColor;
        int    m_SelectedMeshType;
    };
} // namespace nero
#endif // GAMEOBJECTPROPERTYVIEW_H
