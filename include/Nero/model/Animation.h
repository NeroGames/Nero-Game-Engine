#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <Nero/model/AnimationSequence.h>

#include <map>
#include <vector>

namespace nero
{
    class Animation : public sf::Transformable, public sf::Drawable
    {
        public:
            typedef std::map<std::string, AnimationSequence> SequenceMap;

                                        Animation();

            void                        setSprite(sf::Sprite sprite);
            void                        setTexture(std::string texture);
            void                        addSequence(std::string name, AnimationSequence sequence);
            void                        setSequence(std::string name);
            //void                        playSequence(std::string name);
            //void                        playSequence(std::string name, bool hide);
            sf::FloatRect               getGlobalBounds() const;
            std::string                 getTexture() const;

            void                        update(sf::Time timeStep);

            void                       setColor(const sf::Color& color);
            const sf::Color&            getColor()                      const;

            const SequenceMap&          getSequenceMap() const;
            void                        setFrameRate(float frameRate);
            void                        setLoop(bool flag);

            void                        verticalFlip();
            void                        horizontalFlip();
            void                        play();
            void                        pause();


        private:
            virtual void			    draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            std::string                 m_Texture;
            std::string                 m_CurrentSequence;
            sf::Sprite                  m_Sprite;
            sf::Time                    m_ElapsedTime;
            SequenceMap                 m_SequenceMap;
            bool                        m_pause;
    };
}

#endif // ANIMATION_H
