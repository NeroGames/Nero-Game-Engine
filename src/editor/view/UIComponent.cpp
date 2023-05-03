////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/view/UIComponent.h>
////////////////////////////////////////////////////////////
namespace nero
{
    UIComponent::UIComponent(EditorContext::Ptr editorContext)
        : m_EditorContext(std::move(editorContext))
    {
    }

    UIComponent::~UIComponent()
    {
    }

    void UIComponent::update(const sf::Time& timeStep)
    {
    }
} // namespace nero
