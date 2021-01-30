////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/Object.h>
//SFML
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
/////////////////////////////////////////////////////////////
namespace nero
{
    class TextObject : public Object
    {
        public:
            typedef std::shared_ptr<TextObject>     Ptr;
            static Ptr                              Cast(Object::Ptr object);

                                            TextObject();
            void                            setText(const sf::Text& text);
            sf::Text&                       getText();
            const sf::Text&                 getText() const;

            void                            setContent(const sf::String& content);
            void                            setFont(const std::string& font);
            void                            setFontSize(float value);
            void                            setLetterSpacing(float value);
            void                            setLineSpacing(float value);
            void                            setOutlineThickness(float value);
            void                            setStyle(sf::Text::Style style);
            void                            setStyle(bool bold, bool italic, bool underLined, bool strikeThrough);
            std::string                     getContent();
            std::string                     getFont();
            float                           getFontSize();
            float                           getLetterSpacing();
            float                           getLineSpacing();
            float                           getOutlineThickness();
            bool                            isBold();
            bool                            isItalic();
            bool                            isUnderlined();
            bool                            isStrikeThrough();

            void                            setOutlineColor(const sf::Color& color);
            const sf::Color&                getOutlineColor()               const;

            virtual sf::FloatRect           getGlobalBounds()               const;
            virtual void                    setColor(const sf::Color& color);
            virtual const sf::Color&        getColor()                      const;
            virtual Object::Ptr             clone(sf::Vector2f& position)   const;
            virtual Object::Ptr             clone()                         const;
            virtual nlohmann::json          toJson()                        const;

        private:
            virtual void	                drawObject(sf::RenderTarget& target, sf::RenderStates states) const;

            void                            setBold(bool flag);
            void                            setItalic(bool flag);
            void                            setUnderlined(bool flag);
            void                            setStrikeThrough(bool flag);

        private:
            sf::Text                        m_Text;
            std::string                     m_Font;
            std::string                     m_Content;
            float                           m_FontSize;
            float                           m_LetterSpacing;
            float                           m_LineSpacing;
            float                           m_OutlineThickness;
            bool                            m_IsBold;
            bool                            m_IsItalic;
            bool                            m_IsUnderlined;
            bool                            m_IsStrikeThrough;
    };
}
#endif // TEXTOBJECT_H
