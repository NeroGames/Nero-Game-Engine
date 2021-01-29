////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef FONTHOLDER_H
#define FONTHOLDER_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/Font.hpp>
//JSON
#include <json/json.hpp>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class FontHolder
    {
        public:
                        FontHolder();
            void        load();

                        sf::Font& getFont(std::string name);
            const       sf::Font& getFont(std::string name) const;
            const       sf::Font& getDefaultFont()          const;

            const std::vector<std::string>& getFontTable()  const;

        private:
            void    addFont(std::string name, std::unique_ptr<sf::Font> font);

        private:
            std::map<std::string, std::unique_ptr<sf::Font>>     m_FontMap;
            std::vector<std::string>                             m_FontTable;
            nlohmann::json                                       m_Configuration;
    };
}
#endif // FONTHOLDER_H
