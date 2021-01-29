////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/Collision.h>
#include <Nero/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    Collision::Collision(b2Contact* contact, const b2Manifold* oldManifold, const b2ContactImpulse*  contactImpulse):
         m_Contact(contact)
        ,m_OldManifold(oldManifold)
        ,m_ContactImpulse(contactImpulse)
        ,m_ObjectA(nullptr)
        ,m_ObjectB(nullptr)
        ,m_WorldManifold(new b2WorldManifold())
    {
        m_Contact->GetWorldManifold(m_WorldManifold);
    }

    ////////////////////////////////////////////////////////////
    Collision::~Collision()
    {
        //dtor
    }

    ////////////////////////////////////////////////////////////
    void Collision::setObjectA(PhysicObject::Ptr objectA)
    {
        m_ObjectA = objectA;
    }

    ////////////////////////////////////////////////////////////
    void Collision::setObjectB(PhysicObject::Ptr objectB)
    {
        m_ObjectB = objectB;
    }

    ////////////////////////////////////////////////////////////
    float Collision::getFriction()
    {
        m_Contact->GetFriction();
    }

    ////////////////////////////////////////////////////////////
    float Collision::getRestition()
    {
        m_Contact->GetRestitution();
    }

    ////////////////////////////////////////////////////////////
    float Collision::getTangentSpeed()
    {
        m_Contact->GetTangentSpeed();
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isEnabled()
    {
        return m_Contact->IsEnabled();
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isTouching()
    {
        return m_Contact->IsTouching();
    }

    ////////////////////////////////////////////////////////////
    void Collision::resetFriction()
    {
        m_Contact->ResetFriction();
    }

    ////////////////////////////////////////////////////////////
    void Collision::resetRestitution()
    {
         m_Contact->ResetRestitution();
    }

    ////////////////////////////////////////////////////////////
    void Collision::setEnabled(bool flag)
    {
        m_Contact->SetEnabled(flag);
    }

    ////////////////////////////////////////////////////////////
    void Collision::setFriction(float friction)
    {
        m_Contact->SetFriction(friction);
    }

    ////////////////////////////////////////////////////////////
    void Collision::setRestitution(float restitution)
    {
        m_Contact->SetRestitution(restitution);
    }

    ////////////////////////////////////////////////////////////
    void Collision::setTangentSpeed(float speed)
    {
        m_Contact->SetTangentSpeed(speed);
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Collision::getLocalNormal()
    {
        return b2_to_sf(m_Contact->GetManifold()->localNormal, SCALE);
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Collision::getLocalPoint()
    {
        return b2_to_sf(m_Contact->GetManifold()->localPoint, SCALE);
    }

    ////////////////////////////////////////////////////////////
    int Collision::getPointCount()
    {
        return m_Contact->GetManifold()->pointCount;
    }

    ////////////////////////////////////////////////////////////
    float Collision::getNormalImpulse(int index)
    {
        return m_Contact->GetManifold()->points[index].normalImpulse;
    }

    ////////////////////////////////////////////////////////////
    float Collision::getTangentImpulse(int index)
    {
        return m_Contact->GetManifold()->points[index].tangentImpulse;
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Collision::getOldLocalNormal()
    {
        if(m_OldManifold)
            return b2_to_sf(m_OldManifold->localNormal, SCALE);

        return getLocalNormal();
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Collision::getOldLocalPoint()
    {
        if(m_OldManifold)
            return b2_to_sf(m_OldManifold->localPoint, SCALE);

        return getLocalPoint();
    }

    ////////////////////////////////////////////////////////////
    int Collision::getOldPointCount()
    {
        if(m_OldManifold)
            return m_OldManifold->pointCount;

        return getPointCount();
    }

    ////////////////////////////////////////////////////////////
    float Collision::getOldNormalImpulse(int index)
    {
        if(m_OldManifold)
            return m_OldManifold->points[index].normalImpulse;

        return getNormalImpulse(index);
    }

    ////////////////////////////////////////////////////////////
    float Collision::getOldTangentImpulse(int index)
    {
        if(m_OldManifold)
            return m_OldManifold->points[index].tangentImpulse;

        return getTangentImpulse(index);
    }

    ////////////////////////////////////////////////////////////
    int Collision::getImpulseCount()
    {
        if(m_ContactImpulse)
            return m_ContactImpulse->count;

        return 0;
    }

    ////////////////////////////////////////////////////////////
    std::vector<float> Collision::getNormalImpulses()
    {
        std::vector<float> normalTab;

        if(m_ContactImpulse)
        {
            for(int i=0; i<getImpulseCount(); i++)
                normalTab.push_back(m_ContactImpulse->normalImpulses[i]);
        }

        return normalTab;
    }

    ////////////////////////////////////////////////////////////
    std::vector<float> Collision::getTangentImpulses()
    {
        std::vector<float> tangentTab;

        if(m_ContactImpulse)
        {
            for(int i=0; i<getImpulseCount(); i++)
                tangentTab.push_back(m_ContactImpulse->tangentImpulses[i]);
        }

        return tangentTab;
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Collision::getNormal()
    {
        return b2_to_sf(m_WorldManifold->normal, SCALE);
    }

    ////////////////////////////////////////////////////////////
    std::vector<sf::Vector2f> Collision::getPoints()
    {
        std::vector<sf::Vector2f> pointTab;

        for(int i=0; i<getPointCount(); i++)
                pointTab.push_back(b2_to_sf(m_WorldManifold->points[i], SCALE));

        return pointTab;
    }

    ////////////////////////////////////////////////////////////
    std::vector<float> Collision::getSeparations()
    {
        std::vector<float> separations;

        for(int i=0; i<getPointCount(); i++)
            separations.push_back(m_WorldManifold->separations[i]);

        return separations;
    }

    ////////////////////////////////////////////////////////////
    PhysicObject::Ptr Collision::getObjectA()
    {
        return m_ObjectA;
    }

    ////////////////////////////////////////////////////////////
    PhysicObject::Ptr Collision::getObjectB()
    {
        return m_ObjectB;
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isColliding(sf::String catA)
    {
        return  getObjectA()->getCategory() == catA ||
                getObjectB()->getCategory() == catA;
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isColliding(sf::String catA, sf::String catB)
    {
        if(!getObjectA() || !getObjectB()) return false;

        return  (   getObjectA()->getCategory() == catA &&
                    getObjectB()->getCategory() == catB   ) ||
                (   getObjectA()->getCategory() == catB &&
                    getObjectB()->getCategory() == catA   );
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isObjectColliding(sf::String nameA)
    {
        return  getObjectA()->getName() == nameA ||
                getObjectB()->getName() == nameA;
    }

    ////////////////////////////////////////////////////////////
    bool Collision::isObjectColliding(sf::String nameA, sf::String nameB)
    {
        return  (   getObjectA()->getName() == nameA &&
                    getObjectB()->getName() == nameB   ) ||
                (   getObjectA()->getName() == nameB &&
                    getObjectB()->getName() == nameA   );
    }

    ////////////////////////////////////////////////////////////
    PhysicObject::Ptr Collision::getObject(sf::String indicator)
    {
        if(getObjectA()->getCategory() == indicator)
            return getObjectA();
        else if (getObjectB()->getCategory() == indicator)
            return getObjectB();
        else if(getObjectA()->getName() == indicator)
            return getObjectA();
        else if (getObjectB()->getName() == indicator)
            return getObjectB();

        return nullptr;
    }
}
////////////////////////////////////////////////////////////
