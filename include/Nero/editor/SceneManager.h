#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <map>
#include <functional>
#include <Nero/core/scene/Scene.h>
#include <Nero/core/luascene/LuaScene.h>

namespace nero
{
    class SceneManager
    {
        public:
            typedef std::unique_ptr<SceneManager> Ptr;

        public:
            SceneManager();

            void addSceneFacotry(const std::string& projectId, std::function<Scene::Ptr(Scene::Context)> factory);
            void addLuaSceneFacotry(const std::string& projectId, std::function<LuaScene::Ptr(Scene::Context)> factory);

        private:
            std::map<std::string, std::function<LuaScene::Ptr(Scene::Context)>>   m_LuaSceneFactoryMap;
            std::map<std::string, std::function<Scene::Ptr(Scene::Context)>>   m_SceneFactoryMap;
    };
}


#endif // SCENEMANAGER_H
