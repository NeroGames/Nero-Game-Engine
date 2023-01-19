////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/scene/CollisionRule.h>
// STD
#include <iostream>
////////////////////////////////////////////////////////////
namespace nero
{
    CollisionRule::CollisionRule()
    {
        m_CatecotryBitTab = {0x0000,
                             0x0001,
                             0x0002,
                             0x0004,
                             0x0008,
                             0x0010,
                             0x0020,
                             0x0040,
                             0x0080,
                             0x0100,
                             0x0200,
                             0x0400,
                             0x0800,
                             0x1000,
                             0x2000,
                             0x3000,
                             0x8000,
                             0xFFFF};
    }

    CollisionRule::~CollisionRule()
    {
        // dtor
    }

    void CollisionRule::canCollide(std::string categoryA, std::string categoryB)
    {
        m_CollisionMap[categoryA].insert(categoryB);
        m_CollisionMap[categoryB].insert(categoryA);
    }

    void CollisionRule::print()
    {
        std::cout << "\n";

        for(auto it = m_CollisionMap.begin(); it != m_CollisionMap.end(); it++)
        {
            std::cout << it->first << " Can Collide with /";

            for(std::string cat : it->second)
                std::cout << cat << " / ";

            std::cout << "\n";
        }

        std::cout << "\n";
    }

    void CollisionRule::printCategoryTab()
    {
        std::cout << "\n";
        std::cout << "Category bits";
        std::cout << "\n";

        for(auto it = m_CatecotryBitTab.begin(); it != m_CatecotryBitTab.end(); it++)
        {
            std::cout << *it << "\n";
        }

        std::cout << "\n";
    }

    void CollisionRule::updateCategoryBit()
    {
        int i = 1;
        for(auto cat : m_CollisionMap)
        {
            m_CollisionRuleMap[cat.first].first = m_CatecotryBitTab[i];
            i++;
        }
    }

    void CollisionRule::updateCategoryMaskBit()
    {
        for(auto cat : m_CollisionMap)
        {
            m_CollisionRuleMap[cat.first].second = 0x0000;

            for(std::string mask : cat.second)
            {
                m_CollisionRuleMap[cat.first].second =
                    m_CollisionRuleMap[cat.first].second | m_CollisionRuleMap[mask].first;
            }
        }
    }

    void CollisionRule::printBit()
    {
        std::cout << "\n";

        for(auto it = m_CollisionRuleMap.begin(); it != m_CollisionRuleMap.end(); it++)
        {
            std::cout << it->first << " :: ";

            std::cout << it->second.first << " / " << it->second.second;

            std::cout << "\n";
        }

        std::cout << "\n";
    }

    void CollisionRule::apply(Object::Ptr rootObject)
    {
        updateCategoryBit();
        updateCategoryMaskBit();
        noCollision(rootObject);

        auto layerTab = rootObject->getAllChild();

        for(auto layer = layerTab->begin(); layer != layerTab->end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Physic_Object ||
               (*layer)->getSecondType() == Object::Solid_Object ||
               (*layer)->getSecondType() == Object::Animation_Solid_Object)
            {
                auto objectTab = (*layer)->getAllChild();

                for(auto it = objectTab->begin(); it != objectTab->end(); it++)
                {
                    PhysicObject::Ptr  physic_object = PhysicObject::Cast((*it));

                    std::string        category      = physic_object->getCategory();

                    PhysicObject::Rule rule;
                    rule.categoryBits = m_CollisionRuleMap[category].first;
                    rule.maskBits     = m_CollisionRuleMap[category].second;

                    physic_object->setCollisionRule(rule);
                }
            }
        }
    }

    void CollisionRule::fullCollision(Object::Ptr rootObject)
    {
        auto layerTab = rootObject->getAllChild();

        for(auto layer = layerTab->begin(); layer != layerTab->end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Physic_Object ||
               (*layer)->getSecondType() == Object::Solid_Object ||
               (*layer)->getSecondType() == Object::Animation_Solid_Object)
            {
                auto objectTab = (*layer)->getAllChild();

                for(auto it = objectTab->begin(); it != objectTab->end(); it++)
                {
                    PhysicObject::Ptr physic_object = std::static_pointer_cast<PhysicObject>((*it));

                    std::string       category      = physic_object->getCategory();

                    PhysicObject::Rule rule;
                    rule.categoryBits = m_CatecotryBitTab[17];
                    rule.maskBits     = m_CatecotryBitTab[17];

                    physic_object->setCollisionRule(rule);
                }
            }
        }
    }

    void CollisionRule::noCollision(Object::Ptr rootObject)
    {
        auto layerTab = rootObject->getAllChild();

        for(auto layer = layerTab->begin(); layer != layerTab->end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Physic_Object ||
               (*layer)->getSecondType() == Object::Solid_Object ||
               (*layer)->getSecondType() == Object::Animation_Solid_Object)
            {
                auto objectTab = (*layer)->getAllChild();

                for(auto it = objectTab->begin(); it != objectTab->end(); it++)
                {
                    PhysicObject::Ptr physic_object = std::static_pointer_cast<PhysicObject>((*it));

                    PhysicObject::Rule rule;
                    rule.categoryBits = m_CatecotryBitTab[0];
                    rule.maskBits     = m_CatecotryBitTab[0];

                    physic_object->setCollisionRule(rule);
                }
            }
        }
    }

    PhysicObject::Rule CollisionRule::getCollisionData(std::string category)
    {
        PhysicObject::Rule rule;
        rule.categoryBits = m_CollisionRuleMap[category].first;
        rule.maskBits     = m_CollisionRuleMap[category].second;

        return rule;
    }
} // namespace nero
