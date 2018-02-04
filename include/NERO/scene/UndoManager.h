#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <json/json.hpp>

namespace nero
{
    class UndoManager
    {
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
