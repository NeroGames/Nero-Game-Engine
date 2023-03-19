////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/Setting.h>
// Std
#include <string>
#include <unordered_set>
/////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceHolder
    {
      public:
        ResourceHolder();
        ResourceHolder(const Setting& setting);
        virtual ~ResourceHolder();

      public:
        virtual void loadDirectory()                   = 0;
        virtual bool loadFile(const std::string& file) = 0;
        virtual void destroy()                         = 0;
        virtual void clear();

      public:
        void                           setSetting(const Setting& setting);
        void                           loadDirectory(const std::string& directory);
        const std::vector<std::string> loadFile(const std::vector<std::string> fileTable);
        void addDirectory(const std::string& directory, bool autoSelect = false);
        void selectDirectory(const std::string& directory);
        void reloadDirectory(const std::string& directory);

      protected:
        Setting                         m_Setting;
        std::string                     m_SelectedDirectory;
        std::unordered_set<std::string> m_DirectoryTable;
    };

} // namespace nero

#endif // RESOURCEHOLDER_H
