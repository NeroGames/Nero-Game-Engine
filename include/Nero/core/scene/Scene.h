#ifndef SCENE_H
#define SCENE_H

#include <memory>

namespace nero
{
    class Scene
    {
       public:
        typedef std::shared_ptr<Scene> Ptr;

        struct Context
        {

        };


    public:
        Scene(Context context);
    };
}



#endif // SCENE_H
