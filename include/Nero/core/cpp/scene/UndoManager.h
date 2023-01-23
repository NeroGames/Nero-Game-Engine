////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H
///////////////////////////HEADERS//////////////////////////
// JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class UndoManager
    {
      public:
        typedef std::shared_ptr<UndoManager> Ptr;

      public:
        UndoManager();

        void           add(nlohmann::json scene);
        nlohmann::json undo();
        nlohmann::json redo();

      private:
        std::vector<nlohmann::json> m_UndoTab;
        int                         m_UndoState;
    };

} // namespace nero
#endif // UNDOMANAGER_H
