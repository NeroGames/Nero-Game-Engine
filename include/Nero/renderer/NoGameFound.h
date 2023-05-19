////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef NOGAMEFOUND_H
#define NOGAMEFOUND_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/GameScene.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class NoGameFound : public GameScene
    {
      public: // Utility
        typedef std::shared_ptr<NoGameFound> Ptr;

      public:
        // Constructor & Destructor
        NoGameFound(GameScene::Context context);
        virtual ~NoGameFound() override;
        // Main Methods
        virtual void handleEvent(const sf::Event& event) override;
        virtual void update(const sf::Time& timeStep) override;
        virtual void render() override;
    };
} // namespace nero

#endif // NOGAMEFOUND_H
