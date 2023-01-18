////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef NOGAMEFOUND_H
#define NOGAMEFOUND_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/Scene.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class NoGameFound : public Scene {
      public: // Utility
        typedef std::shared_ptr<NoGameFound> Ptr;

      public:
        // Constructor & Destructor
        NoGameFound(Scene::Context context);
        virtual ~NoGameFound() override;
        // Main Methods
        virtual void handleEvent(const sf::Event& event) override;
        virtual void update(const sf::Time& timeStep) override;
        virtual void render() override;
    };
} // namespace nero

#endif // NOGAMEFOUND_H
