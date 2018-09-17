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
        public:
            Animation();

            void            setSprite(sf::Sprite sprite);
            void            setTexture(std::string texture);
            void            addSequence(std::string name, AnimationSequence sequence);
            void            setSequence(std::string name);
            sf::FloatRect   getGlobalBounds() const;
            std::string     getTexture() const;

            void            update(sf::Time timeStep);

             void                    setColor(const sf::Color& color);
            const sf::Color&        getColor()                      const;

            const SequenceMap&            getSequenceMap() const;
            void                        setFrameRate(float frameRate);
            void                        setLoop(bool flag);




        private:
            virtual void			    draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            std::string             m_Texture;
            sf::Sprite              m_Sprite;
            sf::Time                m_ElapsedTime;
            std::string             m_CurrentSequence;

            SequenceMap             m_SequenceMap;
    };
}

#endif // ANIMATION_H
