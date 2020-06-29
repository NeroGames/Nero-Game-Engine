////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ADVENTURESCENE_H
#define ADVENTURESCENE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/Scene.h>
#include <Nero/gamelib.h>
//Adventure Scene
#include "Player.h"
////////////////////////////////////////////////////////////
namespace ng
{
	class AdventureScene : public nero::Scene
    {
        public:
                                AdventureScene(nero::Scene::Context context);
            virtual            ~AdventureScene();

            void                update(const sf::Time& timeStep);


        protected:
            void                init();
            void                checkSceneObject();
            sf::Vector2f        getSceneResolution();
            void                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                handleCollisionContactBegin(nero::Collision collision);


        private:
            //Startup Screen
            void                setupSceneScreens();
            void                setupSceneCollisions();
            void                setupPlayer();
            void                setupPlatform();

        private:
            Player                          mPlayer;
            int                             mCoinCount;
            int                             mStarCount;
            nero::SimpleMovingObject::Ptr   mPlatform;
    };
}
#endif // ADVENTURESCENE_H
