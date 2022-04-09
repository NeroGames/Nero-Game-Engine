////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/LayerObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LayerObject::LayerObject(): Object()
        ,m_Order(-1)
        ,m_Color(sf::Color::Black)
    {
        setFirstType(Object::Layer_Object);
    }

    bool LayerObject::isEmpty() const
    {
        return getChildCount() <= 0;
    }

    void LayerObject::setOrder(const int& order)
    {
        m_Order = order;
    }

    int LayerObject::getOrder() const
    {
        return m_Order;
    }

    Object::Ptr LayerObject::clone() const
    {
        LayerObject::Ptr object(new LayerObject());

        Object::clone<LayerObject::Ptr>(object);

        object->setOrder(getOrder());

        return object;
    }

    nlohmann::json LayerObject::toJson() const
    {
        nlohmann::json layerJson = Object::toJson();

        layerJson["order"] = getOrder();

        switch(getSecondType())
        {
            case Object::Sprite_Object:
            {
                layerJson["type"] = "sprite_layer";

                std::vector<nlohmann::json> spriteJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                    spriteJsonTab.push_back((*it)->toJson());

                layerJson["sprite_table"] = spriteJsonTab;

                return layerJson;
            }

            case Object::Mesh_Object:
            {
                layerJson["type"] = "mesh_layer";

                std::vector<nlohmann::json> meshJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                    meshJsonTab.push_back((*it)->toJson());

                layerJson["mesh_table"] = meshJsonTab;

                return layerJson;
            }

            case Object::Meshed_Object:
            {
                layerJson["type"] = "meshed_layer";

                std::vector<nlohmann::json> meshedJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                {
                    nlohmann::json meshedJson;
                    meshedJson["sprite"]    = (*it)->toJson();
                    meshedJson["mesh"]      = (*it)->getFirstChild()->toJson();

                    meshedJsonTab.push_back(meshedJson);
                }

                layerJson["meshed_table"] = meshedJsonTab;

                return layerJson;
            }

            case Object::Animation_Object:
            {
                layerJson["type"] = "animation_layer";

                std::vector<nlohmann::json> aniamtionJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                    aniamtionJsonTab.push_back((*it)->toJson());

                layerJson["animation_table"] = aniamtionJsonTab;

                return layerJson;
            }

            case Object::Animation_Meshed_Object:
            {
                layerJson["type"] = "animation_meshed_layer";

                std::vector<nlohmann::json> meshedJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                {
                    nlohmann::json meshedJson;
                    meshedJson["animation"]     = (*it)->toJson();
                    meshedJson["mesh"]          = (*it)->getFirstChild()->toJson();

                    meshedJsonTab.push_back(meshedJson);
                }

                layerJson["animation_meshed_table"] = meshedJsonTab;

                return layerJson;
            }

            case Object::Button_Object:
            {
                layerJson["type"] = "button_layer";

                std::vector<nlohmann::json> buttonJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                    buttonJsonTab.push_back((*it)->toJson());

                layerJson["button_table"] = buttonJsonTab;

                return layerJson;
            }

            case Object::Text_Object:
            {
                layerJson["type"] = "text_layer";

                std::vector<nlohmann::json> textJsonTab;

                auto childTab = getAllChild();

                for(auto it = childTab->begin(); it != childTab->end(); it++)
                    textJsonTab.push_back((*it)->toJson());

                layerJson["text_table"] = textJsonTab;

                return layerJson;
            }

            case Object::None:
            {
                layerJson["type"] = "empty_layer";

                return layerJson;
            }
        }
    }

    void LayerObject::setColor(const sf::Color& color)
    {
        if(getSecondType() != Object::Sprite_Object || getSecondType() != Object::Meshed_Object)
            return;

        m_Color = color;

        auto childTab = getAllChild();

        for(auto it = childTab->begin(); it != childTab->end(); it++)
        {
           (*it)->setColor(color);
        }
    }

    const sf::Color& LayerObject::getColor() const
    {
        return m_Color;
    }

    LayerObject::Ptr LayerObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Layer_Object)
            return nullptr;

        return  std::static_pointer_cast<LayerObject>(object);
    }
}
