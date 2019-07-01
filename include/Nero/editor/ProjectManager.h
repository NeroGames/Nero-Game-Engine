#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <memory>

namespace  nero
{
    class ProjectManager
    {
        public:
            typedef std::unique_ptr<ProjectManager> Ptr;

        public:
            ProjectManager();
    };
}



#endif // PROJECTMANAGER_H
