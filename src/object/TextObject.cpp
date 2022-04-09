////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/TextObject.h>
////////////////////////////////////////////////////////////
namespace nero
{
    TextObject::TextObject() : Object()
        ,m_Text()
        ,m_Font("Sansation")
        ,m_Content("New Text")
        ,m_FontSize(20.f)
        ,m_LetterSpacing(2.f)
        ,m_LineSpacing(2.f)
        ,m_OutlineThickness(0.f)
        ,m_IsBold(false)
        ,m_IsItalic(false)
        ,m_IsUnderlined(false)
        ,m_IsStrikeThrough(false)
    {
        setFirstType(Object::Text_Object);
        setSecondType(Object::Text_Object);
    }

    void TextObject::setText(const sf::Text& text)
    {
        m_Text = text;
    }

    sf::Text& TextObject::getText()
    {
        return m_Text;
    }

    const sf::Text& TextObject::getText() const
    {
        return m_Text;
    }

    void TextObject::setContent(const sf::String& content)
    {
        m_Text.setString(content == "" ? "New Text" : content);
        m_Content = content;
    }

    void TextObject::setFont(const std::string& font)
    {
        m_Font = font;
    }

    void TextObject::setFontSize(float value)
    {
        m_Text.setCharacterSize(static_cast<int>(value));
        m_FontSize = value;
    }

    void TextObject::setLetterSpacing(float value)
    {
        //m_Text.setLetterSpacing(value);
        m_LetterSpacing = value;
    }

    void TextObject::setLineSpacing(float value)
    {
        //m_Text.setLineSpacing(value);
        m_LineSpacing = value;
    }

    void TextObject::setOutlineThickness(float value)
    {
        m_Text.setOutlineThickness(value);
        m_OutlineThickness = value;
    }

    void TextObject::setStyle(bool bold, bool italic, bool underLined, bool strikeThrough)
    {
        sf::Text::Style style = sf::Text::Regular;

        if(bold)
            style = static_cast<sf::Text::Style>(style | sf::Text::Bold);
        if(italic)
            style = static_cast<sf::Text::Style>(style | sf::Text::Italic);
        if(underLined)
            style = static_cast<sf::Text::Style>(style | sf::Text::Underlined);
        if(strikeThrough)
            style = static_cast<sf::Text::Style>(style | sf::Text::StrikeThrough);

        setStyle(style);
        setBold(bold);
        setItalic(italic);
        setUnderlined(underLined);
        setStrikeThrough(strikeThrough);
    }

    void TextObject::setStyle(sf::Text::Style style)
    {
        m_Text.setStyle(style);
    }

    void TextObject::setBold(bool flag)
    {
        m_IsBold = flag;
    }

    void TextObject::setItalic(bool flag)
    {
        m_IsItalic = flag;
    }

    void TextObject::setUnderlined(bool flag)
    {
        m_IsUnderlined = flag;
    }

    void TextObject::setStrikeThrough(bool flag)
    {
        m_IsStrikeThrough = flag;
    }

    std::string TextObject::getContent()
    {
        return m_Content;
    }

    std::string TextObject::getFont()
    {
        return m_Font;
    }

    float TextObject::getFontSize()
    {
        return m_FontSize;
    }

    float TextObject::getLetterSpacing()
    {
        return m_LetterSpacing;
    }

    float TextObject::getLineSpacing()
    {
        return m_LineSpacing;
    }

    float TextObject::getOutlineThickness()
    {
        return m_OutlineThickness;
    }

    bool TextObject::isBold()
    {
        return m_IsBold;
    }

    bool TextObject::isItalic()
    {
        return m_IsItalic;
    }

    bool TextObject::isUnderlined()
    {
        return m_IsUnderlined;
    }

    bool TextObject::isStrikeThrough()
    {
        return m_IsStrikeThrough;
    }

    void TextObject::setColor(const sf::Color& color)
    {
        m_Text.setFillColor(color);
    }

    const sf::Color& TextObject::getColor() const
    {
        return m_Text.getFillColor();
    }

    void TextObject::setOutlineColor(const sf::Color& color)
    {
        return m_Text.setOutlineColor(color);
    }

    const sf::Color& TextObject::getOutlineColor() const
    {
        return m_Text.getOutlineColor();
    }

    void TextObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Text, states);
    }

    sf::FloatRect TextObject::getGlobalBounds() const
    {
        return getTransform().transformRect(m_Text.getGlobalBounds());
    }

    TextObject::Ptr TextObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Text_Object)
            return nullptr;

        return  std::static_pointer_cast<TextObject>(object);
    }

    Object::Ptr TextObject::clone(sf::Vector2f& position) const
    {
        TextObject::Ptr text_object = Cast(clone());
        text_object->move(position);
        text_object->setId(-1);

        return text_object;
    }

    Object::Ptr TextObject::clone() const
    {
        TextObject::Ptr text_object(new TextObject());
        Object::clone<TextObject::Ptr>(text_object);

        text_object->setPosition(getPosition());
        text_object->setRotation(getRotation());
        text_object->setScale(getScale());
        text_object->setText(m_Text);
        text_object->setFont(m_Font);
        text_object->setContent(m_Content);
        text_object->setFontSize(m_FontSize);
        text_object->setLetterSpacing(m_LetterSpacing);
        text_object->setLineSpacing(m_LineSpacing);
        text_object->setOutlineThickness(m_OutlineThickness);
        text_object->setBold(m_IsBold);
        text_object->setItalic(m_IsItalic);
        text_object->setUnderlined(m_IsUnderlined);
        text_object->setStrikeThrough(m_IsStrikeThrough);

        return text_object;
    }

    nlohmann::json TextObject::toJson() const
    {
        nlohmann::json textJson;

        textJson = Object::toJson();

        textJson["rotation"]                    = getRotation();
        textJson["position"]                    = {{"x", getPosition().x}, {"y", getPosition().y}};
        textJson["scale"]                       = {{"x", getScale().x}, {"y", getScale().y}};
        textJson["fill_color"]                  = {{"r", getColor().r}, {"g", getColor().g}, {"b", getColor().b}, {"a", getColor().a}};
        textJson["outline_color"]               = {{"r", getOutlineColor().r}, {"g", getOutlineColor().g}, {"b", getOutlineColor().b}, {"a", getOutlineColor().a}};
        textJson["font"]                        = m_Font;
        textJson["content"]                     = m_Content;
        textJson["font_size"]                   = m_FontSize;
        textJson["letter_spacing"]              = m_LetterSpacing;
        textJson["line_spacing"]                = m_LineSpacing;
        textJson["outline_thickness"]           = m_OutlineThickness;
        textJson["style"]["bold"]               = m_IsBold;
        textJson["style"]["italic"]             = m_IsItalic;
        textJson["style"]["underlined"]         = m_IsUnderlined;
        textJson["style"]["strike_through"]     = m_IsStrikeThrough;

        return textJson;
    }
}


