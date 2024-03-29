////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MUSICHOLDER_H
#define MUSICHOLDER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
// SFML
#include <SFML/Audio/Music.hpp>
// JSON
#include <json/json.hpp>
// Std
#include <string>
#include <map>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class MusicHolder : public ResourceHolder
    {
      public: // utility
        typedef std::shared_ptr<MusicHolder> Ptr;
        using ResourceHolder::loadDirectory;
        using ResourceHolder::loadFile;

      public:
        MusicHolder();
        MusicHolder(const Setting& setting);
        virtual ~MusicHolder() override;

        virtual void loadDirectory() override;
        virtual bool loadFile(const std::string& file) override;
        virtual void destroy() override;
        virtual void clear() override;

        void         load();
        sf::Music&   getMusic(std::string name);
        bool         addMusic(const std::string& name, std::unique_ptr<sf::Music> music);
        const std::vector<std::string>& getMusicTable() const;

      private:
        std::map<std::string, std::unique_ptr<sf::Music>> m_MusicMap;
        std::vector<std::string>                          m_MusicTable;
    };
} // namespace nero
#endif // MUSICHOLDER_H
