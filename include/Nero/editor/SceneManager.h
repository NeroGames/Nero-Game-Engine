#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>

namespace nero
{
    class SceneManager
    {
        public:
            typedef std::unique_ptr<SceneManager> Ptr;

        public:
            SceneManager();
    };
}


#endif // SCENEMANAGER_H
