////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCRIPTHOLDER_H
#define SCRIPTHOLDER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceHolder.h>
// JSON
#include <json/json.hpp>
// Std
#include <vector>
#include <map>
////////////////////////////////////////////////////////////
namespace nero
{
    class ScriptHolder : public ResourceHolder
    {
      public: // utility
        typedef std::shared_ptr<ScriptHolder> Ptr;
        using ResourceHolder::loadDirectory;
        using ResourceHolder::loadFile;

      public:
        ScriptHolder();
        void         load();
        virtual void clear() override;

      private:
        std::map<std::string, std::string> m_ScriptMap;
        std::vector<std::string>           m_ScriptTable;
        nlohmann::json                     m_Configuration;
    };
} // namespace nero
#endif // SCRIPTHOLDER_H
