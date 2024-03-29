////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef FONTHOLDER_H
#define FONTHOLDER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
// SFML
#include <SFML/Graphics/Font.hpp>
// JSON
#include <json/json.hpp>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class FontHolder : public ResourceHolder
    {
      public: // utility
        typedef std::shared_ptr<FontHolder> Ptr;
        using ResourceHolder::loadDirectory;
        using ResourceHolder::loadFile;

      public:
        FontHolder();
        FontHolder(const Setting& setting);
        virtual ~FontHolder() override;

        virtual void                    loadDirectory() override;
        virtual bool                    loadFile(const std::string& file) override;
        virtual void                    destroy() override;
        virtual void                    clear() override;

        sf::Font&                       getFont(std::string name);
        const sf::Font&                 getFont(std::string name) const;
        const sf::Font&                 getDefaultFont() const;

        const std::vector<std::string>& getFontTable() const;

      private:
        bool addFont(std::string name, std::unique_ptr<sf::Font> font);

      private:
        std::map<std::string, std::unique_ptr<sf::Font>> m_FontMap;
        std::vector<std::string>                         m_FontTable;
    };
} // namespace nero
#endif // FONTHOLDER_H
