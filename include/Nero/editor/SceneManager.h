#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <map>
#include <functional>
#include <Nero/core/scene/Scene.h>
#include <Nero/core/luascene/LuaScene.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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

            //sf::RenderTexture& render(RenderContext renderContext);
            void addCircleShape(sf::Vector2f position);
            void removeCircleShape(sf::Vector2f position);

        private:
            bool isMouseOnCanvas(sf::Vector2f position);



        private:
            std::map<std::string, std::function<LuaScene::Ptr(Scene::Context)>>   m_LuaSceneFactoryMap;
            std::map<std::string, std::function<Scene::Ptr(Scene::Context)>>   m_SceneFactoryMap;

            sf::RenderTexture renderTexture;
            //RenderContext m_RenderContext;

            std::vector<sf::CircleShape> m_CircleTable;
            sf::View            m_Camera;
    };
}


#endif // SCENEMANAGER_H
