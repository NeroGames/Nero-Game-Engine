////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/SceneBuilder.h>
//EASYLOG
#include <easyloggingpp/easylogging++.h>
//STD
#include <fstream>
#include <string>
////////////////////////////////////////////////////////////
namespace nero
{
    SceneBuilder::SceneBuilder(sfg::Canvas::Ptr renderCanvas, ResourceManager::Ptr resourceManager, SceneSetting& sceneSetting):
        //Main
         m_RenderCanvas(renderCanvas)
        ,m_ResourceManager(resourceManager)
        ,m_PhysicWorld(nullptr)
        ,m_PhysicObjectManager()
        ,m_SelectionRect()
        ,m_LastMousePosition(0.f, 0.f)
        //Speed
        ,m_PanningSpeed(1.f)
        ,m_RotationSpeed(0.5f)
        ,m_ZoomingRatio(0.1f)
        //Object
        ,m_SelectedLayer(nullptr)
        ,m_SelectedObject(nullptr)
        ,m_LayerCount(0)
        ,m_ObjectCount(0)
        ,m_SceneSetting(sceneSetting)
    {
        m_MeshEditor = MeshEditor::Ptr(new MeshEditor(renderCanvas));

        m_SelectionRect.setFillColor(sf::Color::Transparent);
        m_SelectionRect.setOutlineColor(sf::Color::Green);
        m_SelectionRect.setOutlineThickness(-3.f);

        m_UpdateUI  = [](){};
        m_UpdateUndo    = [](){};
    }

    void SceneBuilder::handleEvent(const sf::Event& event)
    {
        switch(event.type)
        {
            //Keyboard
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

            //Mouse_button
            case sf::Event::MouseButtonPressed:
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

            //Mouse_Mouvement
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

    void SceneBuilder::update(const sf::Time& timeStep)
    {
        float32 b2TimeStep = m_SceneSetting.hz > 0.0f ? 1.0f / m_SceneSetting.hz : float32(0.0f);

        if(b2TimeStep > 0.f)
        {
            b2TimeStep = (b2TimeStep * timeStep.asSeconds())/TIME_PER_FRAME.asSeconds();
        }

        if(m_SelectedObject)
        {
            m_SelectionRect.setPosition(m_SelectedObject->getGlobalBounds().left,  m_SelectedObject->getGlobalBounds().top);
            m_SelectionRect.setSize(sf::Vector2f(m_SelectedObject->getGlobalBounds().width,  m_SelectedObject->getGlobalBounds().height));
        }
        else
            m_SelectionRect.setSize(sf::Vector2f(0.f, 0.f));


        if(m_SelectedLayer)
            m_SelectedLayer->update(sf::seconds(b2TimeStep));
    }

    void SceneBuilder::render()
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->isVisible())
            {
                m_RenderCanvas->Draw(*(*it));
                m_RenderCanvas->Draw(m_SelectionRect);
            }
        }
    }

    void SceneBuilder::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Object
        if(isPressed && m_SelectedLayer && m_SelectedObject)
        {
            //Move
            if(key == sf::Keyboard::Numpad8 && CTRL())
                m_SelectedObject->move(sf::Vector2f(0.f, -m_PanningSpeed));

            if(key == sf::Keyboard::Numpad2 && CTRL())
                m_SelectedObject->move(sf::Vector2f(0.f, m_PanningSpeed));

            if(key == sf::Keyboard::Numpad4 && CTRL())
                m_SelectedObject->move(sf::Vector2f(-m_PanningSpeed, 0.f));

            if(key == sf::Keyboard::Numpad6 && CTRL())
                m_SelectedObject->move(sf::Vector2f(m_PanningSpeed, 0.f));

            //Rotate
            if(key == sf::Keyboard::Numpad7 && CTRL())
                m_SelectedObject->rotate(-m_RotationSpeed);

            else if(key == sf::Keyboard::Numpad9 && CTRL())
                m_SelectedObject->rotate(m_RotationSpeed);

            else if(key == sf::Keyboard::Divide && CTRL())
                m_SelectedObject->rotate(-0.1f);

            else if(key == sf::Keyboard::Multiply && CTRL())
                m_SelectedObject->rotate(0.1f);

            //Zoom
            if(key == sf::Keyboard::Add && CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f+m_ZoomingRatio, 1.f+m_ZoomingRatio));

            else if(key == sf::Keyboard::Subtract && CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f-m_ZoomingRatio, 1.f-m_ZoomingRatio));

            //Flip
            if(key == sf::Keyboard::Numpad1 && CTRL())
                m_SelectedObject->scale(sf::Vector2f(-1.f, 1.f));

            else if(key == sf::Keyboard::Numpad3 && CTRL())
                m_SelectedObject->scale(sf::Vector2f(1.f, -1.f));

            //Unselected
            if (key == sf::Keyboard::Escape)
                m_SelectedObject = nullptr;

            //Delete
            if(key == sf::Keyboard::Delete)
                deleteObject(m_SelectedObject);

            //copy sprite
            if(key == sf::Keyboard::Numpad8 && ALT())
                copyObject(Up);

            else if(key == sf::Keyboard::Numpad2 && ALT())
                copyObject(Down);

            else if(key == sf::Keyboard::Numpad4 && ALT())
                copyObject(Left);

            else if(key == sf::Keyboard::Numpad6 && ALT())
                copyObject(Right);

            else if(key == sf::Keyboard::Numpad7 && ALT())
                copyObject(Up_Left);

            else if(key == sf::Keyboard::Numpad9 && ALT())
                copyObject(Up_Right);

            else if(key == sf::Keyboard::Numpad1 && ALT())
                copyObject(Down_Left);

            else if(key == sf::Keyboard::Numpad3 && ALT())
                copyObject(Down_Right);
        }

        //Layer
        if(isPressed && m_SelectedLayer)
        {
            sf::Vector2f pos = sf::Vector2f(0.f, 0.f);

            if(key == sf::Keyboard::Numpad8 && CTRL_ALT())
                pos = sf::Vector2f(0.f, -5.f);

            else if(key == sf::Keyboard::Numpad2 && CTRL_ALT())
                pos = sf::Vector2f(0.f, 5.f);

            else if(key == sf::Keyboard::Numpad4 && CTRL_ALT())
                pos = sf::Vector2f(-5.f, 0.f);

            else if(key == sf::Keyboard::Numpad6 && CTRL_ALT())
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
            if(key == sf::Keyboard::Numpad8 && CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad2 && CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad4 && CTRL())
                m_UpdateUndo();

            if(key == sf::Keyboard::Numpad6 && CTRL())
                m_UpdateUndo();

            //Rotate
            if(key == sf::Keyboard::Numpad7 && CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad9 && CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Divide && CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Multiply && CTRL())
                m_UpdateUndo();

            //Zoom
            if(key == sf::Keyboard::Add && CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Subtract && CTRL())
                m_UpdateUndo();

            //Flip
            if(key == sf::Keyboard::Numpad1 && CTRL())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad3 && CTRL())
                m_UpdateUndo();

            //copy sprite
            if(key == sf::Keyboard::Numpad8 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad2 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad4 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad6 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad7 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad9 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad1 && ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad3 && ALT())
                m_UpdateUndo();

            else  if(key == sf::Keyboard::Numpad8 && CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad2 && CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad4 && CTRL_ALT())
                m_UpdateUndo();

            else if(key == sf::Keyboard::Numpad6 && CTRL_ALT())
                m_UpdateUndo();
        }
    }

    void SceneBuilder::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_RenderCanvas);

        if(mouse.button == sf::Mouse::Left && isPressed && m_SelectedLayer && m_SelectedLayer->isVisible())
        {
            m_SelectedObject = findObject(m_SelectedLayer, world_pos);

            if(m_SelectedObject && ALT())
            {
                deleteObject(m_SelectedObject);
            }

            m_LastMousePosition = world_pos;
        }

        if (mouse.button == sf::Mouse::Right && isPressed && m_SelectedLayer && m_SelectedLayer->isVisible())
        {
            m_SelectedObject = findObject(m_SelectedLayer, world_pos);

            if(m_SelectedObject)
                m_UpdateUI();
        }

        if (mouse.button == sf::Mouse::Left && !isPressed && m_SelectedLayer && m_SelectedObject)
        {
            m_SelectedObject = nullptr;
            m_UpdateUndo();
        }
    }

    void  SceneBuilder::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_RenderCanvas);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_SelectedLayer && m_SelectedLayer->isVisible() && m_SelectedObject)
        {
            sf::Vector2f diff = world_pos - m_LastMousePosition;

            if(!CTRL_SHIFT_ALT())
                m_SelectedObject->move(diff);

            else if(SHIFT() && diff.y >  0.f)
                m_SelectedObject->rotate(m_RotationSpeed*2.f);

            else if(SHIFT() && diff.y <  0.f)
                m_SelectedObject->rotate(-m_RotationSpeed*2.f);

            else if(CTRL() && diff.y < 0.f)
                m_SelectedObject->scale(sf::Vector2f(1.f+m_ZoomingRatio, 1.f+m_ZoomingRatio));

            else if(CTRL() && diff.y > 0.f)
                m_SelectedObject->scale(sf::Vector2f(1.f-m_ZoomingRatio, 1.f-m_ZoomingRatio));

            m_LastMousePosition = world_pos;
        }
    }

    Object::Ptr SceneBuilder::findObject(Object::Ptr object, sf::Vector2f pos)
    {
        Object::Ptr result = nullptr;
        findObject(object, pos, result);

        return result;
    }


    void SceneBuilder::findObject(Object::Ptr object, sf::Vector2f pos, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getGlobalBounds().contains(pos))
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for (auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, pos, result);
        }
    }

    Object::Ptr SceneBuilder::findObject(Object::Ptr object, sf::String name)
    {
        Object::Ptr result = nullptr;
        findObject(object, name, result);

        return result;
    }

    void SceneBuilder::findObject(Object::Ptr object, sf::String name, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getName() == name)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for (auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, name, result);
        }
    }

    Object::Ptr SceneBuilder::findObject(Object::Ptr object, int id)
    {
        Object::Ptr result = nullptr;
        findObject(object, id, result);

        return result;
    }

    void SceneBuilder::findObject(Object::Ptr object, int id, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getId() == id)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for (auto it = childTab->rbegin(); it != childTab->rend(); it++)
                findObject(*it, id, result);
        }
    }

    const LayerObject::Ptr SceneBuilder::addLayer()
    {
        if(m_SelectedLayer)
            m_SelectedLayer->setIsSelected(false);

        int index =  ++m_LayerCount;

        LayerObject::Ptr Layer_object(new LayerObject());
        Layer_object->setId(getNewId());
        Layer_object->setIsVisible(true);
        Layer_object->setIsSelected(true);
        Layer_object->setName("Layer " + toString(index));
        Layer_object->setOrder(0);

        m_LayerTable.push_back(Layer_object);

        m_SelectedLayer = Layer_object;
        m_SelectedObject = nullptr;

        m_UpdateUndo();

        return m_LayerTable.back();
    }

    void SceneBuilder::deleteLayer(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id)
            {
                if(it != m_LayerTable.begin())
                    selectLayer((*(it-1))->getId());
                else if(it == m_LayerTable.begin() && m_LayerTable.size() != 1)
                    selectLayer((*(it+1))->getId());

                m_LayerTable.erase(it);

                m_UpdateUndo();

                break;
            }
        }
    }

    void SceneBuilder::moveLayerDown(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != m_LayerTable.begin())
            {
                std::iter_swap(it, it-1);
                selectLayer((*(it-1))->getId());

                m_UpdateUndo();

                break;
            }
        }
    }

    void SceneBuilder::moveLayerUp(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.end()-1))
            {
                std::iter_swap(it, it+1);
                selectLayer((*(it+1))->getId());

                m_UpdateUndo();

                break;
            }
        }
    }

    bool SceneBuilder::mergeLayerUp(int id)
    {
        nero_log("up up my layer");

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.end()-1))
            {
                if((*it)->getSecondType() == (*(it+1))->getSecondType())
                {
                    auto childTable = (*it)->getAllChild();

                    for(auto child : *childTable)
                        (*(it+1))->addChild(child);

                    selectLayer((*(it+1))->getId());

                    m_LayerTable.erase(it);

                    m_UpdateUndo();

                    return true;
                }

                break;
            }
        }

        return false;
    }

    bool SceneBuilder::mergeLayerDown(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTable.begin()))
            {
                if((*it)->getSecondType() == (*(it-1))->getSecondType())
                {
                    auto childTable = (*it)->getAllChild();

                    for(auto child : *childTable)
                        (*(it-1))->addChild(child);

                    selectLayer((*(it-1))->getId());

                    m_LayerTable.erase(it);

                    m_UpdateUndo();

                    return true;
                }

                break;
            }
        }

        return false;
    }

    void SceneBuilder::updateLayerVisibility(int id)
    {
        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if((*it)->getId() == id)
            {
                (*it)->setIsVisible(!(*it)->isVisible());

                m_UpdateUndo();

                break;
            }
        }
    }

    void SceneBuilder::updateLayerName(int id, sf::String name)
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


    void SceneBuilder::selectLayer(int id)
    {
        m_SelectedLayer->setIsSelected(false);
        m_SelectedLayer = nullptr;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); ++it)
        {
            if((*it)->getId() == id)
            {
                m_SelectedLayer = *it;
                m_SelectedLayer->setIsSelected(true);
                m_SelectedObject = nullptr;

                m_UpdateUndo();

                break;
            }
        }
    }


    void SceneBuilder::copyObject(const Position& position)
    {
        if(!m_SelectedLayer || !m_SelectedObject)
            return;

        sf::Vector2f pos;

        switch(position)
        {
            case Up:
                pos = sf::Vector2f(0.f, -m_SelectionRect.getSize().y);
                break;
            case Down:
                pos = sf::Vector2f(0.f, m_SelectionRect.getSize().y);
                break;
            case Left:
                pos = sf::Vector2f(-m_SelectionRect.getSize().x, 0.f);
                break;
            case Right:
                pos = sf::Vector2f(m_SelectionRect.getSize().x, 0.f);
                break;
            case Up_Left:
                pos = sf::Vector2f(-m_SelectionRect.getSize().x, -m_SelectionRect.getSize().y);
                break;
            case Up_Right:
                pos = sf::Vector2f(m_SelectionRect.getSize().x, -m_SelectionRect.getSize().y);
                break;
            case Down_Left:
                pos = sf::Vector2f(-m_SelectionRect.getSize().x, m_SelectionRect.getSize().y);
                break;
            case Down_Right:
                pos = sf::Vector2f(m_SelectionRect.getSize().x, m_SelectionRect.getSize().y);
                break;
        }

        Object::Ptr object = m_SelectedObject->clone(pos);
        object->setId(getNewId());

        if(object->getSecondType() == Object::Mesh_Object)
        {
            MeshObject::Ptr mesh_object = MeshObject::Cast(object);
            mesh_object->getMesh()->setId(mesh_object->getId());
            m_MeshEditor->addMesh(mesh_object->getMesh());
        }

        else if(object->getSecondType() == Object::Meshed_Object || object->getSecondType() == Object::Animation_Meshed_Object)
        {
            sf::Vector2f posf = -pos;
            Object::Ptr child_object = m_SelectedObject->getFirstChild()->clone(posf);
            child_object->setId(getNewId());

            MeshObject::Ptr mesh_object = MeshObject::Cast(child_object);
            mesh_object->getMesh()->setId(mesh_object->getId());
            mesh_object->setParentLastPosition(object->getPosition()-pos);
            mesh_object->setParentLastRotation(object->getRotation());
            mesh_object->setParentLastScale(object->getScale());
            mesh_object->setIsSelectable(false);

            object->setIsUpdateable(true);
            object->addChild(child_object);

            m_MeshEditor->addMesh(mesh_object->getMesh());
        }

        m_SelectedObject = object;
        m_SelectedLayer->addChild(m_SelectedObject);
    }

    void SceneBuilder::updateLayerColor(const sf::Color& color)
    {
        if(m_SelectedLayer->getSecondType() == Object::Mesh_Object)
            return;

        auto children = m_SelectedLayer->getAllChild();

        for(auto it = children->begin(); it != children->end(); it++)
        {
            (*it)->setColor(color);
        }
    }

    void SceneBuilder::updateAllLayerAlpha(int alpha)
    {

        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Sprite_Object || (*layer)->getSecondType() == Object::Animation_Object || (*layer)->getSecondType() == Object::Meshed_Object || (*layer)->getSecondType() == Object::Animation_Meshed_Object)
            {
                auto children = (*layer)->getAllChild();

                for(auto it = children->begin(); it != children->end(); it++)
                {
                    sf::Color color = (*it)->getColor();
                    color.a = alpha;

                    (*it)->setColor(color);
                }
            }
        }

    }

    void SceneBuilder::updateSpriteColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && (m_SelectedObject->getFirstType() == Object::Sprite_Object || m_SelectedObject->getFirstType() == Object::Animation_Object))
            m_SelectedObject->setColor(color);
    }

    void SceneBuilder::updateTextColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && m_SelectedObject->getFirstType() == Object::Text_Object)
            TextObject::Cast(m_SelectedObject)->setColor(color);
    }

    void SceneBuilder::updateOutlineTextColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && m_SelectedObject->getFirstType() == Object::Text_Object)
            TextObject::Cast(m_SelectedObject)->setOutlineColor(color);
    }

    const LayerObject::Tab& SceneBuilder::getLayerTable() const
    {
       return m_LayerTable;
    }

    MeshEditor::Ptr SceneBuilder::getMeshEditor()
    {
        return  m_MeshEditor;
    }

    bool SceneBuilder::addObject(Object::Type type, const sf::String& label, sf::Vector2f position)
    {
        Object::Ptr object = nullptr;

        switch(type)
        {
            case Object::Sprite_Object:
            {
                sf::Sprite sprite;
                sf::IntRect rect = m_ResourceManager->texture.getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->texture.getSpriteTexture(label));
                sprite.setOrigin(rect.width/2.f, rect.height/2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Sprite_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());

                object = sprite_object;

            }break;

            case Object::Mesh_Object:
            {
                if(!m_SelectedObject)
                {
                    Mesh mesh;

                    if(label == "Polygon")
                        mesh = Mesh(Mesh::Polygon_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                    else if(label == "Circle")
                        mesh = Mesh(Mesh::Circle_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                    else if(label == "Line")
                        mesh = Mesh(Mesh::Line_Mesh, sf::Vector2f(0.f, 0.f), 75.f);

                    mesh.setId(getNewId());

                    MeshObject::Ptr mesh_object(new MeshObject());
                    mesh_object->setId(mesh.getId());
                    mesh_object->setMesh(mesh);
                    mesh_object->setPosition(position);
                    mesh_object->setSecondType(Object::Mesh_Object);

                    m_MeshEditor->addMesh(mesh_object->getMesh());

                    object = mesh_object;
                }
                else if(m_SelectedObject && (m_SelectedObject->getSecondType() == Object::Meshed_Object || m_SelectedObject->getSecondType() == Object::Animation_Meshed_Object))
                {
                    m_MeshEditor->deleteMesh(m_SelectedObject->getFirstChild()->getId());
                    m_SelectedObject->removeFirstChild();

                    Mesh mesh;

                    if(label == "Polygon")
                        mesh = Mesh(Mesh::Polygon_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                    else if(label == "Circle")
                        mesh = Mesh(Mesh::Circle_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                    else if(label == "Line")
                        mesh = Mesh(Mesh::Line_Mesh, sf::Vector2f(0.f, 0.f), 75.f);

                    mesh.setId(getNewId());
                    std::shared_ptr<MeshObject> mesh_object(new MeshObject());
                    mesh_object->setId(mesh.getId());
                    mesh_object->setMesh(mesh);
                    mesh_object->setIsSelectable(false);
                    mesh_object->setSecondType(Object::Mesh_Object);
                    mesh_object->setName(m_SelectedObject->getName());
                    mesh_object->setCategory(m_SelectedObject->getCategory());

                    m_MeshEditor->addMesh(mesh_object->getMesh());

                    m_SelectedObject->addChild(mesh_object);

                    m_SelectedObject->update(TIME_PER_FRAME);

                    nero_log("change animation mesh");
                    m_UpdateUndo();

                    return false;
                }
                else
                {
                    return false;
                }

            }break;

            case Object::Meshed_Object:
            {
                //Sprite Object
                sf::Sprite sprite;
                sf::IntRect rect = m_ResourceManager->texture.getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->texture.getSpriteTexture(label));
                sprite.setOrigin(rect.width/2.f, rect.height/2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Meshed_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());
                sprite_object->setIsUpdateable(true);

                //Mesh Object
                Mesh mesh = Mesh(Mesh::Polygon_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                mesh.setId(getNewId());
                MeshObject::Ptr mesh_object(new MeshObject());
                mesh_object->setId(mesh.getId());
                mesh_object->setMesh(mesh);
                mesh_object->setSecondType(Object::Mesh_Object);
                mesh_object->setIsSelectable(false);

                m_MeshEditor->addMesh(mesh_object->getMesh());
                sprite_object->addChild(mesh_object);

                //update one time
                sprite_object->update(TIME_PER_FRAME);

                object = sprite_object;

            }break;

            case Object::Animation_Object:
            {
                Animation animation;
                auto sequenceMap = m_ResourceManager->animation.getSequenceMap(label);

                for(auto it=sequenceMap.begin(); it!=sequenceMap.end(); it++)
                {
                    AnimationSequence sequence;
                    sequence.setFrameTable(it->second);

                    animation.addSequence(it->first, sequence);
                }

                sf::Sprite sprite;
                sf::IntRect rect = m_ResourceManager->animation.getAnimationBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->animation.getTexture(label));
                sprite.setOrigin(rect.width/2.f, rect.height/2.f);


                animation.setSprite(sprite);
                animation.setTexture(label);
                animation.setSequence("idle");

                AnimationObject::Ptr animation_object(new AnimationObject());
                animation_object->setAnimation(animation);
                animation_object->setSecondType(Object::Animation_Object);
                animation_object->setPosition(position);
                animation_object->setId(getNewId());

                object = animation_object;

            }break;

            case Object::Animation_Meshed_Object:
            {
                Animation animation;
                auto sequenceMap = m_ResourceManager->animation.getSequenceMap(label);

                for(auto it=sequenceMap.begin(); it!=sequenceMap.end(); it++)
                {
                    AnimationSequence sequence;
                    sequence.setFrameTable(it->second);

                    animation.addSequence(it->first, sequence);
                }

                sf::Sprite sprite;
                sf::IntRect rect = m_ResourceManager->animation.getAnimationBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->animation.getTexture(label));
                sprite.setOrigin(rect.width/2.f, rect.height/2.f);

                animation.setSprite(sprite);
                animation.setTexture(label);
                animation.setSequence("idle");


                AnimationObject::Ptr animation_object(new AnimationObject());
                animation_object->setAnimation(animation);
                animation_object->setSecondType(Object::Animation_Meshed_Object);
                animation_object->setPosition(position);
                animation_object->setId(getNewId());

                  //Mesh Object
                Mesh mesh = Mesh(Mesh::Polygon_Mesh, sf::Vector2f(0.f, 0.f), 50.f);
                mesh.setId(getNewId());
                MeshObject::Ptr mesh_object(new MeshObject());
                mesh_object->setId(mesh.getId());
                mesh_object->setMesh(mesh);
                mesh_object->setSecondType(Object::Mesh_Object);
                mesh_object->setIsSelectable(false);

                m_MeshEditor->addMesh(mesh_object->getMesh());
                animation_object->addChild(mesh_object);

                //update one time
                animation_object->update(TIME_PER_FRAME);

                object = animation_object;

            }break;

            case Object::Text_Object:
            {
                sf::Text text;
                text.setFont(m_ResourceManager->font.getDefaultFont());
                text.setString(label);
                text.setCharacterSize(20.f);
                text.setOrigin(text.getLocalBounds().width/2.f, text.getLocalBounds().height/2.f);

                TextObject::Ptr text_object(new TextObject());
                text_object->setText(text);
                text_object->setPosition(position);
                text_object->setId(getNewId());

                object = text_object;

            }break;

            case Object::Button_Object:
            {
                sf::Sprite sprite;
                sf::IntRect rect = m_ResourceManager->texture.getSpriteBound(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_ResourceManager->texture.getSpriteTexture(label));
                sprite.setOrigin(rect.width/2.f, rect.height/2.f);

                SpriteObject::Ptr sprite_object(new SpriteObject());
                sprite_object->setSprite(sprite);
                sprite_object->setTextureName(label);
                sprite_object->setSecondType(Object::Button_Object);
                sprite_object->setPosition(position);
                sprite_object->setId(getNewId());
                sprite_object->setColor(sf::Color(255, 255, 255, 150));

                object = sprite_object;

            }break;
        }

        if(m_SelectedLayer->getSecondType() == type)
        {
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_UpdateUndo();
            return false;
        }
        else if (m_SelectedLayer->getSecondType() == Object::None || m_SelectedLayer->isEmpty())
        {
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_UpdateUndo();
            return true;
        }
        else if(m_SelectedLayer->getSecondType() != type && m_SelectedLayer->getSecondType() != Object::None)
        {
            addLayer();
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_UpdateUndo();
            return true;
        }
    }

    void SceneBuilder::deleteObject(Object::Ptr object)
    {
        if(object->getFirstType() == Object::Mesh_Object)
        {
            m_MeshEditor->deleteMesh(object->getId());
        }

        else if(object->getSecondType() == Object::Meshed_Object)
        {
            m_MeshEditor->deleteMesh(object->getFirstChild()->getId());
        }

        m_SelectedLayer->removeChild(object);

//        if(m_SelectedLayer->getChildCount() == 0)
//        {
//             m_SelectedLayer->setSecondType(Object::None);
//        }

        m_SelectedObject = nullptr;

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshType(const sf::String& label)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        Mesh::Type type;

        if(label == "Static")
            type = Mesh::Static_Mesh;
        else if (label == "Kinematic")
            type = Mesh::Kinematic_Mesh;
        else if(label == "Dynamic")
            type = Mesh::Dynamic_Mesh;


        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshType(type);

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshFixedRotation(bool flag)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFixedRotation(flag);

        m_UpdateUndo();

    }

    void SceneBuilder::setMeshSensor(bool flag)
    {
         if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshSensor(flag);

        m_UpdateUndo();

    }

    void SceneBuilder::setMeshAllowSleep(bool flag)
    {
       if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshAllowSleep(flag);

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshDensity(float density)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshDensity(density);

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshFriction(float friction)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFriction(friction);

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshRestitution(float restitution)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshRestitution(restitution);

        m_UpdateUndo();
    }

    void SceneBuilder::setMeshGravityScale(float gravityScale)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object && m_SelectedObject->getSecondType() != Object::Animation_Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = MeshObject::Cast(m_SelectedObject);
        else
            mesh_object = MeshObject::Cast(m_SelectedObject->getFirstChild());

        mesh_object->setMeshGravityScale(gravityScale);

        m_UpdateUndo();
    }

    void SceneBuilder::buildScene(Object::Ptr rootObject)
    {
        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
        {
            if(!(*layer)->isVisible())
                continue;

            switch((*layer)->getSecondType())
            {
                case Object::Sprite_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr sprite_object = (*it)->clone();
                        layer_object->addChild(sprite_object);
                    }

                    rootObject->addChild(layer_object);

                }break;

                case Object::Mesh_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();
                    layer_object->setSecondType(Object::Physic_Object);

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        //convert into MeshObject
                        MeshObject::Ptr mesh_object = MeshObject::Cast(*it);

                        if(!mesh_object->getMesh()->isValid())
                            continue;

                        PhysicObject::Ptr physic_object = m_PhysicObjectManager.createObject(mesh_object->getMesh());
                        physic_object->setName(mesh_object->getName());
                        physic_object->setCategory(mesh_object->getCategory());
                        physic_object->setId(mesh_object->getId());
                        physic_object->setUserData((void*)physic_object->getId());

                        layer_object->addChild(physic_object);
                    }

                    rootObject->addChild(layer_object);

                }break;

                case Object::Meshed_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    layer_object->setSecondType(Object::Solid_Object);

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        //convert into MeshObject
                        MeshObject::Ptr mesh_object = MeshObject::Cast((*it)->getFirstChild());

                        if(!mesh_object->getMesh()->isValid())
                            break;

                        PhysicObject::Ptr physic_object = m_PhysicObjectManager.createObject(mesh_object->getMesh());
                        physic_object->setSecondType(Object::Solid_Object);
                        physic_object->setName(mesh_object->getName());
                        physic_object->setCategory(mesh_object->getCategory());
                        physic_object->setId(mesh_object->getId());
                        physic_object->setUserData((void*)physic_object->getId());


                        Object::Ptr sprite_object = (*it)->clone();
                        sprite_object->setSecondType(Object::Sprite_Object);
                        sprite_object->setIsUpdateable(true);

                        if(mesh_object->getMesh()->getShape() == Mesh::Circle_Mesh)
                        {
                            sf::Vector2f pos;
                            pos.x = - mesh_object->getMesh()->getPosition().x + (*it)->getPosition().x;
                            pos.y = - mesh_object->getMesh()->getPosition().y + (*it)->getPosition().y;

                            sprite_object->setPosition(pos);
                        }


                        physic_object->addChild(sprite_object);

                        layer_object->addChild(physic_object);
                    }

                    rootObject->addChild(layer_object);

                }break;

                case Object::Animation_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr animation_object = (*it)->clone();
                        layer_object->addChild(animation_object);
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
                        MeshObject::Ptr mesh_object = MeshObject::Cast((*it)->getFirstChild());

                        if(!mesh_object->getMesh()->isValid())
                            break;

                        PhysicObject::Ptr physic_object = m_PhysicObjectManager.createObject(mesh_object->getMesh());
                        physic_object->setSecondType(Object::Solid_Object);
                        physic_object->setName(mesh_object->getName());
                        physic_object->setCategory(mesh_object->getCategory());
                        physic_object->setId(mesh_object->getId());
                        physic_object->setUserData((void*)physic_object->getId());


                        Object::Ptr animation_object = (*it)->clone();
                        animation_object->setSecondType(Object::Animation_Object);

                        if(mesh_object->getMesh()->getShape() == Mesh::Circle_Mesh)
                        {
                            sf::Vector2f pos;
                            pos.x = - mesh_object->getMesh()->getPosition().x + (*it)->getPosition().x;
                            pos.y = - mesh_object->getMesh()->getPosition().y + (*it)->getPosition().y;

                            animation_object->setPosition(pos);
                        }

                        physic_object->addChild(animation_object);

                        layer_object->addChild(physic_object);
                    }

                    rootObject->addChild(layer_object);

                }break;

                case Object::Text_Object:
                {
                    Object::Ptr layer_object = (*layer)->clone();

                    auto children = (*layer)->getAllChild();

                    for(auto it = children->begin(); it != children->end(); it++)
                    {
                        Object::Ptr text_object = (*it)->clone();
                        layer_object->addChild(text_object);
                    }

                    rootObject->addChild(layer_object);

                }break;
            }
        }
    }

    void SceneBuilder::destroyAllPhysicObject(Object::Ptr rootObject)
    {
        auto children = rootObject->getAllChild();

        for(auto layer = children->begin(); layer != children->end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Physic_Object || (*layer)->getSecondType() == Object::Solid_Object)
            {
                auto layer_children = (*layer)->getAllChild();

                for(auto it = layer_children->begin(); it != layer_children->end(); it++)
                {
                    PhysicObject::Ptr physic_object = PhysicObject::Cast((*it));
                    m_PhysicWorld->DestroyBody(physic_object->getBody());
                }
            }
        }
    }


    void SceneBuilder::setUpdateUI(std::function<void()>  fn)
    {
        m_UpdateUI = fn;
    }

    void SceneBuilder::setUpdateUndo(std::function<void()>  fn)
    {
        m_UpdateUndo = fn;
         m_MeshEditor->setUpdateUndo(fn);
    }

    void SceneBuilder::setUpdateLog(std::function<void(const std::string&, int)>  fn)
    {
        m_UpdateLog = fn;
        m_MeshEditor->setUpdateLog(fn);
    }

    void SceneBuilder::setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn)
    {
        m_UpdateLogIf = fn;
        m_MeshEditor->setUpdateLogIf(fn);
    }

    Object::Ptr SceneBuilder::getSelectedObject()
    {
        return m_SelectedObject;
    }

    void SceneBuilder::setObjectName(const sf::String& name)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setName(name);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setName(name);

        m_UpdateUndo();
    }

    void SceneBuilder::setObjectCategory(const sf::String& category)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setCategory(category);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setCategory(category);

        m_UpdateUndo();
    }

    SpriteObject::Ptr SceneBuilder::loadSprite(nlohmann::json& json)
    {
        sf::Sprite sprite;
        sf::IntRect rect = m_ResourceManager->texture.getSpriteBound(json["sprite"].get<std::string>());
        sprite.setTextureRect(rect);
        sprite.setTexture(m_ResourceManager->texture.getSpriteTexture(json["sprite"].get<std::string>()));
        sprite.setOrigin(rect.width/2.f, rect.height/2.f);

        SpriteObject::Ptr sprite_object(new SpriteObject());
        sprite_object->setSprite(sprite);
        sprite_object->setTextureName(toString(json["sprite"].get<std::string>()));
        sprite_object->setPosition(json["position"]["x"], json["position"]["y"]);
        sprite_object->setRotation(json["rotation"]);
        sprite_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        sprite_object->setColor(sf::Color(json["color"]["r"], json["color"]["g"], json["color"]["b"], json["color"]["a"]));

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

    MeshObject::Ptr SceneBuilder::loadMesh(nlohmann::json& json)
    {
        Mesh mesh = Mesh();

        nlohmann::json mesh_json = json["mesh"];
        //shape
        if(mesh_json["shape"] == "polygon_mesh")
            mesh.setShape(Mesh::Polygon_Mesh);
        else if(mesh_json["shape"] == "circle_mesh")
            mesh.setShape(Mesh::Circle_Mesh);
        else if(mesh_json["shape"] == "line_mesh")
            mesh.setShape(Mesh::Line_Mesh);
        else if(mesh_json["shape"] == "chain_mesh")
            mesh.setShape(Mesh::Chain_Mesh);

        //type
        if(mesh_json["type"] == "static_mesh")
            mesh.setType(Mesh::Static_Mesh);
        else if(mesh_json["type"] == "dynamic_mesh")
            mesh.setType(Mesh::Dynamic_Mesh);
        else if(mesh_json["type"] == "kinematic_mesh")
            mesh.setType(Mesh::Kinematic_Mesh);

        //properties
        mesh.setFixedRotation(mesh_json["fixed_rotation"]);
        mesh.setIsSensor(mesh_json["is_sensor"]);
        mesh.setAllowSleep(mesh_json["allow_sleep"]);
        mesh.setDensity(mesh_json["density"]);
        mesh.setFriction(mesh_json["friction"]);
        mesh.setRestitution(mesh_json["restitution"]);
        mesh.setGravityScale(mesh_json["gravity_scale"]);
        mesh.setIsValid(mesh_json["is_valid"]);
        mesh.setId(json["object_id"]);

        //vertex
        nlohmann::json vertex_table = mesh_json["vertex_table"];

        for (auto& vertex : vertex_table)
            mesh.addVertex(sf::Vector2f(vertex["x"], vertex["y"]));

        mesh.updateShape();
        mesh.updateColor();

        MeshObject::Ptr mesh_object(new MeshObject());

        mesh_object->setMesh(mesh);
        mesh_object->setId(json["object_id"]);
        mesh_object->setName(json["name"].get<std::string>());
        mesh_object->setCategory(json["category"].get<std::string>());
        mesh_object->setSecondType(Object::Mesh_Object);
        mesh_object->setIsVisible(json["is_visible"]);
        mesh_object->setIsUpdateable(json["is_updateable"]);
        mesh_object->setIsSelectable(json["is_selectable"]);
        mesh_object->setIsSelected(json["is_selected"]);

        m_MeshEditor->addMesh(mesh_object->getMesh());

        return mesh_object;
    }

    TextObject::Ptr SceneBuilder::loadText(nlohmann::json& json)
    {
        nero_log("loading text");

        sf::Text text;
        text.setFont(m_ResourceManager->font.getFont(json["font"].get<std::string>()));
        text.setString(json["content"].get<std::string>());
        text.setCharacterSize(json["font_size"]);
        //text.setLetterSpacing(json["letter_spacing"]);
        //text.setLineSpacing(json["line_spacing"]);
        text.setOutlineThickness(json["outline_thickness"]);
        text.setOrigin(text.getLocalBounds().width/2.f, text.getLocalBounds().height/2.f);

        nero_log("loading text 1");

        TextObject::Ptr text_object(new TextObject());
        text_object->setText(text);
        text_object->setPosition(json["position"]["x"], json["position"]["y"]);
        text_object->setRotation(json["rotation"]);
        text_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        text_object->setColor(sf::Color(json["fill_color"]["r"], json["fill_color"]["g"], json["fill_color"]["b"], json["fill_color"]["a"]));
        text_object->setOutlineColor(sf::Color(json["outline_color"]["r"], json["outline_color"]["g"], json["outline_color"]["b"], json["outline_color"]["a"]));

        nero_log("loading text 2");

        text_object->setFont(json["font"].get<std::string>());
        text_object->setContent(json["content"].get<std::string>());
        text_object->setFontSize(json["font_size"]);
        text_object->setLetterSpacing(json["letter_spacing"]);
        text_object->setLineSpacing(json["line_spacing"]);
        text_object->setOutlineThickness(json["outline_thickness"]);
        text_object->setStyle(json["style"]["bold"], json["style"]["italic"], json["style"]["underlined"], json["style"]["strike_through"]);

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


    void SceneBuilder::updateLayerOrder()
    {
        int i = 0;
        for(auto layer : m_LayerTable)
            layer->setOrder(i++);
    }

    sf::Color SceneBuilder::getLayerColor()
    {
        if(m_SelectedLayer && !m_SelectedLayer->isEmpty())
        {
            return m_SelectedLayer->getColor();
        }

        return sf::Color::White;
    }

    nlohmann::json SceneBuilder::saveScene()
    {
        std::vector<nlohmann::json> layer_table;

        for(auto layer = m_LayerTable.begin(); layer != m_LayerTable.end(); layer++)
            layer_table.push_back((*layer)->toJson());

        nlohmann::json scene;

        scene["layer_table"]    = layer_table;
        scene["layer_count"]    = m_LayerCount;
        scene["object_count"]   = m_ObjectCount;
        scene["canvas_color"]   = nero::toJson(m_CanvasColor);

        return scene;
    }

    void SceneBuilder::loadScene(nlohmann::json scene)
    {
        nero_log("loading scene");

        m_LayerTable.clear();
        m_MeshEditor->destroyAllMesh();
        m_SelectedObject = nullptr;

        m_LayerCount    = scene["layer_count"];
        m_ObjectCount   = scene["object_count"];
        m_CanvasColor   = colorFromJson(scene["canvas_color"]);

        nlohmann::json layer_table = scene["layer_table"];

        for (auto& layer : layer_table)
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

            layer_object->setOrder(layer["order"]);

            if(layer["is_selected"])
                m_SelectedLayer = layer_object;

            switch(layer_type)
            {
                case Object::Sprite_Object:
                {
                    nlohmann::json sprite_table = layer["sprite_table"];

                    for (auto& sprite : sprite_table)
                        layer_object->addChild(loadSprite(sprite));

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Mesh_Object:
                {
                    nlohmann::json mesh_table = layer["mesh_table"];

                    for (auto& mesh : mesh_table)
                        layer_object->addChild(loadMesh(mesh));

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Meshed_Object:
                {
                    nlohmann::json meshed_table = layer["meshed_table"];

                    for (auto& meshed : meshed_table)
                    {
                        SpriteObject::Ptr   sprite_object   = loadSprite(meshed["sprite"]);
                        MeshObject::Ptr     mesh_object     = loadMesh(meshed["mesh"]);

                        sprite_object->setSecondType(Object::Meshed_Object);
                        mesh_object->setParentLastPosition(sprite_object->getPosition());
                        mesh_object->setParentLastRotation(sprite_object->getRotation());
                        mesh_object->setParentLastScale(sprite_object->getScale());
                        sprite_object->addChild(mesh_object);

                        layer_object->addChild(sprite_object);

                    }

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Animation_Object:
                {
                    nlohmann::json animation_table = layer["animation_table"];

                    for (auto& animation : animation_table)
                        layer_object->addChild(loadAnimation(animation));

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Animation_Meshed_Object:
                {
                    nlohmann::json meshed_table = layer["animation_meshed_table"];

                    for (auto& meshed : meshed_table)
                    {
                        AnimationObject::Ptr      animation_object   = loadAnimation(meshed["animation"]);
                        MeshObject::Ptr            mesh_object     = loadMesh(meshed["mesh"]);

                        animation_object->setSecondType(Object::Meshed_Object);
                        mesh_object->setParentLastPosition(animation_object->getPosition());
                        mesh_object->setParentLastRotation(animation_object->getRotation());
                        mesh_object->setParentLastScale(animation_object->getScale());
                        animation_object->addChild(mesh_object);

                        layer_object->addChild(animation_object);

                    }

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Button_Object:
                {
                    nlohmann::json button_table = layer["button_table"];

                    for (auto& sprite : button_table)
                        layer_object->addChild(loadSprite(sprite));

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::Text_Object:
                {
                    nlohmann::json text_table = layer["text_table"];

                    for (auto& text : text_table)
                        layer_object->addChild(loadText(text));

                    m_LayerTable.push_back(layer_object);

                } break;

                case Object::None:
                {
                    m_LayerTable.push_back(layer_object);

                } break;
            }
        }
    }

    void SceneBuilder::setPhysicWorld(b2World* world)
    {
        m_PhysicWorld = world;
        m_PhysicObjectManager.setWorld(world);
    }

    int SceneBuilder::getNewId()
    {
        return m_ObjectCount++;
    }

    AnimationObject::Ptr SceneBuilder::loadAnimation(nlohmann::json& json)
    {
        nero_log("loading animation");

        AnimationObject::Ptr animation_object(new AnimationObject());

        std::string label = json["animation"].get<std::string>();

        Animation animation;
        auto sequenceMap = m_ResourceManager->animation.getSequenceMap(label);
        auto sequenceJon = json["sequence_table"];

        nero_log("loading animation");

        nero_log(sequenceMap.size());

        for(auto it=sequenceMap.begin(); it!=sequenceMap.end(); it++)
        {
            AnimationSequence sequence;
            sequence.setFrameTable(it->second);
            sequence.setFrameRate(sequenceJon[it->first]["frame_rate"]);
            sequence.setLoop(sequenceJon[it->first]["loop"]);

            animation.addSequence(it->first, sequence);
        }

        sf::Sprite sprite;
        sf::IntRect rect = m_ResourceManager->animation.getAnimationBound(label);
        sprite.setTextureRect(rect);
        sprite.setTexture(m_ResourceManager->animation.getTexture(label));
        sprite.setOrigin(rect.width/2.f, rect.height/2.f);


        animation.setSprite(sprite);
        animation.setTexture(label);
        animation.setSequence("idle");

        animation_object->setAnimation(animation);

        animation_object->setPosition(json["position"]["x"], json["position"]["y"]);
        animation_object->setRotation(json["rotation"]);
        animation_object->setScale(json["scale"]["x"], json["scale"]["y"]);
        animation_object->setColor(sf::Color(json["color"]["r"], json["color"]["g"], json["color"]["b"], json["color"]["a"]));

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

    void SceneBuilder::moveObjectUp()
    {
        if(!m_SelectedLayer && !m_SelectedObject)
            return;

        auto childTable = m_SelectedLayer->getAllChild();

        for(auto it = childTable->begin(); it != childTable->end(); it++)
        {
            if((*it)->getId() == m_SelectedObject->getId() && it != (childTable->end()-1))
            {
                std::iter_swap(it, it+1);
                m_UpdateUndo();

                break;
            }
        }
    }

    void SceneBuilder::moveObjectDown()
    {
        if(!m_SelectedLayer && !m_SelectedObject)
            return;

        auto childTable = m_SelectedLayer->getAllChild();

        for(auto it = childTable->begin(); it != childTable->end(); it++)
        {
            if((*it)->getId() == m_SelectedObject->getId() && it != childTable->begin())
            {
                std::iter_swap(it, it-1);
                m_UpdateUndo();

                break;
            }
        }
    }

    void SceneBuilder::buildUI(UIObject::Ptr rootObject)
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
                        Object::Ptr sprite_object = (*it)->clone();

                        ButtonObject::Ptr button_object = ButtonObject::Ptr(new ButtonObject());
                        button_object->setName(sprite_object->getName());
                        button_object->setCategory(sprite_object->getCategory());
                        sprite_object->setName("no_name");
                        sprite_object->setCategory("no_category");

                        button_object->addChild(sprite_object);

                        layer_object->addChild(button_object);
                    }

                    rootObject->addChild(layer_object);

                }break;
            }
        }
    }

    void SceneBuilder::setTextContent(const sf::String& content)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setContent(content);
        }
    }

    void SceneBuilder::setTextFont(const sf::String& font)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            sf::Text& text = TextObject::Cast(m_SelectedObject)->getText();
            text.setFont(m_ResourceManager->font.getFont(font));
            TextObject::Cast(m_SelectedObject)->setFont(font);
        }
    }

    void SceneBuilder::setTextFontSize(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setFontSize(value);
        }
    }

    void SceneBuilder::setTextLetterSpacing(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setLetterSpacing(value);
        }
    }

    void SceneBuilder::setTextLineSpacing(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setLineSpacing(value);
        }
    }

    void SceneBuilder::setTextOutlineThickness(float value)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setOutlineThickness(value);
        }
    }

    void SceneBuilder::setTextStyle(bool bold, bool italic, bool underLined, bool strikeThrough)
    {
        if(m_SelectedObject && m_SelectedObject->getFirstType() == Object::Text_Object)
        {
            TextObject::Cast(m_SelectedObject)->setStyle(bold, italic, underLined, strikeThrough);
        }
    }

    const sf::Color& SceneBuilder::getCanvasColor() const
    {
        return m_CanvasColor;
    }

    void SceneBuilder::setCanvasColor(const sf::Color& color)
    {
        m_CanvasColor = color;
    }
}
