////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PLAYER_H
#define PLAYER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/gamelib.h>
#include <Nero/object/PhysicObject.h>
#include <Nero/object/AnimationObject.h>
//BOX2D
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Joints/b2WeldJoint.h>
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
////////////////////////////////////////////////////////////
namespace ng
{
	class Player : public nero::PhysicActionObject
	{
	    private:
	        enum                    PlayerFace{Left, Right};

	    public:
	                                Player();
	        virtual                ~Player();

	        void                    update(const sf::Time& timeStep);
	        void                    setObject(Object::Ptr object);
	        void                    handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
	        //Player Actions
	        void                    moveLeft();
	        void                    moveRight();
	        void                    runLeft();
	        void                    runRight();
	        void                    moveNone();
	        void                    runNone();
	        void                    run();
	        void                    jump();
	        void                    resetJump();

	    private:
	        float                           mMoveSpeed;
	        float                           mRunSpeed;
	        float                           mJumpForce;
	        bool                            mIsMovingLeft;
	        bool                            mIsMovingRight;
	        bool                            mIsRunningLeft;
	        bool                            mIsRunningRight;
	        bool                            mIsJumping;
	        bool                            mIsRunning;
	        nero::PhysicObject::Ptr         mPhysic;
	        nero::AnimationObject::Ptr      mAnimation;
            PlayerFace                      mFace;
	};
}
#endif // PLAYER_H
