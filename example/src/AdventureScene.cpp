////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Adventure Scene
#include "AdventureScene.h"
#include "ConstantPool.h"
//NERO
#include <Nero/scene/CollisionRule.h>
#include <Nero/model/FrontScreen.h>
#include <Nero/object/TextObject.h>
#include <Nero/model/JointProperty.h>
#include <Nero/model/PrismaticJoint.h>
////////////////////////////////////////////////////////////
namespace ng
{
    ////////////////////////////////////////////////////////////
	AdventureScene::AdventureScene(nero::Scene::Context context): nero::Scene(context)
		,mPlayer()
	    ,mCoinCount(0)
	    ,mStarCount(0)
	    ,mPlatform(nullptr)
	{
		//Empty
	}

	////////////////////////////////////////////////////////////
	AdventureScene::~AdventureScene()
	{
	    //Empty
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::checkSceneObject()
    {
        /*
            --> This method let you check the existence of all objects in your Scene
            --> This method is called automatically when the Scene is render
            --> It's the First method in your Scene to be called
        */

    	log("checking Scene Objects ...");

    	log("checking World Objects ...");
	    getObjectManager()->checkWorldObject(
	    {
	      	ObjectPool.player,
        	ObjectPool.player_body,
	    });

	    log("checking Screen Objects ...");
	    getObjectManager()->checkScreenObject
	    (
	        ScreenPool.start_screen,
	        {
                ButtonPool.startButton,
                ButtonPool.quitButton
            }
	    );

	    log("Scene Objects checking completed");
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f AdventureScene::getSceneResolution()
    {
        log("Setting up Resolution ...");

	    return sf::Vector2f(800.f, 600.f);
    }

    ////////////////////////////////////////////////////////////
	void AdventureScene::init()
	{
	     /*
            --> This method let you build the initial state of your Scene
            --> This method is called automatically when the Scene is render
            --> It's the Second method in your Scene to be called
        */

	    log("Initializing Nero Adventure Scene v0.2");

	    log("Setting up Sound and Music Volume ...");
	    getSoundManager()->setSoundVolume(30.f);
	    getSoundManager()->setMusicVolume(30.f);

	    log("Setting up Scene Screens ...");
	    setupSceneScreens();

	    log("Setting up Collision rules ...");
	    setupSceneCollisions();

	    log("Setting up the Player ...");
	    setupPlayer();

	    log("Setting up the platform ...");
        setupPlatform();

	    log("Setting up Scene settings ...");
	    getSceneSetting().drawShapes = false;
	    getSceneSetting().drawJoints = false;
	}

	void AdventureScene::setupSceneScreens()
	{
	    //Only the Render Engine support Screens Management
	    //If the Scene is not rendered within the Render Engine we can skip that part
	    if(!isRenderEngine())
            return;

        //Play a music when the Game start
	    getSoundManager()->playMusic(MusicPool.game_menu);

	    //We add the Screens we are going to use on the Screen_Stack
	    log("Pushing Scene Screens");
	    pushScreen(ScreenPool.game_screen);
	    pushScreen(ScreenPool.pause_screen);
	    pushScreen(ScreenPool.start_screen);
	    pushScreen(ScreenPool.options_screen);

	    //The Game begin by the Game Menu (Start Screen)
	    //We hide the Game World and display the Game Menu
	    log("Activating the Game Menu ...");
	    hideWorld();
	    showScreen(ScreenPool.start_screen);
	    setCanvasColor(getScreenCanvasColor(ScreenPool.start_screen));

        //There are many Button_Object on our Screens. We retrieve them and configure them
	    log("Retrieving Button Objects ...");
	    auto startButton        = getObjectManager()->findScreenUIObject(ScreenPool.start_screen, ButtonPool.startButton);
	    auto quitButton         = getObjectManager()->findScreenUIObject(ScreenPool.start_screen, ButtonPool.quitButton);
	    auto resumeButton       = getObjectManager()->findScreenUIObject(ScreenPool.pause_screen, ButtonPool.resumeButton);
	    auto optionsButton      = getObjectManager()->findScreenUIObject(ScreenPool.pause_screen, ButtonPool.optionsButton);
	    auto startMenuButton    = getObjectManager()->findScreenUIObject(ScreenPool.pause_screen, ButtonPool.startMenuButton);
	    auto closeOptionsButton = getObjectManager()->findScreenUIObject(ScreenPool.options_screen, ButtonPool.closeOptionsButton);
	    auto musicDownButton    = getObjectManager()->findScreenUIObject(ScreenPool.options_screen, ButtonPool.musicDownButton);
	    auto musicUpButton      = getObjectManager()->findScreenUIObject(ScreenPool.options_screen, ButtonPool.musicUpButton);
	    auto soundDownButton    = getObjectManager()->findScreenUIObject(ScreenPool.options_screen, ButtonPool.soundDownButton);
	    auto soundUpButton      = getObjectManager()->findScreenUIObject(ScreenPool.options_screen, ButtonPool.soundUpButton);

	    //We configure all Buttons to play a sound when the mouse comes over
	    log("Setting up all buttons on_mouse_enter callback ...");
	    nero::ButtonObject::Cast(startButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(quitButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(resumeButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(optionsButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(startMenuButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(closeOptionsButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(musicDownButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(musicUpButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(soundDownButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});
	    nero::ButtonObject::Cast(soundUpButton)->setOnMouseEnter([this](){getSoundManager()->playSound(SoundPool.button_over);});

	    //We configure all Buttons actions when they are clicked
	    log("Setting up all buttons on_mouse_click callback ...");

	    //Quit the Engine
	    nero::ButtonObject::Cast(quitButton)->setOnMouseClick([this](){quitScene();});

	    //Hide the Game Menu and Display the Game World
	    nero::ButtonObject::Cast(startButton)->setOnMouseClick([this]()
	    {
	        setCanvasColor(getCanvasColor());
	        showWorld();
            hideScreen(ScreenPool.start_screen);
	        showScreen(ScreenPool.game_screen);

	        getSoundManager()->playMusic(MusicPool.netherplace);
	    });

	    //Close Pause Menu and return to the Game World
	    nero::ButtonObject::Cast(resumeButton)->setOnMouseClick([this]()
	    {
	        hideScreen(ScreenPool.pause_screen);
	        resumeScene();
	        getSoundManager()->resumeMusic();
	    });

	    //Quit the Game and return to the Game Menu
	    nero::ButtonObject::Cast(startMenuButton)->setOnMouseClick([this]()
	    {
            getSoundManager()->playSound(SoundPool.button_click);
	        getSoundManager()->stopMusic();
	        hideScreen(ScreenPool.pause_screen);
	        resetScene();
	    });

	    //Display the Options Menu
	    nero::ButtonObject::Cast(optionsButton)->setOnMouseClick([this]()
	    {
            getSoundManager()->playSound(SoundPool.button_click);
	        hideScreen(ScreenPool.pause_screen);
	        showScreen(ScreenPool.options_screen);
	    });

	    //Close the Options Menu
	    nero::ButtonObject::Cast(closeOptionsButton)->setOnMouseClick([this]()
	    {
            getSoundManager()->playSound(SoundPool.button_click);
	        hideScreen(ScreenPool.options_screen);
	        showScreen(ScreenPool.pause_screen);
	    });

	    //Increase the Music Volume
	    nero::ButtonObject::Cast(musicUpButton)->setOnMouseClick([this]()
	    {
	        getSoundManager()->playSound(SoundPool.button_click);

	        float volume = getSoundManager()->increaseMusicVolume();

	        auto musicVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.musicVolumeText);
	        nero::TextObject::Cast(musicVolumeText)->setContent(nero::toString(volume));
	    });

	    //Decrease the Music Volume
	    nero::ButtonObject::Cast(musicDownButton)->setOnMouseClick([this]()
	    {
	        getSoundManager()->playSound(SoundPool.button_click);

	        float volume = getSoundManager()->decreaseMusicVolume();

	        auto musicVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.musicVolumeText);
	        nero::TextObject::Cast(musicVolumeText)->setContent(nero::toString(volume));
	    });

        //Increase the Sound Volume
	    nero::ButtonObject::Cast(soundUpButton)->setOnMouseClick([this]()
	    {
	        getSoundManager()->playSound(SoundPool.button_click);

	        float volume = getSoundManager()->increaseSoundVolume();

	        auto soundVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.soundVolumeText);
	        nero::TextObject::Cast(soundVolumeText)->setContent(nero::toString(volume));
	    });

        //Decrease the Sound Volume
	    nero::ButtonObject::Cast(soundDownButton)->setOnMouseClick([this]()
	    {
	        getSoundManager()->playSound(SoundPool.button_click);

	        float volume = getSoundManager()->decreaseSoundVolume();

	        auto soundVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.soundVolumeText);
	        nero::TextObject::Cast(soundVolumeText)->setContent(nero::toString(volume));
	    });

	    //Update the Music Text_Object in the Options Menu
	    auto musicVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.musicVolumeText);
	    nero::TextObject::Cast(musicVolumeText)->setContent(nero::toString(getSoundManager()->getMusicVolume()));

        //Update the Sound Text_Object in the Options Menu
        auto soundVolumeText = getObjectManager()->findScreenObject(ScreenPool.options_screen, TextPool.soundVolumeText);
	    nero::TextObject::Cast(soundVolumeText)->setContent(nero::toString(getSoundManager()->getSoundVolume()));


	   log("Scene Screens setting up completed");
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::setupSceneCollisions()
	{
	    //Update all Physic_Objects in the Scene according to our Collisions Scheme

	    nero::CollisionRule collisionRule;
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.ground);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.wall);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.platform);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.box);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.coin);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.key);
	    collisionRule.canCollide(CategoryPool.player, CategoryPool.star);
	    collisionRule.canCollide(CategoryPool.ground, CategoryPool.coin);
	    collisionRule.canCollide(CategoryPool.ground, CategoryPool.key);
	    collisionRule.canCollide(CategoryPool.ground, CategoryPool.star);
	    collisionRule.canCollide(CategoryPool.ground, CategoryPool.box);
	    collisionRule.canCollide(CategoryPool.wall, CategoryPool.box);
	    collisionRule.apply(getWorld());
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::setupPlayer()
	{
	    //Our Player in built with two Circles
	    // circle 1 : player_foot (lower part)
	    // circle 2 : player_body (upper part)

	    //retrieve the player object (solid_animation_object)
	    auto playerOjbect = getObjectManager()->findObject(ObjectPool.player);
	    //set up the player object (player_foot)
	    mPlayer.setObject(playerOjbect);
	    //let the camera follow the player
	    setCameraTarget(playerOjbect);

	    //create a weld joint between the player_foot and the player_body objects
	    nero::PhysicObject::Ptr playerFoot = nero::PhysicObject::Cast(playerOjbect);
	    nero::PhysicObject::Ptr playerBody = nero::PhysicObject::Cast(getObjectManager()->findObject(ObjectPool.player_body));

	    nero::PrismaticJointProperty property;
	    property.collideConnected = false;
	    property.enableLimit = true;
	    property.enableMotor = true;
	    property.localAxisA = sf::Vector2f(0.f, -1.f);
	    property.lowerTranslation = playerBody->getSize().y/2.f;
	    property.upperTranslation = 0.f;
	    getObjectManager()->createJoint(playerFoot, playerBody, property);
	}

	void AdventureScene::setupPlatform()
	{
	    auto platformObject = getObjectManager()->findObject(ObjectPool.platform_part_1);
	    mPlatform = nero::SimpleMovingObject::Ptr(new nero::SimpleMovingObject(nero::SimpleMovingObject::Right_Left, 70.f, 0.f, 320.f));
	    mPlatform->setObject(platformObject);

	    auto plaform_part_2 = getObjectManager()->moveObject(ObjectPool.platform_part_2);
	    auto plaform_part_3 = getObjectManager()->moveObject(ObjectPool.platform_part_3);
	    auto plaform_part_4 = getObjectManager()->moveObject(ObjectPool.platform_part_4);

	    sf::Vector2f offset = sf::Vector2f(plaform_part_2->getGlobalBounds().width, 0.f);
	    sf::Vector2f pos    = platformObject->getFirstChild()->getPosition();

	    plaform_part_2->setPosition(pos - offset);
	    platformObject->addChild(plaform_part_2);

	    plaform_part_3->setPosition(pos - 2.f*offset);
	    platformObject->addChild(plaform_part_3);

	    plaform_part_4->setPosition(pos - 3.f*offset);
	    platformObject->addChild(plaform_part_4);

	    getObjectManager()->addObject(mPlatform);
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::update(const sf::Time& timeStep)
	{
	    //Update parent
	    nero::Scene::update(timeStep);
	    //Update player
	    mPlayer.update(timeStep);
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
	{
	    //Handle parent inputs
	    nero::Scene::handleKeyboardInput(key, isPressed);

	    //Handle player inputs
	    mPlayer.handleKeyboardInput(key, isPressed);

	    //Handle Scene inputs
	    if(isPressed)
	    {
	        //Pause the Game : Display Pause Menu
	        if(key == sf::Keyboard::Space)
	        {
	            if(!isScenePause())
	            {
	                mPlayer.moveNone();
	                pauseScene();
	                showScreen(ScreenPool.pause_screen);
	                getSoundManager()->pauseMusic();
	            }
	        }
	    }
	}

	////////////////////////////////////////////////////////////
	void AdventureScene::handleCollisionContactBegin(nero::Collision collision)
	{
	    //When the Player touch the Ground, a Box, or a Platform we give him the possibility to jump again
	    if(collision.isCollising(CategoryPool.player, CategoryPool.ground) ||
	       collision.isCollising(CategoryPool.player, CategoryPool.box) ||
	       collision.isCollising(CategoryPool.player, CategoryPool.platform))
	    {
	        mPlayer.resetJump();

	        nero::PhysicObject::Cast(getObjectManager()->findObject(ObjectPool.player_body))->clearVelocity();
	    }

	    //The Player pickup some coins
	    if(collision.isCollising(CategoryPool.player, CategoryPool.coin))
	    {
	        //Update the number of coins the player has
	        mCoinCount++;
	        //Remove the coin from the Scene
	        getObjectManager()->removeObject(collision.getObject(CategoryPool.coin));
	        //Update the number of coins on the Game Screen
	        auto text = getObjectManager()->findScreenObject(ScreenPool.game_screen, TextPool.countCoinText);
	        nero::TextObject::Cast(text)->setContent(nero::toString(mCoinCount));
	        //disable this collision
            collision.setEnabled(false);
	    }

	    //The Player pickup some stars
	    if(collision.isCollising(CategoryPool.player, CategoryPool.star))
	    {
	        //Update the number of stars the player has
	        mStarCount++;
	        //Remove the start from the Scene
	        getObjectManager()->removeObject(collision.getObject(CategoryPool.star));
	        //Update the number of stars on the Game Screen
	        auto text = getObjectManager()->findScreenObject(ScreenPool.game_screen, TextPool.countStarText);
	        nero::TextObject::Cast(text)->setContent(nero::toString(mStarCount));
	        //disable this collision
            collision.setEnabled(false);
	    }
	}
}
