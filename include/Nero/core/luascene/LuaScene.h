#ifndef LUASCENE_H
#define LUASCENE_H

#include <Nero/core/scene/Scene.h>
#include <memory>
namespace  nero
{
    class LuaScene : public Scene
    {
    public:
        typedef std::shared_ptr<LuaScene> Ptr;

    public:
        LuaScene(Scene::Context context);
    };
}

#endif // LUASCENE_H
