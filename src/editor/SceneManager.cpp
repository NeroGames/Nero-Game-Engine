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

    sf::RenderTexture& SceneManager::render(RenderContext renderContext)
    {
        if(renderContext.focus)
        {
            m_Camera.move(1.f, 0.f);
        }
        //update render context;
        m_RenderContext = renderContext;
        renderTexture.create(renderContext.canvas_size.x, renderContext.canvas_size.y);
        m_Camera.setSize(renderContext.canvas_size.x, renderContext.canvas_size.y);
        renderTexture.setView(m_Camera);
        renderTexture.clear(sf::Color::Black);


        for(sf::CircleShape circle : m_CircleTable)
        {
            renderTexture.draw(circle);
        }

        renderTexture.display();

        return renderTexture;
    }

    void SceneManager::addCircleShape(sf::Vector2f position)
    {
        if(!isMouseOnCanvas(m_RenderContext.mouse_position) || !m_RenderContext.focus)
        {
            return;
        }

        sf::CircleShape circle(50.f);
        circle.setFillColor(sf::Color::Green);
        circle.setOrigin(50.f, 50.f);

        auto new_pos = renderTexture.mapPixelToCoords(sf::Vector2i(m_RenderContext.mouse_position.x, m_RenderContext.mouse_position.y), m_Camera);


        circle.setPosition(new_pos);

        m_CircleTable.push_back(circle);

    }

    bool SceneManager::isMouseOnCanvas(sf::Vector2f position)
    {
        return (position.x >= 0.f && position.y >= 0.f
                && position.x <= m_RenderContext.canvas_size.x
                && position.y <= m_RenderContext.canvas_size.y);
    }

    void SceneManager::removeCircleShape(sf::Vector2f position)
    {
        if(!isMouseOnCanvas(m_RenderContext.mouse_position) || !m_RenderContext.focus)
        {
            return;
        }

        if(m_CircleTable.empty())
        {
            return;
        }

        m_CircleTable.erase(m_CircleTable.end()-1);
    }

}

