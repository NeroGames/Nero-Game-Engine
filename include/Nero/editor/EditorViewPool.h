#ifndef EDITORVIEWPOOL_H
#define EDITORVIEWPOOL_H

#include <string>

namespace  nero
{
    struct _ViewPool
    {
            const std::string SCENE_SCREEN  = "Screen   ";
            const std::string SCENE_CHUNCK  = "Chunck   ";
            const std::string UTILITY       = "Utility  ";
            const std::string SCENE         = "Scene     ";
    };

    const _ViewPool ViewPool;
}

#endif // EDITORVIEWPOOL_H
