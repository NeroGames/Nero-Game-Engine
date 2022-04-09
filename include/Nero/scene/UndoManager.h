////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
/////////////////////////////////////////////////////////////
#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H
///////////////////////////HEADERS//////////////////////////
//JSON
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

            void                    add(nlohmann::json scene);
            nlohmann::json          undo();
            nlohmann::json          redo();

        private:
            std::vector<nlohmann::json>         m_UndoTab;
            int                                 m_UndoState;
    };

}
#endif // UNDOMANAGER_H
