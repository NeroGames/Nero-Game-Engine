#include <Nero/editor/SceneManager.h>

namespace nero
{
    SceneManager::SceneManager()
    {

    }

    void SceneManager::addSceneFacotry(const std::string& projectId, std::function<Scene::Ptr(Scene::Context)> factory)
    {
        m_SceneFactoryMap[projectId] = factory;
    }
    void SceneManager::addLuaSceneFacotry(const std::string& projectId, std::function<LuaScene::Ptr(Scene::Context)> factory)
    {
        m_LuaSceneFactoryMap[projectId] = factory;
    }
}

