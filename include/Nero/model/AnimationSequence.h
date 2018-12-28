////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ANIMATIONSEQUENCE_H
#define ANIMATIONSEQUENCE_H

#include <string>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

namespace nero
{
    class AnimationSequence
    {
        public:
                                            AnimationSequence();

            void                            setFrameTable(const std::vector<sf::IntRect>& frameTable);
            void                            setFrameRate(const float& frameRate);
            void                            setLoop(bool flag);
            const   sf::IntRect             getNextFrame();
            float                           getFrameRate() const;
            bool                            getLoop() const;
            void                            reset();
            void                            lock();
            int                             getFrameCount() const;


        private:
            std::vector<sf::IntRect>        m_FrameTable;
            float                           m_FrameRate;
            bool                            m_Loop;
            int                             m_CurrentFrame;
    };
}


#endif // ANIMATIONSEQUENCE_H
