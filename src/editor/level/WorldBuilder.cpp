////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/level/WorldBuilder.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/editor/icon/LightIcon.h>
#include <Nero/core/cpp/model/CircleMesh.h>
#include <Nero/core/cpp/model/PolygonMesh.h>
// Std
#include <fstream>
#include <string>
////////////////////////////////////////////////////////////
namespace nero
{
    WorldBuilder::WorldBuilder()
        // Main
        : m_SelectionRect()
        , m_LastMousePosition(0.f, 0.f)
        , m_PanningSpeed(1.f)
        , m_RotationSpeed(0.5f)
        // Speed
        , m_ZoomingRatio(0.1f)
        , m_SelectedLayer(nullptr)
        , m_LayerCount(0)
        // Object
        , m_SelectedObject(nullptr)
        , m_ObjectCount(0)
        , m_PhysicsManager()
        , m_PhysicWorld(nullptr)
        , m_RenderContext(nullptr)
        , m_RightSelection(false)
        , m_ClickedObject(false)
    {
        m_MeshEditor = MeshEditor::Ptr(new MeshEditor());

        m_SelectionRect.setFillColor(sf::Color::Transparent);
        m_SelectionRect.setOutlineColor(sf::Color::Green);
        m_SelectionRect.setOutlineThickness(-3.f);

        m_UpdateUI = []()
        {
        };
        m_UpdateUndo = []()
        {
        };
        m_UpdateLog = [](const std::string&, int)
        {
        };
        m_UpdateLogIf = [](const std::string&, bool, int)
        {
        };

        // TODO
        m_LightTexture.loadFromFile("resource/editor/texture/bubble_light.png");
    }

    void WorldBuilder::init()
    {
        auto layer = addLayer();
        selectLayer(layer->getObjectId());
    }

    void WorldBuilder::handleEvent(const sf::Event& event)
    {
        switch(event.type)
        {
            // Keyboard
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

            // Mouse_button
            case sf::Event::MouseButtonPressed:
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

            // Mouse_Mouvement
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

    void WorldBuilder::update(const sf::Time& timeStep)
    {
        float32 b2TimeStep = m_SceneSetting.hz > 0.0f ? 1.0f / m_SceneSetting.hz : float32(0.0f);

        if(b2TimeStep > 0.f)
        {
            b2TimeStep =
                (b2TimeStep * timeStep.asSeconds()) / EngineConstant.TIME_PER_FRAME.asSeconds();
        }

        if(m_SelectedObject)
        {
            m_SelectionRect.setPosition(m_SelectedObject->getGlobalBounds().left,
                                        m_SelectedObject->getGlobalBounds().top);
            m_SelectionRect.setSize(sf::Vector2f(m_SelectedObject->getGlobalBounds().width,
                                                 m_SelectedObject->getGlobalBounds().height));
        }
        else
            m_SelectionRect.setSize(sf::Vector2f(0.f, 0.f));

        if(m_SelectedLayer)
            m_SelectedLayer->update(sf::seconds(b2TimeStep));
    }

    void WorldBuilder::render()
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->isVisible())
            {
                m_RenderTexture->draw(*(*it));
                m_RenderTexture->draw(m_SelectionRect);
            }
        }
    }

    void WorldBuilder::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        // Object
        if(isPressed && m_SelectedLayer && m_SelectedObject)
        {
            // Move
            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL())
                m_SelectedObject->move(sf::Vector2f(0.f, -m_PanningSpeed));

            if(key == sf::Keyboard::Numpad2 && keyboard::CTRL())
                m_SelectedObject->move(sf::Vector2f(0.f, m_PanningSpeed));

            if(key == sf::Keyboard::Numpad4 && keyboard::CTRL())
                m_SelectedObject->move(sf::Vector2f(-m_PanningSpeed, 0.f));

            if(key == sf::Keyboard::Numpad6 && keyboard::CTRL())
                m_SelectedObject->move(sf::Vector2f(m_PanningSpeed, 0.f));

            // Rotate
            if(key == sf::Keyboard::Numpad7 && keyboard::CTRL())
                m_SelectedObject->rotate(-m_RotationSpeed);

            else if(key == sf::Keyboard::Numpad9 && keyboard::CTRL())
                m_SelectedObject->rotate(m_RotationSpeed);

            else if(key == sf::Keyboard::Divide && keyboard::CTRL())
                m_SelectedObject->rotate(-0.1f);

            else if(key == sf::Keyboard::Multiply && keyboard::CTRL())
                m_SelectedObject->rotate(0.1f);

            // Zoom
            if(key == sf::Keyboard::Add && keyboard::CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f + m_ZoomingRatio, 1.f + m_ZoomingRatio));

            else if(key == sf::Keyboard::Subtract && keyboard::CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f - m_ZoomingRatio, 1.f - m_ZoomingRatio));

            // Flip
            if(key == sf::Keyboard::Numpad1 && keyboard::CTRL())
                m_SelectedObject->scale(sf::Vector2f(-1.f, 1.f));

            else if(key == sf::Keyboard::Numpad3 && keyboard::CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f, -1.f));

            // Unselected
            if(key == sf::Keyboard::Escape)
                m_SelectedObject = nullptr;

            // Delete
            if(key == sf::Keyboard::Delete)
                deleteObject(m_SelectedObject);

            // copy sprite
            if(key == sf::Keyboard::Numpad8 && keyboard::ALT())
                copyObject(Up);

            else if(key == sf::Keyboard::Numpad2 && keyboard::ALT())
                copyObject(Down);

            else if(key == sf::Keyboard::Numpad4 && keyboard::ALT())
                copyObject(Left);

            else if(key == sf::Keyboard::Numpad6 && keyboard::ALT())
                copyObject(Right);

            else if(key == sf::Keyboard::Numpad7 && keyboard::ALT())
                copyObject(Up_Left);

            else if(key == sf::Keyboard::Numpad9 && keyboard::ALT())
                copyObject(Up_Right);

            else if(key == sf::Keyboard::Numpad1 && keyboard::ALT())
                copyObject(Down_Left);

            else if(key == sf::Keyboard::Numpad3 && keyboard::ALT())
                copyObject(Down_Right);
        }

        // Layer
        if(isPressed && m_SelectedLayer)
        {
            sf::Vector2f pos = sf::Vector2f(0.f, 0.f);

            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL_ALT())
                pos = sf::Vector2f(0.f, -5.f);

            else if(key == sf::Keyboard::Numpad2 && keyboard::CTRL_ALT())
                pos = sf::Vector2f(0.f, 5.f);

            else if(key == sf::Keyboard::Numpad4 && keyboard::CTRL_ALT())
                pos = sf::Vector2f(-5.f, 0.f);

            else if(key == sf::Keyboard::Numpad6 && keyboard::CTRL_ALT())
                pos = sf::Vector2f(5.f, 0.f);

            if(pos != sf::Vector2f(0.f, 0.f))
            {
                auto children = m_SelectedLayer->getAllChild();
                for(auto it = children->begin(); it != children->end(); it++)
                {
                    (*it)->move(pos);
                }
            }
        }

        if(!isPressed)
        {
            if(key == sf::Keyboard::Numpad8 && keyboard::CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad2 && keyboard::CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad4 && keyboard::CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad6 && keyboard::CTRL())
                m_UpdateUndo();

            // Rotate
            if(key == sf::Keyboard::Numpad7 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad9 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Divide && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Multiply && keyboard::CTRL())
                m_UpdateUndo();

            // Zoom
            if(key == sf::Keyboard::Add && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Subtract && keyboard::CTRL())
                m_UpdateUndo();

            // Flip
            if(key == sf::Keyboard::Numpad1 && keyboard::CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad3 && keyboard::CTRL())
                m_UpdateUndo();

            // copy sprite
            if(key == sf::Keyboard::Numpad8 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad2 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad4 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad6 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad7 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad9 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad1 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad3 && keyboard::ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad8 && keyboard::CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad2 && keyboard::CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad4 && keyboard::CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad6 && keyboard::CTRL_ALT())
                m_UpdateUndo();
        }
    }

    void WorldBuilder::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                               const bool&                        isPressed)
    {
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        if(mouse.button == sf::Mouse::Left && isPressed && m_SelectedLayer &&
           m_SelectedLayer->isVisible())
        {
            auto selectedObject = findObject(m_SelectedLayer, world_pos);

            if(selectedObject && keyboard::ALT())
            {
                deleteObject(selectedObject);
            }

            m_LastMousePosition = world_pos;

            m_ClickedObject     = selectedObject ? true : false;

            if(selectedObject)
            {
                m_SelectedObject = selectedObject;
                m_RightSelection = false;
            }
        }

        if(mouse.button == sf::Mouse::Right && isPressed && m_SelectedLayer &&
           m_SelectedLayer->isVisible())
        {
            auto founded = findObject(m_SelectedLayer, world_pos);

            if(founded)
            {
                m_SelectedObject = founded;
                m_RightSelection = true;
            }

            if(m_SelectedObject)
                m_UpdateUI();
        }

        if(mouse.button == sf::Mouse::Left && !isPressed && m_SelectedLayer && m_SelectedObject)
        {
            // if(!m_RightSelection)
            //{
            m_SelectedObject = nullptr;
            m_UpdateUndo();
            //}
        }
    }

    void WorldBuilder::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f world_pos = m_RenderTexture->mapPixelToCoords(
            sf::Vector2i(m_RenderContext->mousePosition.x, m_RenderContext->mousePosition.y),
            m_RenderTexture->getView());

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_SelectedLayer &&
           m_SelectedLayer->isVisible() && m_SelectedObject)
        {
            if(m_ClickedObject)
            {
                sf::Vector2f diff = world_pos - m_LastMousePosition;

                if(!keyboard::CTRL_SHIFT_ALT())
                    m_SelectedObject->move(diff);

                else if(keyboard::SHIFT() && diff.y > 0.f)
                    m_SelectedObject->rotate(m_RotationSpeed * 2.f);

                else if(keyboard::SHIFT() && diff.y < 0.f)
                    m_SelectedObject->rotate(-m_RotationSpeed * 2.f);

                else if(keyboard::CTRL() && diff.y < 0.f)
                    m_SelectedObject->scale(
                        sf::Vector2f(1.f + m_ZoomingRatio, 1.f + m_ZoomingRatio));

                else if(keyboard::CTRL() && diff.y > 0.f)
                    m_SelectedObject->scale(
                        sf::Vector2f(1.f - m_ZoomingRatio, 1.f - m_ZoomingRatio));

                m_LastMousePosition = world_pos;
            }
        }
    }

    Object::Ptr WorldBuilder::findObject(Object::Ptr object, sf::Vector2f pos)
    {
        Object::Ptr result = nullptr;
        findObject(object, pos, result);

        return result;
    }

    void WorldBuilder::findObject(Object::Ptr object, sf::Vector2f pos, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getGlobalBounds().contains(pos))
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for(auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, pos, result);
        }
    }

    Object::Ptr WorldBuilder::findObject(Object::Ptr object, sf::String name)
    {
        Object::Ptr result = nullptr;
        findObject(object, name, result);

        return result;
    }

    void WorldBuilder::findObject(Object::Ptr object, sf::String name, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getName() == name)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for(auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, name, result);
        }
    }

    Object::Ptr WorldBuilder::findObject(Object::Ptr object, int id)
    {
        Object::Ptr result = nullptr;
        findObject(object, id, result);

        return result;
    }

    void WorldBuilder::findObject(Object::Ptr object, int id, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getId() == id)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for(auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, id, result);
        }
    }

    const LayerObject::Ptr WorldBuilder::addLayer()
    {
        int index = ++m_LayerCount;

        // m_UpdateLog("adding new Layer [Layer " + toString(index) + "]", nero::Info);

        if(m_SelectedLayer)
            m_SelectedLayer->setIsSelected(false);

        LayerObject::Ptr Layer_object(new LayerObject());
        Layer_object->setId(getNewId());
        Layer_object->setIsVisible(true);
        Layer_object->setIsSelected(true);
        Layer_object->setName("Layer " + toString(index));
        Layer_object->setOrder(0);

        m_LayerTable.push_back(Layer_object);

        m_SelectedLayer  = Layer_object;
        m_SelectedObject = nullptr;

        m_UpdateUndo();

        return m_LayerTable.back();
    }

    void WorldBuilder::deleteLayer(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id)
            {
                // m_UpdateLog("removing Layer [" + (*it)->getName() + "]", nero::Info);

                if(it != m_LayerTable.begin())
                    selectLayer((*(it - 1))->getId());
                else if(it == m_LayerTable.begin() && m_LayerTable.size() != 1)
                    selectLayer((*(it + 1))->getId());

                m_LayerTable.erase(it);

                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::moveLayerDown(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != m_LayerTable.begin())
            {
                // m_UpdateLog("moving Layer [" + (*it)->getName() + "] Down", nero::Info);

                std::iter_swap(it, it - 1);
                selectLayer((*(it - 1))->getId());

                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::moveLayerUp(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.end() - 1))
            {
                // m_UpdateLog("moving Layer [" + (*it)->getName() + "] Up", nero::Info);

                std::iter_swap(it, it + 1);
                selectLayer((*(it + 1))->getId());

                m_UpdateUndo();

                break;
            }
        }
    }

    bool WorldBuilder::mergeLayerUp(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.end() - 1))
            {
                if((*it)->getSecondType() == (*(it + 1))->getSecondType())
                {
                    // m_UpdateLog("merging Layer Up : Merging [" + (*it)->getName() + "] into [" +
                    // (*(it+1))->getName() + "]", nero::Info);

                    auto childTable = (*it)->getAllChild();

                    for(auto child : *childTable)
                        (*(it + 1))->addChild(child);

                    selectLayer((*(it + 1))->getId());

                    m_LayerTable.erase(it);

                    m_UpdateUndo();

                    return true;
                }

                break;
            }
        }

        return false;
    }

    bool WorldBuilder::mergeLayerDown(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.begin()))
            {
                if((*it)->getSecondType() == (*(it - 1))->getSecondType())
                {
                    // m_UpdateLog("merging Layer Down : Merging [" + (*it)->getName() + "] into ["
                    // + (*(it-1))->getName() + "]", nero::Info);

                    auto childTable = (*it)->getAllChild();

                    for(auto child : *childTable)
                        (*(it - 1))->addChild(child);

                    selectLayer((*(it - 1))->getId());

                    m_LayerTable.erase(it);

                    m_UpdateUndo();

                    return true;
                }

                break;
            }
        }

        return false;
    }

    void WorldBuilder::updateLayerVisibility(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id)
            {
                //(*it)->isVisible() ? m_UpdateLog("Hiding Layer [" + (*it)->getName() + "]",
                // Nero::Info) : m_UpdateLog("Showing Layer [" + (*it)->getName() + "]",
                // Nero::Info);

                (*it)->setIsVisible(!(*it)->isVisible());

                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::updateLayerName(int id, sf::String name)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id)
            {
                (*it)->setName(name);

                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::selectLayer(int id)
    {
        m_SelectedLayer->setIsSelected(false);
        m_SelectedLayer = nullptr;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); ++it)
        {
            if((*it)->getId() == id)
            {
                // m_UpdateLog("Selecting Layer [" + (*it)->getName() + "]", nero::Info);

                m_SelectedLayer = *it;
                m_SelectedLayer->setIsSelected(true);
                m_SelectedObject = nullptr;

                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::setSelectedLayer(LayerObject::Ptr layerObject)
    {
        // unselect
        m_SelectedLayer->setIsSelected(false);
        m_SelectedLayer = nullptr;

        // select
        m_SelectedLayer = layerObject;
        m_SelectedLayer->setIsSelected(true);
        m_SelectedObject = nullptr;
    }

    void WorldBuilder::setSelectedObject(Object::Ptr object)
    {
        m_SelectedObject = object;
    }

    void WorldBuilder::copyObject(const Position& position)
    {
        if(!m_SelectedLayer || !m_SelectedObject)
            return;

        sf::Vector2f pos;
        std::string  pos_string = "";

        switch(position)
        {
            case Up:
                pos        = sf::Vector2f(0.f, -m_SelectionRect.getSize().y);
                pos_string = "Up";
                break;
            case Down:
                pos        = sf::Vector2f(0.f, m_SelectionRect.getSize().y);
                pos_string = "Down";
                break;
            case Left:
                pos        = sf::Vector2f(-m_SelectionRect.getSize().x, 0.f);
                pos_string = "Left";
                break;
            case Right:
                pos        = sf::Vector2f(m_SelectionRect.getSize().x, 0.f);
                pos_string = "Right";
                break;
            case Up_Left:
                pos = sf::Vector2f(-m_SelectionRect.getSize().x, -m_SelectionRect.getSize().y);
                pos_string = "Up_Left";
                break;
            case Up_Right:
                pos = sf::Vector2f(m_SelectionRect.getSize().x, -m_SelectionRect.getSize().y);
                pos_string = "Up_Right";
                break;
            case Down_Left:
                pos = sf::Vector2f(-m_SelectionRect.getSize().x, m_SelectionRect.getSize().y);
                pos_string = "Down_Left";
                break;
            case Down_Right:
                pos        = sf::Vector2f(m_SelectionRect.getSize().x, m_SelectionRect.getSize().y);
                pos_string = "Down_Right";
                break;
        }

        // m_UpdateLog("copying Object [" + m_SelectedObject->getName() + "] " + pos_string,
        // Nero::Info);

        Object::Ptr object = m_SelectedObject->clone(pos);
        object->setId(getNewId());

        std::string object_name = StringPool.BLANK;

        switch(object->getSecondType())
        {
            case Object::Sprite_Object:
                object_name = "sprite " + toString(object->getId());
                break;

            case Object::Mesh_Object:
                object_name = "mesh " + toString(object->getId());
                break;

            case Object::Meshed_Object:
                object_name = "meshed sprite " + toString(object->getId());
                break;

            case Object::Animation_Object:
                object_name = "animation " + toString(object->getId());
                break;

            case Object::Animation_Meshed_Object:
                object_name = "meshed animation " + toString(object->getId());
                break;

            case Object::Text_Object:
                object_name = "sprite " + toString(object->getId());
                break;

            case Object::Button_Object:
                object_name = "button " + toString(object->getId());
                break;

            default:
                object_name = "object " + toString(object->getId());
                break;
        }

        object->setName(object_name);

        if(object->getSecondType() == Object::Mesh_Object)
        {
            PhysicsMeshObject::Ptr mesh_object = PhysicsMeshObject::Cast(object);
            mesh_object->getMesh()->setMeshId(mesh_object->getObjectId());
            m_MeshEditor->addMesh(mesh_object);
        }

        else if(object->getSecondType() == Object::Meshed_Object ||
                object->getSecondType() == Object::Animation_Meshed_Object)
        {
            sf::Vector2f posf         = -pos;
            Object::Ptr  child_object = m_SelectedObject->getFirstChild()->clone(posf);
            child_object->setId(getNewId());

            PhysicsMeshObject::Ptr mesh_object = PhysicsMeshObject::Cast(child_object);
            mesh_object->getMesh()->setMeshId(mesh_object->getObjectId());
            mesh_object->setIsSelectable(false);

            object->setIsUpdateable(true);
            object->addChild(child_object);

            m_MeshEditor->addMesh(mesh_object);
        }

        m_SelectedObject = object;
        m_SelectedLayer->addChild(m_SelectedObject);
    }

    void WorldBuilder::updateLayerColor(const sf::Color& color)
    {
        if(m_SelectedLayer->getSecondType() == Object::Mesh_Object)
            return;

        m_SelectedLayer->setColor(color);

        auto children = m_SelectedLayer->getAllChild();

        for(auto it = children->begin(); it != children->end(); it++)
        {
            (*it)->setColor(color);
        }
    }

    void WorldBuilder::updateAllLayerAlpha(int alpha)
    {

        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Sprite_Object ||
               (*layer)->getSecondType() == Object::Animation_Object ||
               (*layer)->getSecondType() == Object::Meshed_Object ||
               (*layer)->getSecondType() == Object::Animation_Meshed_Object)
            {
                auto children = (*layer)->getAllChild();

                for(auto it = children->begin(); it != children->end(); it++)
                {
                    sf::Color color = (*it)->getColor();
                    color.a         = alpha;

                    (*it)->setColor(color);
                }
            }
        }
    }

    void WorldBuilder::updateSpriteColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr &&
           (m_SelectedObject->getFirstType() == Object::Sprite_Object ||
            m_SelectedObject->getFirstType() == Object::Animation_Object))
            m_SelectedObject->setColor(color);
    }

    void WorldBuilder::updateTextColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && m_SelectedObject->getFirstType() == Object::Text_Object)
            TextObject::Cast(m_SelectedObject)->setColor(color);
    }

    void WorldBuilder::updateOutlineTextColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && m_SelectedObject->getFirstType() == Object::Text_Object)
            TextObject::Cast(m_SelectedObject)->setOutlineColor(color);
    }

    const LayerObject::Tab& WorldBuilder::getLayerTable() const
    {
        return m_LayerTable;
    }

    MeshEditor::Ptr WorldBuilder::getMeshEditor()
    {
        return m_MeshEditor;
    }

    bool WorldBuilder::addObject(Object::Type type, const sf::String& label, sf::Vector2f position)
    {
        Object::Ptr object = nullptr;

        switch(type)
        {
            case Object::Sprite_Object:
            {
                // m_UpdateLog("adding Sprite Object with Sprite [" + label + "]", nero::Info);

                sf::Sprite  sprite;
                sf::IntRect rect = m_ResourceManager->getTextureHolder()->getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->getTextureHolder()->getSpriteTexture(label));
                sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Sprite_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());
                std::string object_name = "sprite " + toString(sprite_object->getId());
                sprite_object->setName(object_name);

                object = sprite_object;
            }
            break;

            case Object::Mesh_Object:
            {
                PointMesh::Ptr mesh;

                if(label == "Line")
                    mesh = std::make_shared<PointMesh>(PointMesh::Shape::Line);
                else if(label == "Polygon")
                    mesh = std::make_shared<PolygonMesh>();
                else if(label == "Circle")
                    mesh = std::make_shared<CircleMesh>();

                mesh->setMeshId(getNewId());
                mesh->generateDefaultShape();

                PhysicsMeshObject::Ptr meshObject = std::make_shared<PhysicsMeshObject>();
                meshObject->setId(mesh->getMeshId());
                meshObject->setName("mesh " + toString(mesh->getMeshId()));
                meshObject->setMesh(mesh);
                meshObject->setPosition(position);
                meshObject->setSecondType(Object::Mesh_Object);
                sf::FloatRect globalBound = meshObject->getGlobalBounds();
                meshObject->setOrigin(globalBound.width / 2.f, globalBound.height / 2.f);

                m_MeshEditor->addMesh(meshObject);

                object = meshObject;

                /*if(!m_SelectedObject)
                {
                    // m_UpdateLog("adding Mesh Object of Type [" + label + "]", nero::Info);

                    PointMesh::Ptr mesh;

                    if(label == "Polygon")
                        mesh = Mesh(PointMesh::Shape::Polygon);
                    else if(label == "Circle")
                        mesh = Mesh(PointMesh::Shape::Circle);
                    else if(label == "Line")
                        mesh = Mesh(PointMesh::Shape::Line);

                    mesh.setMeshId(getNewId());

                    PhysicsMeshObject::Ptr mesh_object(new MeshObject());
                    mesh_object->setId(mesh.getMeshId());
                    std::string object_name = "mesh " + toString(mesh.getMeshId());
                    mesh_object->setName(object_name);
                    mesh_object->setMesh(mesh);
                    mesh_object->setPosition(position);
                    mesh_object->setSecondType(Object::Mesh_Object);
                    sf::FloatRect bound = mesh_object->getGlobalBounds();
                    mesh_object->setOrigin(bound.width / 2.f, bound.height / 2.f);

                    m_MeshEditor->addMesh(mesh_object);

                    object = mesh_object;
                }
                else if(m_SelectedObject &&
                        (m_SelectedObject->getSecondType() == Object::Meshed_Object ||
                         m_SelectedObject->getSecondType() == Object::Animation_Meshed_Object))
                {
                    // m_UpdateLog("changing Object [" + m_SelectedObject->getName() + "] Mesh Type
                    // to
                    // [" + label + "]", nero::Info);

                    m_MeshEditor->deleteMesh(m_SelectedObject->getFirstChild()->getId());
                    m_SelectedObject->removeFirstChild();

                    Mesh mesh;

                    if(label == "Polygon")
                        mesh = Mesh(PointMesh::Shape::Polygon);
                    else if(label == "Circle")
                        mesh = Mesh(PointMesh::Shape::Circle);
                    else if(label == "Line")
                        mesh = Mesh(PointMesh::Shape::Line);

                    mesh.setMeshId(getNewId());
                    std::shared_ptr<MeshObject> mesh_object(new MeshObject());
                    mesh_object->setId(mesh.getMeshId());
                    mesh_object->setMesh(mesh);
                    mesh_object->setIsSelectable(false);
                    mesh_object->setSecondType(Object::Mesh_Object);
                    mesh_object->setName(m_SelectedObject->getName());
                    mesh_object->setCategory(m_SelectedObject->getCategory());

                    m_MeshEditor->addMesh(mesh_object);

                    m_SelectedObject->addChild(mesh_object);

                    m_SelectedObject->update(EngineConstant.TIME_PER_FRAME);

                    nero_log("change animation mesh");
                    m_UpdateUndo();

                    return false;
                }
                else
                {
                    return false;
                }*/
            }
            break;

            case Object::Meshed_Object:
            {
                // m_UpdateLog("adding Meshed Sprite Object with Sprite [" + label + "]",
                // Nero::Info);

                // Sprite Object
                /*sf::Sprite  sprite;
                sf::IntRect rect = m_ResourceManager->getTextureHolder()->getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->getTextureHolder()->getSpriteTexture(label));
                sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Meshed_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());
                std::string object_name = "sprite " + toString(sprite_object->getId());
                sprite_object->setName(object_name);
                sprite_object->setIsUpdateable(true);

                // Mesh Object
                Mesh mesh = Mesh(PointMesh::Shape::Polygon);
                mesh.setMeshId(getNewId());
                PhysicsMeshObject::Ptr mesh_object(new MeshObject());
                mesh_object->setId(mesh.getMeshId());
                object_name = "mesh " + toString(mesh.getMeshId());
                mesh_object->setName(object_name);
                mesh_object->setMesh(mesh);
                mesh_object->setSecondType(Object::Mesh_Object);
                mesh_object->setIsSelectable(false);

                m_MeshEditor->addMesh(mesh_object);
                sprite_object->addChild(mesh_object);

                // update one time
                sprite_object->update(EngineConstant.TIME_PER_FRAME);

                object = sprite_object;*/
            }
            break;

            case Object::Animation_Object:
            {
                // m_UpdateLog("adding Animation Object with Animation [" + label + "]",
                // Nero::Info);

                Animation animation;
                auto sequenceMap = m_ResourceManager->getAnimationHolder()->getSequenceMap(label);

                for(auto it = sequenceMap.begin(); it != sequenceMap.end(); it++)
                {
                    AnimationSequence sequence;
                    sequence.setFrameTable(it->second);

                    animation.addSequence(it->first, sequence);
                }

                sf::Sprite  sprite;
                sf::IntRect rect =
                    m_ResourceManager->getAnimationHolder()->getAnimationBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->getAnimationHolder()->getTexture(label));
                sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

                animation.setSprite(sprite);
                animation.setTexture(label);
                animation.setSequence(
                    m_ResourceManager->getAnimationHolder()->getDefaultSequence(label));

                AnimationObject::Ptr animation_object(new AnimationObject());
                animation_object->setAnimation(animation);
                animation_object->setSecondType(Object::Animation_Object);
                animation_object->setPosition(position);
                animation_object->setId(getNewId());
                std::string object_name = "animation " + toString(animation_object->getId());
                animation_object->setName(object_name);

                object = animation_object;
            }
            break;

            case Object::Animation_Meshed_Object:
            {
                // m_UpdateLog("adding Meshed Animation Object with Animation [" + label + "]",
                // Nero::Info);

                /*Animation animation;
                auto sequenceMap = m_ResourceManager->getAnimationHolder()->getSequenceMap(label);

                for(auto it = sequenceMap.begin(); it != sequenceMap.end(); it++)
                {
                    AnimationSequence sequence;
                    sequence.setFrameTable(it->second);

                    animation.addSequence(it->first, sequence);
                }

                sf::Sprite  sprite;
                sf::IntRect rect =
                    m_ResourceManager->getAnimationHolder()->getAnimationBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->getAnimationHolder()->getTexture(label));
                sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

                animation.setSprite(sprite);
                animation.setTexture(label);
                animation.setSequence(
                    m_ResourceManager->getAnimationHolder()->getDefaultSequence(label));

                AnimationObject::Ptr animation_object(new AnimationObject());
                animation_object->setAnimation(animation);
                animation_object->setSecondType(Object::Animation_Meshed_Object);
                animation_object->setPosition(position);
                animation_object->setId(getNewId());
                std::string object_name = "animation " + toString(animation_object->getId());
                animation_object->setName(object_name);

                // Mesh Object
                Mesh mesh = Mesh(PointMesh::Shape::Polygon);
                mesh.setMeshId(getNewId());
                PhysicsMeshObject::Ptr mesh_object(new MeshObject());
                mesh_object->setId(mesh.getMeshId());
                object_name = "mesh " + toString(mesh.getMeshId());
                mesh_object->setName(object_name);
                mesh_object->setMesh(mesh);
                mesh_object->setSecondType(Object::Mesh_Object);
                mesh_object->setIsSelectable(false);

                m_MeshEditor->addMesh(mesh_object);
                animation_object->addChild(mesh_object);

                // update one time
                animation_object->update(EngineConstant.TIME_PER_FRAME);

                object = animation_object;*/
            }
            break;

            case Object::Text_Object:
            {
                // m_UpdateLog("adding Text Object with Text [" + label + "]", nero::Info);

                sf::Text text;
                text.setFont(m_ResourceManager->getFontHolder()->getFont(label));
                text.setString(label);
                text.setCharacterSize(20.f);
                text.setOrigin(text.getLocalBounds().width / 2.f,
                               text.getLocalBounds().height / 2.f);

                TextObject::Ptr text_object(new TextObject());
                text_object->setText(text);
                text_object->setPosition(position);
                text_object->setId(getNewId());
                text_object->setFont(label);
                std::string object_name = "text " + toString(text_object->getId());
                text_object->setName(object_name);
                text_object->setContent(text.getString());

                object = text_object;
            }
            break;

            case Object::Button_Object:
            {
                // m_UpdateLog("adding Button Object with Sprite [" + label + "]", nero::Info);

                sf::Sprite  sprite;
                sf::IntRect rect = m_ResourceManager->getTextureHolder()->getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->getTextureHolder()->getSpriteTexture(label));
                sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Button_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());
                std::string object_name = "button " + toString(sprite_object->getId());
                sprite_object->setName(object_name);

                sprite_object->setColor(sf::Color(255, 255, 255, 150));

                object = sprite_object;
            }
            break;

            case Object::Light_Object:
            {
                sf::Sprite sprite;
                sprite.setTexture(m_LightTexture);
                const auto textureSize = m_LightTexture.getSize();
                sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

                LightIcon::Ptr light_icon = std::make_shared<LightIcon>();

                light_icon->setSprite(sprite);
                light_icon->setLightmapName(label);
                light_icon->setSecondType(Object::Light_Object);
                light_icon->setPosition(position);
                light_icon->setId(getNewId());
                std::string object_name = "light_icon " + toString(light_icon->getId());
                light_icon->setName(object_name);

                object = light_icon;
            }
            break;
        }

        if(m_SelectedLayer->getSecondType() == type)
        {
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object)
                object->update(EngineConstant.TIME_PER_FRAME);
            m_UpdateUndo();
            return false;
        }
        else if(m_SelectedLayer->getSecondType() == Object::None || m_SelectedLayer->isEmpty())
        {
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object)
                object->update(EngineConstant.TIME_PER_FRAME);
            m_UpdateUndo();
            return true;
        }
        else if(m_SelectedLayer->getSecondType() != type &&
                m_SelectedLayer->getSecondType() != Object::None)
        {
            addLayer();
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object)
                object->update(EngineConstant.TIME_PER_FRAME);
            m_UpdateUndo();
            return true;
        }
    }

    void WorldBuilder::deleteObject(Object::Ptr object)
    {
        // m_UpdateLog("removing Object [" + object->getName() + "]", nero::Info);

        if(object->getFirstType() == Object::Mesh_Object)
        {
            m_MeshEditor->deleteMesh(object->getId());
        }

        else if(object->getSecondType() == Object::Meshed_Object)
        {
            m_MeshEditor->deleteMesh(object->getFirstChild()->getId());
        }

        m_SelectedLayer->removeChild(object);

        m_SelectedObject = nullptr;

        m_UpdateUndo();
    }

    void WorldBuilder::setMeshType(const sf::String& label)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        // m_UpdateLog("changing Mesh Type to : " + toString(label) + " Mesh", nero::Info);

        Mesh::Type type;

        if(label == "Static")
            type = Mesh::Type::Static;
        else if(label == "Kinematic")
            type = Mesh::Type::Kinematic;
        else if(label == "Dynamic")
            type = Mesh::Type::Dynamic;

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshType(type);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshFixedRotation(bool flag)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        // flag ?  m_UpdateLog("enabling fix rotation", nero::Info) :  //m_UpdateLog("disabling fix
        // rotation", nero::Info);

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFixedRotation(flag);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshSensor(bool flag)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        // flag ?  m_UpdateLog("enabling sensor mode", nero::Info) :  //m_UpdateLog("disabling
        // sensor mode", nero::Info);

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshSensor(flag);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshAllowSleep(bool flag)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        // flag ?  m_UpdateLog("enabling allow to sleeping", nero::Info) :  //m_UpdateLog("disabling
        // allow to sleeping", nero::Info);

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshAllowSleep(flag);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshDensity(float density)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshDensity(density);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshFriction(float friction)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFriction(friction);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshRestitution(float restitution)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshRestitution(restitution);

        m_UpdateUndo();*/
    }

    void WorldBuilder::setMeshGravityScale(float gravityScale)
    {
        /*if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object &&
           m_SelectedObject->getSecondType() != Object::Meshed_Object &&
           m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        PhysicsMeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshGravityScale(gravityScale);

        m_UpdateUndo();*/
    }

    Object::Ptr WorldBuilder::buildScene(std::shared_ptr<ltbl::LightSystem> lightManager,
                                         std::shared_ptr<b2World>           physicsWorld)
    {
        Object::Ptr chunkRoot = std::make_shared<Object>();
        m_PhysicsManager.setPhysicsWorld(physicsWorld);

        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            if(!(*layer)->isVisible())
                continue;

            switch((*layer)->getSecondType())
            {
                case Object::Sprite_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto        children     = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr sprite_object = (*it)->clone();
                        layer_object->addChild(sprite_object);
                    }

                    chunkRoot->addChild(layer_object);
                }
                break;

                case Object::Light_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto        children     = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        LightIcon::Ptr            lightIcon = LightIcon::Cast(*it);

                        // create the light object
                        ltbl::LightPointEmission* point_light =
                            lightManager->createLightPointEmission();
                        sf::Texture& light_map = m_ResourceManager->getLightmapHolder()->getTexture(
                            lightIcon->getLightmapName());

                        point_light->setTexture(light_map);
                        point_light->setOrigin(sf::Vector2f(light_map.getSize().x * 0.5f,
                                                            light_map.getSize().y * 0.5f));
                        point_light->setPosition(lightIcon->getPosition());
                        point_light->setScale(lightIcon->getScale());
                        point_light->setRotation(lightIcon->getRotation());
                        point_light->setColor(lightIcon->getColor());
                        point_light->setTurnedOn(lightIcon->getLightEnabled());

                        LightObject::Ptr light_object = std::make_shared<LightObject>();
                        light_object->setLight(point_light);
                        light_object->setCloneCallback(
                            [=]() -> ltbl::LightPointEmission*
                            {
                                ltbl::LightPointEmission* point_light =
                                    lightManager->createLightPointEmission();
                                sf::Texture& light_map =
                                    m_ResourceManager->getLightmapHolder()->getTexture(
                                        lightIcon->getLightmapName());
                                point_light->setOrigin(sf::Vector2f(light_map.getSize().x * 0.5f,
                                                                    light_map.getSize().y * 0.5f));
                                point_light->setScale(lightIcon->getScale());
                                point_light->setRotation(lightIcon->getRotation());
                                point_light->setColor(lightIcon->getColor());
                                point_light->setTexture(light_map);
                                point_light->setTurnedOn(lightIcon->getLightEnabled());

                                return point_light;
                            });

                        layer_object->addChild(light_object);
                    }

                    chunkRoot->addChild(layer_object);
                }
                break;

                case Object::Animation_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto        children     = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr animation_object = (*it)->clone();
                        layer_object->addChild(animation_object);
                    }

                    chunkRoot->addChild(layer_object);
                }
                break;

                case Object::Text_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto        children     = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr text_object = (*it)->clone();
                        layer_object->addChild(text_object);
                    }

                    chunkRoot->addChild(layer_object);
                }
                break;

                case Object::Mesh_Object:
                {
                    Object::Ptr layerObject = (*layer)->clone();
                    layerObject->setSecondType(Object::Physic_Object);

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        // convert into MeshObject
                        PhysicsMeshObject::Ptr meshObject = PhysicsMeshObject::Cast(*it);

                        if(!meshObject->getMesh()->meshValid())
                            continue;

                        PhysicsObject::Ptr physicObject =
                            m_PhysicsManager.createObject(meshObject->getMesh(),
                                                          meshObject->getPhysicsPoperty());
                        physicObject->setName(meshObject->getName());
                        physicObject->setCategory(meshObject->getCategory());
                        physicObject->setId(meshObject->getObjectId());
                        physicObject->setUserData((void*)physicObject->getId());

                        if(lightManager &&
                           meshObject->getMesh()->getMeshType() == PointMesh::Type::Static)
                        {
                            switch(meshObject->getMesh()->getMeshShape())
                            {
                                case PointMesh::Circle:
                                {
                                    lightManager->createLightShape(
                                        CircleMesh::Cast(meshObject->getMesh())->getCircleShape());
                                }
                                break;

                                case PointMesh::Line:
                                case PointMesh::Chain:
                                {

                                    for(sf::RectangleShape shape :
                                        meshObject->getMesh()->getLineTable())
                                    {
                                        lightManager->createLightShape(shape);
                                    }
                                }
                                break;

                                case PointMesh::Shape::Polygon:
                                {

                                    for(sf::ConvexShape shape :
                                        PolygonMesh::Cast(meshObject->getMesh())->getPolygonTable())
                                    {
                                        lightManager->createLightShape(shape);
                                    }
                                }
                                break;
                            }
                        }

                        layerObject->addChild(physicObject);
                    }

                    chunkRoot->addChild(layerObject);
                }
                break;
            }
        }

        return chunkRoot;
    }

    void WorldBuilder::buildScene(Object::Ptr rootObject)
    {
        /*for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
{
    if(!(*layer)->isVisible())
        continue;

    switch((*layer)->getSecondType())
    {

        case Object::Meshed_Object:
        {
            Object::Ptr layer_object = (*layer)->clone();

            layer_object->setSecondType(Object::Solid_Object);

            auto children = (*layer)->getAllChild();

            for(auto it = children->begin(); it != children->end(); it++)
            {
                //convert into MeshObject
                PhysicsMeshObject::Ptr mesh_object = MeshObject::Cast((*it)->getFirstChild());

                if(!mesh_object->getMesh()->isValid())
                    break;

                PhysicsObject::Ptr physic_object =
m_PhysicsObjectManager.createObject(mesh_object->getMesh());
                physic_object->setSecondType(Object::Solid_Object);
                physic_object->setName(mesh_object->getName());
                physic_object->setCategory(mesh_object->getCategory());
                                        physic_object->setId(mesh_object->getObjectId());
                physic_object->setUserData((void*)physic_object->getId());

                Object::Ptr sprite_object = (*it)->clone();
                sprite_object->setSecondType(Object::Sprite_Object);
                sprite_object->setIsUpdateable(true);
                sprite_object->setPosition(sprite_object->getPosition()-mesh_object->getMesh()->getCenter());

                physic_object->addChild(sprite_object);

                layer_object->addChild(physic_object);
            }

            rootObject->addChild(layer_object);

        }break;

         case Object::Animation_Meshed_Object:
        {
            Object::Ptr layer_object = (*layer)->clone();

            layer_object->setSecondType(Object::Animation_Solid_Object);

            auto children = (*layer)->getAllChild();

            for(auto it = children->begin(); it != children->end(); it++)
            {
                //Convert into MeshObject
                PhysicsMeshObject::Ptr mesh_object = MeshObject::Cast((*it)->getFirstChild());

                if(!mesh_object->getMesh()->isValid())
                    break;

                PhysicsObject::Ptr physic_object =
m_PhysicsObjectManager.createObject(mesh_object->getMesh());
                physic_object->setSecondType(Object::Animation_Solid_Object);
                physic_object->setName(mesh_object->getName());
                physic_object->setCategory(mesh_object->getCategory());
                                        physic_object->setId(mesh_object->getObjectId());
                physic_object->setUserData((void*)physic_object->getId());


                Object::Ptr animation_object = (*it)->clone();
                animation_object->setSecondType(Object::Animation_Object);
                animation_object->setPosition(animation_object->getPosition()-mesh_object->getMesh()->getCenter());

                physic_object->addChild(animation_object);

                layer_object->addChild(physic_object);
            }

            rootObject->addChild(layer_object);

        }break;
    }
        }*/
    }

    void WorldBuilder::destroyAllPhysicObject(Object::Ptr rootObject)
    {
        auto children = rootObject->getAllChild();

        for(auto layer = children->begin(); layer != children->end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Physic_Object ||
               (*layer)->getSecondType() == Object::Solid_Object)
            {
                auto layer_children = (*layer)->getAllChild();

                for(auto it = layer_children->begin(); it != layer_children->end(); it++)
                {
                    PhysicsObject::Ptr physic_object = PhysicsObject::Cast((*it));
                    m_PhysicWorld->DestroyBody(physic_object->getBody());
                }
            }
        }
    }

    void WorldBuilder::setUpdateUI(std::function<void()> fn)
    {
        m_UpdateUI = fn;
    }

    void WorldBuilder::setUpdateUndo(std::function<void()> fn)
    {
        m_UpdateUndo = fn;
        // m_MeshEditor->setUpdateUndo(fn);
    }

    void WorldBuilder::setUpdateLog(std::function<void(const std::string&, int)> fn)
    {
        m_UpdateLog = fn;
        // m_MeshEditor->setUpdateLog(fn);
    }

    void WorldBuilder::setUpdateLogIf(std::function<void(const std::string&, bool, int)> fn)
    {
        m_UpdateLogIf = fn;
        // m_MeshEditor->setUpdateLogIf(fn);
    }

    Object::Ptr WorldBuilder::getSelectedObject()
    {
        return m_SelectedObject;
    }

    void WorldBuilder::setObjectName(const sf::String& name)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setName(name);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setName(name);

        m_UpdateUndo();
    }

    void WorldBuilder::setObjectCategory(const sf::String& category)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setCategory(category);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setCategory(category);

        m_UpdateUndo();
    }

    SpriteObject::Ptr WorldBuilder::loadSprite(nlohmann::json& json)
    {
        sf::Sprite  sprite;
        sf::IntRect rect = m_ResourceManager->getTextureHolder()->getSpriteBound(
            json["sprite"].get<std::string>());
        sprite.setTextureRect(rect);
        sprite.setTexture(m_ResourceManager->getTextureHolder()->getSpriteTexture(
            json["sprite"].get<std::string>()));
        sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

        SpriteObject::Ptr sprite_object(new SpriteObject());
        sprite_object->setSprite(sprite);
        sprite_object->setTextureName(toString(json["sprite"].get<std::string>()));
        sprite_object->setPosition(json["position"]["x"], json["position"]["y"]);
        sprite_object->setRotation(json["rotation"]);
        sprite_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        sprite_object->setColor(sf::Color(json["color"]["r"],
                                          json["color"]["g"],
                                          json["color"]["b"],
                                          json["color"]["a"]));

        sprite_object->setId(json["object_id"]);
        sprite_object->setName(json["name"].get<std::string>());
        sprite_object->setCategory(json["category"].get<std::string>());
        sprite_object->setSecondType(Object::Sprite_Object);
        sprite_object->setIsVisible(json["is_visible"]);
        sprite_object->setIsUpdateable(json["is_updateable"]);
        sprite_object->setIsSelectable(json["is_selectable"]);
        sprite_object->setIsSelected(json["is_selected"]);

        return sprite_object;
    }

    LightIcon::Ptr WorldBuilder::loadLight(nlohmann::json& json)
    {
        sf::Sprite sprite;
        sprite.setTexture(m_LightTexture);
        const auto textureSize = m_LightTexture.getSize();
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

        LightIcon::Ptr light_icon = std::make_shared<LightIcon>();

        light_icon->setSprite(sprite);
        light_icon->setLightmapName(json["lightmap_name"].get<std::string>());
        light_icon->setLightEnabled(json["enabled"]);
        light_icon->setPosition(json["position"]["x"], json["position"]["y"]);
        light_icon->setRotation(json["rotation"]);
        light_icon->setScale(json["scale"]["x"], json["scale"]["y"]);
        light_icon->setColor(sf::Color(json["color"]["r"],
                                       json["color"]["g"],
                                       json["color"]["b"],
                                       json["color"]["a"]));

        light_icon->setId(json["object_id"]);
        light_icon->setName(json["name"].get<std::string>());
        light_icon->setCategory(json["category"].get<std::string>());
        light_icon->setSecondType(Object::Light_Object);
        light_icon->setIsVisible(json["is_visible"]);
        light_icon->setIsUpdateable(json["is_updateable"]);
        light_icon->setIsSelectable(json["is_selectable"]);
        light_icon->setIsSelected(json["is_selected"]);

        return light_icon;
    }

    PhysicsMeshObject::Ptr WorldBuilder::loadMesh(nlohmann::json& json)
    {
        PointMesh::Ptr mesh     = nullptr;
        nlohmann::json meshJson = json["mesh"];

        // shape
        if(meshJson["shape"] == "polygon_mesh")
        {
            mesh = std::make_shared<PolygonMesh>();
        }
        else if(meshJson["shape"] == "circle_mesh")
        {
            mesh = std::make_shared<CircleMesh>();
        }
        else if(meshJson["shape"] == "line_mesh")
        {
            mesh = std::make_shared<PointMesh>(PointMesh::Line);
        }
        else if(meshJson["shape"] == "chain_mesh")
        {
            mesh = std::make_shared<PointMesh>(PointMesh::Chain);
        }
        else if(meshJson["shape"] == "none")
        {
            mesh = std::make_shared<PointMesh>(PointMesh::None);
        }

        // type
        if(meshJson["type"] == "static_mesh")
            mesh->setMeshType(PointMesh::Type::Static);
        else if(meshJson["type"] == "dynamic_mesh")
            mesh->setMeshType(PointMesh::Type::Dynamic);
        else if(meshJson["type"] == "kinematic_mesh")
            mesh->setMeshType(PointMesh::Type::Kinematic);

        const auto     position    = sf::Vector2f(json["position"]["x"], json["position"]["y"]);
        const auto     rotation    = float(json["rotation"]);
        const auto     scale       = sf::Vector2f(json["scale"]["x"], json["scale"]["y"]);

        // vertex
        nlohmann::json vertexTable = meshJson["vertex_table"];
        for(const auto& vertex : vertexTable)
            mesh->addVertex(sf::Vector2f(vertex["x"], vertex["y"]));

        mesh->loadTransform(position, scale, rotation);
        mesh->updateShape();
        mesh->updateColor();

        mesh->setMeshId(json["object_id"]);

        PhysicsMeshObject::Ptr meshObject(new PhysicsMeshObject());

        meshObject->setMesh(mesh);
        meshObject->setId(json["object_id"]);
        meshObject->setName(json["name"].get<std::string>());
        meshObject->setCategory(json["category"].get<std::string>());
        meshObject->setSecondType(Object::Mesh_Object);
        meshObject->setIsVisible(json["is_visible"]);
        meshObject->setIsUpdateable(json["is_updateable"]);
        meshObject->setIsSelectable(json["is_selectable"]);
        meshObject->setIsSelected(json["is_selected"]);
        meshObject->setPosition(position);
        meshObject->setScale(scale);
        meshObject->setRotation(rotation);

        meshObject->setProperty("fixed_rotation", json["fixed_rotation"].get<bool>());
        meshObject->setProperty("sensor", json["sensor"].get<bool>());
        meshObject->setProperty("allow_sleep", json["allow_sleep"].get<bool>());
        meshObject->setProperty("density", json["density"].get<float>());
        meshObject->setProperty("friction", json["friction"].get<float>());
        meshObject->setProperty("restitution", json["restitution"].get<float>());
        meshObject->setProperty("gravity_scale", json["gravity_scale"].get<float>());

        m_MeshEditor->addMesh(meshObject);

        return meshObject;
    }

    TextObject::Ptr WorldBuilder::loadText(nlohmann::json& json)
    {
        nero_log("loading text");

        sf::Text text;
        text.setFont(m_ResourceManager->getFontHolder()->getFont(json["font"].get<std::string>()));
        text.setString(json["content"].get<std::string>());
        text.setCharacterSize(json["font_size"]);
        // text.setLetterSpacing(json["letter_spacing"]);
        // text.setLineSpacing(json["line_spacing"]);
        text.setOutlineThickness(json["outline_thickness"]);
        text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);

        nero_log("loading text 1");

        TextObject::Ptr text_object(new TextObject());
        text_object->setText(text);
        text_object->setPosition(json["position"]["x"], json["position"]["y"]);
        text_object->setRotation(json["rotation"]);
        text_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        text_object->setColor(sf::Color(json["fill_color"]["r"],
                                        json["fill_color"]["g"],
                                        json["fill_color"]["b"],
                                        json["fill_color"]["a"]));
        text_object->setOutlineColor(sf::Color(json["outline_color"]["r"],
                                               json["outline_color"]["g"],
                                               json["outline_color"]["b"],
                                               json["outline_color"]["a"]));

        nero_log("loading text 2");

        text_object->setFont(json["font"].get<std::string>());
        text_object->setContent(json["content"].get<std::string>());
        text_object->setFontSize(json["font_size"]);
        text_object->setLetterSpacing(json["letter_spacing"]);
        text_object->setLineSpacing(json["line_spacing"]);
        text_object->setOutlineThickness(json["outline_thickness"]);
        text_object->setStyle(json["style"]["bold"],
                              json["style"]["italic"],
                              json["style"]["underlined"],
                              json["style"]["strike_through"]);

        nero_log("loading text 3");

        text_object->setId(json["object_id"]);
        text_object->setName(json["name"].get<std::string>());
        text_object->setCategory(json["category"].get<std::string>());
        text_object->setSecondType(Object::Text_Object);
        text_object->setIsVisible(json["is_visible"]);
        text_object->setIsUpdateable(json["is_updateable"]);
        text_object->setIsSelectable(json["is_selectable"]);
        text_object->setIsSelected(json["is_selected"]);

        return text_object;
    }

    void WorldBuilder::updateLayerOrder()
    {
        int i = 0;
        for(auto layer : m_LayerTable)
            layer->setOrder(i++);
    }

    sf::Color WorldBuilder::getLayerColor()
    {
        if(m_SelectedLayer && !m_SelectedLayer->isEmpty())
        {
            return m_SelectedLayer->getColor();
        }

        return sf::Color::White;
    }

    nlohmann::json WorldBuilder::saveScene()
    {
        std::vector<nlohmann::json> layer_table;

        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            layer_table.push_back((*layer)->toJson());
        }

        nlohmann::json scene;

        scene["layer_table"]  = layer_table;
        scene["layer_count"]  = m_LayerCount;
        scene["object_count"] = m_ObjectCount;
        scene["canvas_color"] = graphics::colorToJson(m_CanvasColor);

        return scene;
    }

    void WorldBuilder::loadScene(nlohmann::json scene)
    {
        nero_log("loading scene");

        m_LayerTable.clear();
        m_MeshEditor->destroyAllMesh();
        m_SelectedObject           = nullptr;

        m_LayerCount               = scene["layer_count"];
        m_ObjectCount              = scene["object_count"];
        m_CanvasColor              = graphics::colorFromJson(scene["canvas_color"]);

        nlohmann::json layer_table = scene["layer_table"];

        for(auto& layer : layer_table)
        {
            Object::Type layer_type;

            if(layer["type"] == "sprite_layer")
                layer_type = Object::Sprite_Object;
            else if(layer["type"] == "mesh_layer")
                layer_type = Object::Mesh_Object;
            else if(layer["type"] == "meshed_layer")
                layer_type = Object::Meshed_Object;
            else if(layer["type"] == "animation_layer")
                layer_type = Object::Animation_Object;
            else if(layer["type"] == "animation_meshed_layer")
                layer_type = Object::Animation_Meshed_Object;
            else if(layer["type"] == "button_layer")
                layer_type = Object::Button_Object;
            else if(layer["type"] == "text_layer")
                layer_type = Object::Text_Object;
            else if(layer["type"] == "light_layer")
                layer_type = Object::Light_Object;
            else if(layer["type"] == "empty_layer")
                layer_type = Object::None;

            LayerObject::Ptr layer_object(new LayerObject());

            layer_object->setId(layer["object_id"]);
            layer_object->setName(layer["name"].get<std::string>());
            layer_object->setCategory(layer["category"].get<std::string>());
            layer_object->setSecondType(layer_type);
            layer_object->setIsVisible(layer["is_visible"]);
            layer_object->setIsUpdateable(layer["is_updateable"]);
            layer_object->setIsSelectable(layer["is_selectable"]);
            layer_object->setIsSelected(layer["is_selected"]);

            // layer_object->setOrder(layer["order"]);

            if(layer["is_selected"])
                m_SelectedLayer = layer_object;

            switch(layer_type)
            {
                case Object::Sprite_Object:
                {
                    nlohmann::json sprite_table = layer["sprite_table"];

                    for(auto& sprite : sprite_table)
                        layer_object->addChild(loadSprite(sprite));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Mesh_Object:
                {
                    nlohmann::json mesh_table = layer["mesh_table"];

                    for(auto& mesh : mesh_table)
                        layer_object->addChild(loadMesh(mesh));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Meshed_Object:
                {
                    nlohmann::json meshed_table = layer["meshed_table"];

                    for(auto& meshed : meshed_table)
                    {
                        SpriteObject::Ptr      sprite_object = loadSprite(meshed["sprite"]);
                        PhysicsMeshObject::Ptr mesh_object   = loadMesh(meshed["mesh"]);

                        sprite_object->setSecondType(Object::Meshed_Object);
                        sprite_object->addChild(mesh_object);

                        layer_object->addChild(sprite_object);
                    }

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Animation_Object:
                {
                    nlohmann::json animation_table = layer["animation_table"];

                    for(auto& animation : animation_table)
                        layer_object->addChild(loadAnimation(animation));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Animation_Meshed_Object:
                {
                    nlohmann::json meshed_table = layer["animation_meshed_table"];

                    for(auto& meshed : meshed_table)
                    {
                        AnimationObject::Ptr animation_object = loadAnimation(meshed["animation"]);
                        PhysicsMeshObject::Ptr mesh_object    = loadMesh(meshed["mesh"]);

                        animation_object->setSecondType(Object::Meshed_Object);
                        animation_object->addChild(mesh_object);

                        layer_object->addChild(animation_object);
                    }

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Button_Object:
                {
                    nlohmann::json button_table = layer["button_table"];

                    for(auto& sprite : button_table)
                        layer_object->addChild(loadSprite(sprite));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Text_Object:
                {
                    nlohmann::json text_table = layer["text_table"];

                    for(auto& text : text_table)
                        layer_object->addChild(loadText(text));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::Light_Object:
                {
                    nlohmann::json light_table = layer["light_table"];

                    for(auto& light : light_table)
                        layer_object->addChild(loadLight(light));

                    m_LayerTable.push_back(layer_object);
                }
                break;

                case Object::None:
                {
                    m_LayerTable.push_back(layer_object);
                }
                break;
            }
        }
    }

    void WorldBuilder::setPhysicWorld(std::shared_ptr<b2World> world)
    {
        m_PhysicWorld = world;
        // m_PhysicsObjectManager.setWorld(world);
    }

    int WorldBuilder::getNewId()
    {
        return m_ObjectCount++;
    }

    AnimationObject::Ptr WorldBuilder::loadAnimation(nlohmann::json& json)
    {
        nero_log("loading animation");

        AnimationObject::Ptr animation_object(new AnimationObject());

        std::string          label = json["animation"].get<std::string>();

        Animation            animation;
        auto sequenceMap = m_ResourceManager->getAnimationHolder()->getSequenceMap(label);
        auto sequenceJon = json["sequence_table"];

        nero_log("loading animation");

        nero_log(sequenceMap.size());

        for(auto it = sequenceMap.begin(); it != sequenceMap.end(); it++)
        {
            AnimationSequence sequence;
            sequence.setFrameTable(it->second);
            sequence.setFrameRate(sequenceJon[it->first]["frame_rate"]);
            sequence.setLoop(sequenceJon[it->first]["loop"]);

            animation.addSequence(it->first, sequence);
        }

        sf::Sprite  sprite;
        sf::IntRect rect = m_ResourceManager->getAnimationHolder()->getAnimationBound(label);
        sprite.setTextureRect(rect);
        sprite.setTexture(m_ResourceManager->getAnimationHolder()->getTexture(label));
        sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);

        animation.setSprite(sprite);
        animation.setTexture(label);
        animation.setSequence(json["current_sequence"].get<std::string>());

        animation_object->setAnimation(animation);

        animation_object->setPosition(json["position"]["x"], json["position"]["y"]);
        animation_object->setRotation(json["rotation"]);
        animation_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        animation_object->setColor(sf::Color(json["color"]["r"],
                                             json["color"]["g"],
                                             json["color"]["b"],
                                             json["color"]["a"]));

        animation_object->setId(json["object_id"]);
        animation_object->setName(json["name"].get<std::string>());
        animation_object->setCategory(json["category"].get<std::string>());
        animation_object->setSecondType(Object::Animation_Object);
        animation_object->setIsVisible(json["is_visible"]);
        animation_object->setIsUpdateable(json["is_updateable"]);
        animation_object->setIsSelectable(json["is_selectable"]);
        animation_object->setIsSelected(json["is_selected"]);

        return animation_object;
    }

    void WorldBuilder::moveObjectUp()
    {
        if(!m_SelectedLayer || !m_SelectedObject)
            return;

        // m_UpdateLog("moving Object [" + m_SelectedObject->getName() +  "] Up", nero::Info);

        auto childTable = m_SelectedLayer->getAllChild();

        for(auto it = childTable->begin(); it != childTable->end(); it++)
        {
            if((*it)->getId() == m_SelectedObject->getId() && it != (childTable->end() - 1))
            {
                std::iter_swap(it, it + 1);
                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::moveObjectDown()
    {
        if(!m_SelectedLayer || !m_SelectedObject)
            return;

        // m_UpdateLog("moving Object [" + m_SelectedObject->getName() +  "] Down", nero::Info);

        auto childTable = m_SelectedLayer->getAllChild();

        for(auto it = childTable->begin(); it != childTable->end(); it++)
        {
            if((*it)->getId() == m_SelectedObject->getId() && it != childTable->begin())
            {
                std::iter_swap(it, it - 1);
                m_UpdateUndo();

                break;
            }
        }
    }

    void WorldBuilder::buildUI(UIObject::Ptr rootObject)
    {
        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            if(!(*layer)->isVisible())
                continue;

            switch((*layer)->getSecondType())
            {
                case Object::Button_Object:
                {
                    UIObject::Ptr layer_object = UIObject::Ptr(new UIObject());
                    layer_object->setName((*layer)->getName());
                    layer_object->setId(getNewId());

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr       sprite_object = (*it)->clone();

                        ButtonObject::Ptr button_object = ButtonObject::Ptr(new ButtonObject());
                        button_object->setName(sprite_object->getName());
                        button_object->setCategory(sprite_object->getCategory());
                        sprite_object->setName("no_name");
                        sprite_object->setCategory("no_category");

                        button_object->addChild(sprite_object);

                        layer_object->addChild(button_object);
                    }

                    rootObject->addChild(layer_object);
                }
                break;
            }
        }
    }

    void WorldBuilder::setTextContent(const sf::String& content)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setContent(content);
        }
    }

    void WorldBuilder::setTextFont(const sf::String& font)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            // m_UpdateLog("changing text object [" + m_SelectedObject->getName() + "] font to ["+
            // font +"]", nero::Info);

            sf::Text& text = TextObject::Cast(m_SelectedObject)->getText();
            text.setFont(m_ResourceManager->getFontHolder()->getFont(font));
            TextObject::Cast(m_SelectedObject)->setFont(font);
        }
    }

    void WorldBuilder::setTextFontSize(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            // m_UpdateLog("changing text object [" + m_SelectedObject->getName() + "] font size to
            // "+ toString(value), nero::Info);

            TextObject::Cast(m_SelectedObject)->setFontSize(value);
        }
    }

    void WorldBuilder::setTextLetterSpacing(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setLetterSpacing(value);
        }
    }

    void WorldBuilder::setTextLineSpacing(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setLineSpacing(value);
        }
    }

    void WorldBuilder::setTextOutlineThickness(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            // m_UpdateLog("changing text object [" + m_SelectedObject->getName() + "] outline
            // thickness to "+ toString(value), nero::Info);

            TextObject::Cast(m_SelectedObject)->setOutlineThickness(value);
        }
    }

    void WorldBuilder::setTextStyle(bool bold, bool italic, bool underLined, bool strikeThrough)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            // m_UpdateLog("changing text object [" + m_SelectedObject->getName() + "] font style",
            // Nero::Info);

            TextObject::Cast(m_SelectedObject)->setStyle(bold, italic, underLined, strikeThrough);
        }
    }

    const sf::Color& WorldBuilder::getCanvasColor() const
    {
        return m_CanvasColor;
    }

    void WorldBuilder::setCanvasColor(const sf::Color& color)
    {
        m_CanvasColor = color;
    }

    int WorldBuilder::getObjectCount()
    {
        return m_ObjectCount;
    }

    void WorldBuilder::setRenderContext(const RenderContext::Ptr& renderContext)
    {
        m_RenderContext = renderContext;

        m_MeshEditor->setRenderContext(renderContext);
    }

    void WorldBuilder::setRenderTexture(const RenderTexturePtr& renderTexture)
    {
        m_RenderTexture = renderTexture;

        m_MeshEditor->setRenderTexture(renderTexture);
    }

    void WorldBuilder::setResourceManager(const ResourceManager::Ptr& resourceManager)
    {
        m_ResourceManager = resourceManager;
    }

    LayerObject::Ptr WorldBuilder::getSelectedLayer()
    {
        return m_SelectedLayer;
    }
} // namespace nero
