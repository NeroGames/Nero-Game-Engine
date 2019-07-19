#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <memory>

namespace nero
{
    class UndoManager
    {
        public:
            typedef std::shared_ptr<UndoManager> Ptr;

        public:
            UndoManager();
    };
}



#endif // UNDOMANAGER_H
