////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Adventure Scene
#include "Player.h"
#include "ConstantPool.h"
//NERO
#include <Nero/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace ng
{
    ////////////////////////////////////////////////////////////
	Player::Player(): PhysicActionObject()
	    ,mMoveSpeed(150.f)
	    ,mRunSpeed(300.f)
	    ,mJumpForce(700.f)
	    ,mIsMovingLeft(false)
	    ,mIsMovingRight(false)
	    ,mIsRunning(false)
	    ,mIsJumping(false)
	    ,mPhysic(nullptr)
	    ,mAnimation(nullptr)
	    ,mFace(PlayerFace::Right)
	{
	    //We give our player some Actions
	    registerAction<nero::MoveAction>(PlayerActionPool.move_left, nero::getMoveLeftAction(mMoveSpeed));
	    registerAction<nero::MoveAction>(PlayerActionPool.move_right, nero::getMoveRightAction(mMoveSpeed));
	    registerAction<nero::MoveAction>(PlayerActionPool.run_left, nero::getMoveLeftAction(mRunSpeed));
	    registerAction<nero::MoveAction>(PlayerActionPool.run_right, nero::getMoveRightAction(mRunSpeed));
	    registerAction<nero::MoveAction>(PlayerActionPool.move_none, nero::getMoveNoneXAction());
	    registerAction<nero::JumpAction>(PlayerActionPool.jump, nero::getJumpAction(mJumpForce));
	}

	////////////////////////////////////////////////////////////
	Player::~Player()
	{
	    //Empty
	}

	////////////////////////////////////////////////////////////
	void Player::setObject(Object::Ptr object)
	{
	    nero::PhysicActionObject::setObject(object);

	    //Our player is a Animation Solid Object (Physic Object + Animation Object)
	    //We retrieve and hold the Physic part and the Animation part of our Player
	    mPhysic     = nero::PhysicObject::Cast(getObject());
	    mAnimation  = nero::AnimationObject::Cast(getObject()->getFirstChild());
	}

	////////////////////////////////////////////////////////////
	void Player::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
	    if(isPressed)
	    {
	        if(key == sf::Keyboard::Left)
	        {
	            moveLeft();
	        }
	        else if(key == sf::Keyboard::Right)
	        {
	            moveRight();
	        }

	        if(key == sf::Keyboard::X)
	        {
	            run();
	        }

	        if(key == sf::Keyboard::C)
	        {
	            jump();
	        }

	    }
	    else // not isPressed
	    {
	         if(key == sf::Keyboard::Left || key == sf::Keyboard::Right)
	         {
	             moveNone();
	         }
	         else if(key == sf::Keyboard::X)
	         {
	             runNone();
	         }
	    }

	}

	////////////////////////////////////////////////////////////
	void Player::update(const sf::Time& timeStep)
	{
	    //Update the parent Class
	    nero::PhysicActionObject::update(timeStep);

	    //Move Action
	    if(mIsMovingLeft)
	    {
	        if(!mIsJumping)
	        {
	            if(mIsRunning)
	            {
	                mAnimation->setSequence(PlayerSequencePool.run);
	            }
	            else
	            {
	                mAnimation->setSequence(PlayerSequencePool.walk);
	            }

	            if(mFace == PlayerFace::Right)
	            {
	                mAnimation->verticalFlip();
	                mFace = PlayerFace::Left;
	            }
	        }

	        if(mIsRunning)
	        {
	            callAction(PlayerActionPool.run_left);
	        }
	        else
	        {
	             callAction(PlayerActionPool.move_left);
	        }
	    }
	    else if(mIsMovingRight)
	    {
	        if(!mIsJumping)
	        {
	             if(mIsRunning)
	            {
	                mAnimation->setSequence(PlayerSequencePool.run);
	            }
	            else
	            {
	                mAnimation->setSequence(PlayerSequencePool.walk);
	            }

	            if(mFace == PlayerFace::Left)
	            {
	                mAnimation->verticalFlip();
	                mFace = PlayerFace::Right;
	            }
	        }

	        if(mIsRunning)
	        {
	            callAction(PlayerActionPool.run_right);
	        }
	        else
	        {
	             callAction(PlayerActionPool.move_right);
	        }
	    }
	}

	////////////////////////////////////////////////////////////
	void Player::moveLeft()
	{
	    mIsMovingLeft = true;
	}

	////////////////////////////////////////////////////////////
	void Player::moveRight()
	{
	    mIsMovingRight = true;
	}

	////////////////////////////////////////////////////////////
	void Player::run()
	{
	    mIsRunning = true;
	}

	////////////////////////////////////////////////////////////
	void Player::moveNone()
	{
	    if(!mIsJumping)
	    {
	        mAnimation->setSequence(PlayerSequencePool.idle);
	    }

	    mIsMovingLeft = false;
	    mIsMovingRight = false;

	    callAction(PlayerActionPool.move_none);
	}

	////////////////////////////////////////////////////////////
	void Player::runNone()
	{
	    if(!mIsJumping && (mIsMovingLeft || mIsMovingRight))
	    {
	        mAnimation->setSequence(PlayerSequencePool.walk);
	    }

	    mIsRunning = false;
	}

	////////////////////////////////////////////////////////////
	void Player::jump()
	{
	    if(!mIsJumping)
	    {
	        mAnimation->setSequence(PlayerSequencePool.jump);
	        mPhysic->clearVerticalVelocity();
	        callAction(PlayerActionPool.jump);
	        mIsJumping = true;
	    }
	}

	////////////////////////////////////////////////////////////
	void Player::resetJump()
	{
	    //The player is falling down
	    if(mPhysic->getLinearVelocity().y > 0.f)
	    {
	        mIsJumping = false;
	        mAnimation->setSequence(PlayerSequencePool.idle);
	        mPhysic->clearVelocity();
	    }
	}
}



