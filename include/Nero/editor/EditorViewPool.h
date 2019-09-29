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

            const std::string PROJECT_MANAGER   = "Project Manager";
            const std::string CREATE_PROJECT    = "Create Project";
            const std::string OPEN_PROJECT      = "Open Project";
            const std::string RECENT_PROJECT    = "Recent Project";
            const std::string WORKSPACE         = "Workspace";
            const std::string ERROR_CREATING_WORKSPACE  = "Error Creating Workspace";
            const std::string ERROR_CREATING_PROJECT    = "Error Creating Project";


            const std::string Toolbar    = "Toolbar";

    };

    const _ViewPool ViewPool;
}

#endif // EDITORVIEWPOOL_H
