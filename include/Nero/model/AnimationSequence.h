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

            void    setFrameTable(std::vector<sf::IntRect> frameTable);
            void    setFrameRate(const float& frameRate);
            void    setLoop(bool flag);
            int     getFrameCount() const;
            const   sf::IntRect  getNextFrame();
            float   getFrameRate() const;
            bool    getLoop() const;

        private:
            std::vector<sf::IntRect>        m_FrameTable;
            float                           m_FrameRate;
            bool                            m_Loop;
            int                             m_CurrentFrame;
    };
}


#endif // ANIMATIONSEQUENCE_H
