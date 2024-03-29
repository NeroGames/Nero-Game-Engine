////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
// Std
#include <algorithm>
#include <cassert>
////////////////////////////////////////////////////////////
namespace nero
{
    Object::Object()
        : m_Id(-1)
        , m_Parent(nullptr)
        , m_Name("no_name")
        , m_Category("no_category")
        , m_IsVisible(true)
        , m_IsUpdateable(true)
        , m_IsSelectable(true)
        , m_IsSelected(false)
        , m_FirstType(Object::None)
        , m_SecondType(Object::None)
        , m_ChildTab()
    {
        // Empty
    }

    void Object::setId(const int& id)
    {
        m_Id = id;
    }

    // TODO : remove
    const int& Object::getId() const
    {
        return m_Id;
    }

    const int& Object::getObjectId() const
    {
        return m_Id;
    }

    void Object::setName(const std::string& name)
    {
        m_Name = name;
    }

    const std::string& Object::getName() const
    {
        return m_Name;
    }

    void Object::setCategory(const std::string& category)
    {
        m_Category = category;
    }

    const std::string& Object::getCategory() const
    {
        return m_Category;
    }

    void Object::setFirstType(const Type& type)
    {
        m_FirstType = type;
    }

    const Object::Type& Object::getFirstType() const
    {
        return m_FirstType;
    }

    void Object::setSecondType(const Type& type)
    {
        m_SecondType = type;
    }

    const Object::Type& Object::getSecondType() const
    {
        return m_SecondType;
    }

    void Object::setIsVisible(bool flag)
    {
        m_IsVisible = flag;
    }

    void Object::setIsUpdateable(bool flag)
    {
        m_IsUpdateable = flag;
    }

    void Object::setIsSelectable(bool flag)
    {
        m_IsSelectable = flag;
    }

    bool Object::isVisible() const
    {
        return m_IsVisible;
    }

    bool Object::isUpdateable() const
    {
        return m_IsUpdateable;
    }

    bool Object::isSelectable() const
    {
        return m_IsSelectable;
    }

    bool Object::isSelected() const
    {
        if(!isSelectable())
            return false;

        return m_IsSelected;
    }

    void Object::setIsSelected(bool flag)
    {
        m_IsSelected = flag;
    }

    sf::Vector2f Object::getWorldPosition() const
    {
        return getWorldTransform() * sf::Vector2f();
    }

    sf::Transform Object::getWorldTransform() const
    {
        sf::Transform transform = sf::Transform::Identity;

        for(const Object* object = this; object != nullptr; object = object->m_Parent)
            transform = object->getTransform() * transform;

        return transform;
    }

    sf::FloatRect Object::getGlobalBounds() const
    {
        return sf::FloatRect();
    }

    void Object::addChild(Object::Ptr child)
    {
        child->m_Parent = this;
        m_ChildTab.push_back(std::move(child));
    }

    Object::Ptr Object::removeChild(const Object::Ptr& child)
    {
        auto found = std::find_if(m_ChildTab.begin(),
                                  m_ChildTab.end(),
                                  [&](Object::Ptr& p)
                                  {
                                      return p.get() == child.get();
                                  });
        assert(found != m_ChildTab.end());

        Object::Ptr result = std::move(*found);
        result->m_Parent   = nullptr;
        m_ChildTab.erase(found);

        return result;
    }

    void Object::removeAllChild()
    {
        for(auto it = m_ChildTab.begin(); it != m_ChildTab.end();)
        {
            Object::Ptr result = std::move(*it);
            result->m_Parent   = nullptr;
            it                 = m_ChildTab.erase(it);
        }
    }

    const std::vector<Object::Ptr>* Object::getAllChild() const
    {
        return &m_ChildTab;
    }

    std::vector<Object::Ptr>* Object::getAllChild()
    {
        return &m_ChildTab;
    }

    Object::Ptr Object::getChild(const int& index) const
    {
        if(index >= m_ChildTab.size())
            return nullptr;

        return m_ChildTab[index];
    }

    Object::Ptr Object::getFirstChild() const
    {
        return getChild(0);
    }

    Object::Ptr Object::removeChild(const int& index)
    {
        auto        it     = m_ChildTab.begin() + index;

        Object::Ptr result = std::move(*it);
        result->m_Parent   = nullptr;
        it                 = m_ChildTab.erase(it);

        return result;
    }

    Object::Ptr Object::removeFirstChild()
    {
        return removeChild(0);
    }

    const int Object::getChildCount() const
    {
        return m_ChildTab.size();
    }

    void Object::setColor(const sf::Color& color)
    {
        return;
    }

    const sf::Color& Object::getColor() const
    {
        return sf::Color::Black;
    }

    Object::Ptr Object::clone(sf::Vector2f& position) const
    {
        return nullptr;
    }

    Object::Ptr Object::clone() const
    {
        return nullptr;
    }

    nlohmann::json Object::toJson() const
    {
        nlohmann::json objectJson;

        objectJson["object_id"]     = getId();
        objectJson["name"]          = getName();
        objectJson["category"]      = getCategory();
        objectJson["first_type"]    = getTypeString(getFirstType());
        objectJson["second_type"]   = getTypeString(getSecondType());
        objectJson["is_visible"]    = isVisible();
        objectJson["is_updateable"] = isUpdateable();
        objectJson["is_selectable"] = isSelectable();
        objectJson["is_selected"]   = isSelected();
        objectJson["child_count"]   = getChildCount();

        return objectJson;
    }

    sf::String Object::toString() const
    {
        return toJson().dump(4);
    }

    void Object::update(sf::Time time_step)
    {
        if(!isUpdateable())
            return;

        updateObject(time_step);
        updateAllChild(time_step);
    }

    void Object::updateObject(sf::Time time_step)
    {
        return;
    }

    void Object::updateAllChild(sf::Time time_step)
    {
        for(Object::Ptr& child : m_ChildTab)
            child->update(time_step);
    }

    void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if(!isVisible())
            return;

        states.transform *= getTransform(); // Apply transform of current node

        drawObject(target, states);
        drawAllChild(target, states);
    }

    void Object::drawObject(sf::RenderTarget&, sf::RenderStates) const
    {
        return;
    }

    void Object::drawAllChild(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(const Object::Ptr& child : m_ChildTab)
            child->draw(target, states);
    }

    Object::Ptr Object::Cast(Object::Ptr object)
    {
        return object;
    }

    sf::Vector2f Object::getCenter() const
    {
        return sf::Vector2f();
    }

    std::string Object::getTypeString(const Object::Type& type) const
    {
        switch(type)
        {
            case Sprite_Object:
                return "sprite_object";

            case Mesh_Object:
                return "mesh_object";

            case Physic_Object:
                return "physic_object";

            case Meshed_Object:
                return "meshed_object";

            case Solid_Object:
                return "solid_object";

            case Layer_Object:
                return "layer_object";

            case Animation_Object:
                return "animation_object";

            case Animation_Meshed_Object:
                return "animation_meshed_object";

            case Animation_Solid_Object:
                return "animation_solid_object";

            case Text_Object:
                return "text_object";

            case UI_Object:
                return "ui_object";

            case Button_Object:
                return "button_object";

            case Light_Object:
                return "light_object";

            default:
                return "no_type";
        }
    }

    std::string Object::getTypeString() const
    {
        switch(getSecondType())
        {
            case Sprite_Object:
                return "Sprite Object";

            case Mesh_Object:
                return "Mesh Object";

            case Physic_Object:
                return "Physic Object";

            case Meshed_Object:
                return "Meshed Object";

            case Solid_Object:
                return "Solid Object";

            case Layer_Object:
                return "Layer Object";

            case Animation_Object:
                return "Animation Object";

            case Animation_Meshed_Object:
                return "Meshed Animation Object";

            case Animation_Solid_Object:
                return "Solid Animation Object";

            case Text_Object:
                return "Text Object";

            case UI_Object:
                return "UI Object";

            case Button_Object:
                return "Button Object";

            case Light_Object:
                return "Light Object";

            default:
                return "No Type";
        }
    }

    Object* Object::getParent() const
    {
        return m_Parent;
    }

} // namespace nero
