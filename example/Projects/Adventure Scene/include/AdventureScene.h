////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2019 sk-landry
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
                                AdventureScene(nero::Context context);
            virtual            ~AdventureScene();

            void                update(const sf::Time& timeStep);


        protected:
            void                init();
            void                checkSceneObject();
            void                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                handleCollisionContactBegin(nero::Collision collision);

        private:
            //Startup Screen
            void                setupSceneScreens();
            void                setupSceneCollisions();
            void                setupPlayer();

        private:
            Player              mPlayer;
            int                 mCoinCount;
            int                 mStarCount;
            nero::SimpleMovingObject::Ptr   mPlatform;
    };
}
#endif // ADVENTURESCENE_H
