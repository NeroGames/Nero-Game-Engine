////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////
#ifndef GAMELIB_H_INCLUDED
#define GAMELIB_H_INCLUDED

#include <NERO/object/Action.h>
#include <NERO/object/PhysicActionObject.h>

//Header
namespace nero
{
    /////////////////////////////////////////////////////////////////////////////
    //Move Action
    class MoveAction : public PhysicAction
    {
        public:
            enum        Direction {Left, Right, Up, Down, NoneX, NoneY, None};
            float       velocity = 0.f;
            Direction   direction;

            inline void call(nero::PhysicObject::Ptr object, sf::Time time_step);
    };

    /////////////////////////////////////////////////////////////////////////////
    //Jump Action
    class JumpAction : public PhysicAction
    {
        public:
            float force = 0.f;

            inline void call(PhysicObject::Ptr object, sf::Time time_step);
    };

    /////////////////////////////////////////////////////////////////////////////
    //Dash Action
    class DashAction : public PhysicAction
    {
        public:
            float force = 0.f;

            inline void call(PhysicObject::Ptr object, sf::Time time_step);
    };

    /////////////////////////////////////////////////////////////////////////////
    //Moving Action
    class SimpleMovingObject : public PhysicActionObject
    {
        public:

            typedef std::shared_ptr<SimpleMovingObject>     Ptr;

            enum Direction  {Right_Left, UP_Down, None};
            enum Step       {One, Two};

            Direction   direction   = Direction::None;
            Step        step        = Step::One;
            float       velocity    = 0.f;
            float       maxOne      = 0.f;
            float       maxTwo      = 0.f;
            float       distance    = 0.f;

           	inline                     SimpleMovingObject(Direction direction, float velocity = 10.f, float maxOne = 100.f, float maxTwo = 100.f);
            inline void                setObject(Object::Ptr object);
            inline Object::Ptr         getObject();
            inline void                updateObject(sf::Time time_step);
    };


    /////////////////////////////////////////////////////////////////////////////
    //Getter collection
    inline MoveAction getMoveRightAction(float velocity);
    inline MoveAction getMoveLeftAction(float velocity);
    inline MoveAction getMoveUpAction(float velocity);
    inline MoveAction getMoveDownAction(float velocity);
    inline MoveAction getMoveNoneXAction();
    inline MoveAction getMoveNoneYAction();
    inline MoveAction getMoveNoneAction();
    //
    inline JumpAction getJumpAction(float force);
    //
    inline DashAction getDashRightAction(float force);
    inline DashAction getDashLeftAction(float force);
}


//Source
namespace nero
{
    /////////////////////////////////////////////////////////////////////////////
    //Move Action
    void MoveAction::call(nero::PhysicObject::Ptr object, sf::Time time_step)
    {
        switch(direction)
        {
            case Direction::Left:
            {
                object->setLinearVelocity(sf::Vector2f(-velocity, object->getLinearVelocity().y));
            }break;

            case Direction::Right:
            {
                object->setLinearVelocity(sf::Vector2f(velocity, object->getLinearVelocity().y));
            }break;

            case Direction::Up:
            {
                object->setLinearVelocity(sf::Vector2f(object->getLinearVelocity().x, -velocity));
            }break;

             case Direction::Down:
            {
                object->setLinearVelocity(sf::Vector2f(object->getLinearVelocity().x, velocity));
            }break;

            case Direction::NoneX:
            {
                object->setLinearVelocity(sf::Vector2f(0.f, object->getLinearVelocity().y));
            }break;

            case Direction::NoneY:
            {
                object->setLinearVelocity(sf::Vector2f(object->getLinearVelocity().x, 0.f));
            }break;

            case Direction::None:
            {
                object->setLinearVelocity(sf::Vector2f(0.f, 0.f));
            }break;
        }
    }

    MoveAction getMoveRightAction(float velocity)
    {
        MoveAction action;
        action.velocity = velocity;
        action.direction = MoveAction::Right;

        return action;
    }

    MoveAction getMoveLeftAction(float velocity)
    {
        MoveAction action;
        action.velocity = velocity;
        action.direction = MoveAction::Left;

        return action;
    }

    MoveAction getMoveUpAction(float velocity)
    {
        MoveAction action;
        action.velocity = velocity;
        action.direction = MoveAction::Up;

        return action;
    }

    MoveAction getMoveDownAction(float velocity)
    {
        MoveAction action;
        action.velocity = velocity;
        action.direction = MoveAction::Down;

        return action;
    }

    MoveAction getMoveNoneXAction()
    {
        MoveAction action;
        action.direction = MoveAction::NoneX;

        return action;
    }

    MoveAction getMoveNoneYAction()
    {
        MoveAction action;
        action.direction = MoveAction::NoneY;

        return action;
    }

    MoveAction getMoveNoneAction()
    {
        MoveAction action;
        action.direction = MoveAction::None;

        return action;
    }

    /////////////////////////////////////////////////////////////////////////////
    //Jump Action
    void JumpAction::call(PhysicObject::Ptr object, sf::Time time_step)
    {
        object->applyForce(sf::Vector2f(0, force), object->getWorldCenter(), true);
    }

    JumpAction getJumpAction(float force)
    {
        JumpAction action;
        action.force = -force * SCALE;

        return action;
    }

    /////////////////////////////////////////////////////////////////////////////
    //Dash Action
    void DashAction::call(PhysicObject::Ptr object, sf::Time time_step)
    {
        object->applyForce(sf::Vector2f(force, 0), object->getWorldCenter(), true);
    }

    DashAction getDashRightAction(float force)
    {
        DashAction action;
        action.force = force * SCALE;

        return action;
    }

    DashAction getDashLeftAction(float force)
    {
        DashAction action;
        action.force = -force * SCALE;

        return action;
    }

    /////////////////////////////////////////////////////////////////////////////
    //Moving Object
    SimpleMovingObject::SimpleMovingObject(Direction direction, float velocity, float maxOne, float maxTwo):
        direction(direction),
        velocity(velocity),
        maxOne(maxOne*SCALE),
        maxTwo(-maxTwo*SCALE)
    {
        registerAction<nero::MoveAction>("move_right", nero::getMoveRightAction(velocity));
        registerAction<nero::MoveAction>("move_left", nero::getMoveLeftAction(velocity));
        registerAction<nero::MoveAction>("move_up", nero::getMoveUpAction(velocity));
        registerAction<nero::MoveAction>("move_down", nero::getMoveDownAction(velocity));

        if(step == Step::One && maxOne == 0.f)
            step = Step::Two;
        else if(step == Step::Two && maxTwo == 0.f)
            step = Step::One;
    }

    void SimpleMovingObject::setObject(Object::Ptr object)
    {
        addChild(object);
        setSecondType(Object::Physic_Object);
        PhysicActionObject::setObject(object);
    }

    void SimpleMovingObject::updateObject(sf::Time time_step)
    {
        if(time_step.asSeconds() <= 0)
            return;

        switch(direction)
        {
            case Direction::Right_Left:
            {
                if(distance < maxOne && step == Step::One)
                {
                    callAction("move_right");
                    distance += velocity;

                    if(distance >= maxOne)
                        step = Step::Two;
                }
                else if (distance > maxTwo && step == Step::Two)
                {
                    callAction("move_left");
                    distance -= velocity;

                    if(distance <= maxTwo)
                        step = Step::One;
                }
            }break;

            case Direction::UP_Down:
            {
                if(distance < maxOne && step == Step::One)
                {
                    callAction("move_up");
                    distance += velocity;

                    if(distance >= maxOne)
                        step = Step::Two;
                }
                else if (distance > maxTwo && step == Step::Two)
                {
                    callAction("move_down");
                    distance -= velocity;

                    if(distance <= maxTwo)
                        step = Step::One;
                }
            }break;
        }
    }
}
#endif // GAMELIB_H_INCLUDED



