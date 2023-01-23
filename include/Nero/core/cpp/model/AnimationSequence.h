////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ANIMATIONSEQUENCE_H
#define ANIMATIONSEQUENCE_H
///////////////////////////HEADERS//////////////////////////
// SFML
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
// STD
#include <string>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class AnimationSequence
    {
      public:
        AnimationSequence();

        void              setFrameTable(const std::vector<sf::IntRect>& frameTable);
        void              setFrameRate(const float& frameRate);
        void              setLoop(bool flag);
        const sf::IntRect getNextFrame();
        float             getFrameRate() const;
        bool              getLoop() const;
        void              reset();
        void              lock();
        int               getFrameCount() const;

      private:
        std::vector<sf::IntRect> m_FrameTable;
        float                    m_FrameRate;
        bool                     m_Loop;
        int                      m_CurrentFrame;
    };
} // namespace nero
#endif // ANIMATIONSEQUENCE_H
