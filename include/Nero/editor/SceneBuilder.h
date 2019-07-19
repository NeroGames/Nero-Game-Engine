#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <memory>

namespace nero
{
    class SceneBuilder
    {
        public:
            typedef std::shared_ptr<SceneBuilder> Ptr;

        public:
            SceneBuilder();
    };

}


#endif // SCENEBUILDER_H
