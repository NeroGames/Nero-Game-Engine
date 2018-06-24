#include <NERO/scene/ObjectManager.h>
#include <fstream>
#include <string>

#include <easyloggingpp/easylogging++.h>

namespace nero
{
    ObjectManager::ObjectManager(sfg::Canvas::Ptr& renderCanvas, TextureHolder& textureHolder):
        //Main
         m_RenderCanvas(renderCanvas)
        ,m_TextureHolder(textureHolder)
        ,m_World(nullptr)
        ,m_MeshEditor(renderCanvas)
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
    {
        m_SelectionRect.setFillColor(sf::Color::Transparent);
        m_SelectionRect.setOutlineColor(sf::Color::Green);
        m_SelectionRect.setOutlineThickness(-3.f);

        m_UpdateEngine  = [](){};
        m_EngineUndo    = [](){};
    }

    void ObjectManager::handleEvent(const sf::Event& event)
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

    void ObjectManager::update(const sf::Time& elapsedTime)
    {
        if(m_SelectedObject)
        {
            m_SelectionRect.setPosition(m_SelectedObject->getGlobalBounds().left,  m_SelectedObject->getGlobalBounds().top);
            m_SelectionRect.setSize(sf::Vector2f(m_SelectedObject->getGlobalBounds().width,  m_SelectedObject->getGlobalBounds().height));
        }
        else
            m_SelectionRect.setSize(sf::Vector2f(0.f, 0.f));


        if(m_SelectedLayer)
            m_SelectedLayer->update(elapsedTime);
    }

    void ObjectManager::render()
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->isVisible())
            {
                m_RenderCanvas->Draw(*(*it));
                m_RenderCanvas->Draw(m_SelectionRect);
            }
        }
    }

    void ObjectManager::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
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
                m_EngineUndo();

            if(key == sf::Keyboard::Numpad2 && CTRL())
                m_EngineUndo();

            if(key == sf::Keyboard::Numpad4 && CTRL())
                m_EngineUndo();

            if(key == sf::Keyboard::Numpad6 && CTRL())
                m_EngineUndo();

            //Rotate
            if(key == sf::Keyboard::Numpad7 && CTRL())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad9 && CTRL())
                m_EngineUndo();

            else if(key == sf::Keyboard::Divide && CTRL())
                m_EngineUndo();

            else if(key == sf::Keyboard::Multiply && CTRL())
                m_EngineUndo();

            //Zoom
            if(key == sf::Keyboard::Add && CTRL())
                m_EngineUndo();

            else if(key == sf::Keyboard::Subtract && CTRL())
                m_EngineUndo();

            //Flip
            if(key == sf::Keyboard::Numpad1 && CTRL())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad3 && CTRL())
                m_EngineUndo();

            //copy sprite
            if(key == sf::Keyboard::Numpad8 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad2 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad4 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad6 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad7 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad9 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad1 && ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad3 && ALT())
                m_EngineUndo();

            else  if(key == sf::Keyboard::Numpad8 && CTRL_ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad2 && CTRL_ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad4 && CTRL_ALT())
                m_EngineUndo();

            else if(key == sf::Keyboard::Numpad6 && CTRL_ALT())
                m_EngineUndo();
        }
    }

    void ObjectManager::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
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
                m_UpdateEngine();
        }

        if (mouse.button == sf::Mouse::Left && !isPressed && m_SelectedLayer && m_SelectedObject)
        {
            m_SelectedObject = nullptr;
            m_EngineUndo();
        }
    }

    void  ObjectManager::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
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

    Object::Ptr ObjectManager::findObject(Object::Ptr object, sf::Vector2f pos)
    {
        Object::Ptr result = nullptr;
        findObject(object, pos, result);

        return result;
    }


    void ObjectManager::findObject(Object::Ptr object, sf::Vector2f pos, Object::Ptr& result)
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

    Object::Ptr ObjectManager::findObject(Object::Ptr object, sf::String name)
    {
        Object::Ptr result = nullptr;
        findObject(object, name, result);

        return result;
    }

    void ObjectManager::findObject(Object::Ptr object, sf::String name, Object::Ptr& result)
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

    Object::Ptr ObjectManager::findObject(Object::Ptr object, int id)
    {
        Object::Ptr result = nullptr;
        findObject(object, id, result);

        return result;
    }

    void ObjectManager::findObject(Object::Ptr object, int id, Object::Ptr& result)
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

    const LayerObject::Ptr ObjectManager::addLayer()
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

        m_LayerTab.push_back(Layer_object);

        m_SelectedLayer = Layer_object;
        m_SelectedObject = nullptr;

        m_EngineUndo();

        return m_LayerTab.back();
    }

    void ObjectManager::deleteLayer(int id)
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->getId() == id)
            {
                if(it != m_LayerTab.begin())
                    selectLayer((*(it-1))->getId());
                else if(it == m_LayerTab.begin() && m_LayerTab.size() != 1)
                    selectLayer((*(it+1))->getId());

                m_LayerTab.erase(it);

                m_EngineUndo();

                break;
            }
        }
    }

    void ObjectManager::moveLayerDown(int id)
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->getId() == id && it != m_LayerTab.begin())
            {
                std::iter_swap(it, it-1);
                selectLayer((*(it-1))->getId());

                m_EngineUndo();

                break;
            }
        }
    }

    void ObjectManager::moveLayerUp(int id)
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->getId() == id && it != (m_LayerTab.end()-1))
            {
                std::iter_swap(it, it+1);
                selectLayer((*(it+1))->getId());

                m_EngineUndo();

                break;
            }
        }
    }

    void ObjectManager::updateLayerVisibility(int id)
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->getId() == id)
            {
                (*it)->setIsVisible(!(*it)->isVisible());

                m_EngineUndo();

                break;
            }
        }
    }

    void ObjectManager::updateLayerName(int id, sf::String name)
    {
        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if((*it)->getId() == id)
            {
                (*it)->setName(name);

                m_EngineUndo();

                break;
            }
        }
    }


    void ObjectManager::selectLayer(int id)
    {
        m_SelectedLayer->setIsSelected(false);
        m_SelectedLayer = nullptr;

        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); ++it)
        {
            if((*it)->getId() == id)
            {
                m_SelectedLayer = *it;
                m_SelectedLayer->setIsSelected(true);
                m_SelectedObject = nullptr;

                m_EngineUndo();

                break;
            }
        }
    }


    void ObjectManager::copyObject(const Position& position)
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
            MeshObject::Ptr mesh_object = std::static_pointer_cast<MeshObject>(object);
            mesh_object->getMesh()->setId(mesh_object->getId());
            m_MeshEditor.addMesh(mesh_object->getMesh());
        }

        if(object->getSecondType() == Object::Meshed_Object)
        {
            sf::Vector2f posf = -pos;
            Object::Ptr child_object = m_SelectedObject->getFirstChild()->clone(posf);
            child_object->setId(getNewId());

            MeshObject::Ptr mesh_object = std::static_pointer_cast<MeshObject>(child_object);
            mesh_object->getMesh()->setId(mesh_object->getId());
            mesh_object->setParentLastPosition(object->getPosition()-pos);
            mesh_object->setParentLastRotation(object->getRotation());
            mesh_object->setParentLastScale(object->getScale());
            mesh_object->setIsSelectable(false);

            object->setIsUpdateable(true);
            object->addChild(child_object);

            m_MeshEditor.addMesh(mesh_object->getMesh());

        }

        m_SelectedObject = object;
        m_SelectedLayer->addChild(m_SelectedObject);
    }

    void ObjectManager::updateLayerColor(const sf::Color& color)
    {
        if(m_SelectedLayer->getSecondType() == Object::Mesh_Object)
            return;

        auto children = m_SelectedLayer->getAllChild();

        for(auto it = children->begin(); it != children->end(); it++)
        {
            (*it)->setColor(color);
        }
    }

    void ObjectManager::updateAllLayerAlpha(int alpha)
    {

        for(auto layer = m_LayerTab.begin(); layer != m_LayerTab.end(); layer++)
        {
            if((*layer)->getSecondType() == Object::Sprite_Object || (*layer)->getSecondType() == Object::Meshed_Object)
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

    void ObjectManager::updateSpriteColor(const sf::Color& color)
    {
        if(m_SelectedObject != nullptr && m_SelectedObject->getFirstType() == Object::Sprite_Object)
            m_SelectedObject->setColor(color);
    }

    const LayerObject::Tab& ObjectManager::getLayerTab() const
    {
       return m_LayerTab;
    }

    MeshEditor* ObjectManager::getMeshEditor()
    {
        return  &m_MeshEditor;
    }

    bool ObjectManager::addObject(Object::Type type, const sf::String& label, sf::Vector2f position)
    {
        Object::Ptr object = nullptr;

        switch(type)
        {
            case Object::Sprite_Object:
            {
                sf::Sprite sprite;
                sf::IntRect rect = m_TextureHolder.getSpriteRect(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_TextureHolder.getSpriteTexture(label));
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

                    m_MeshEditor.addMesh(mesh_object->getMesh());

                    object = mesh_object;
                }
                else if(m_SelectedObject && m_SelectedObject->getSecondType() == Object::Meshed_Object)
                {
                    m_MeshEditor.deleteMesh(m_SelectedObject->getFirstChild()->getId());

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

                    m_MeshEditor.addMesh(mesh_object->getMesh());

                    m_SelectedObject->removeFirstChild();
                    m_SelectedObject->addChild(mesh_object);

                    m_EngineUndo();

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
                sf::IntRect rect = m_TextureHolder.getSpriteRect(label);
                sprite.setTextureRect(rect);
                sprite.setTexture(m_TextureHolder.getSpriteTexture(label));
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

                m_MeshEditor.addMesh(mesh_object->getMesh());
                sprite_object->addChild(mesh_object);

                //update one time
                sprite_object->update(TIME_PER_FRAME);

                object = sprite_object;

            }break;
        }

        if(m_SelectedLayer->getSecondType() == type)
        {
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_EngineUndo();
            return false;
        }
        else if (m_SelectedLayer->getSecondType() == Object::None || m_SelectedLayer->isEmpty())
        {
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_EngineUndo();
            return true;
        }
        else if(m_SelectedLayer->getSecondType() != type && m_SelectedLayer->getSecondType() != Object::None)
        {
            addLayer();
            m_SelectedLayer->setSecondType(type);
            m_SelectedLayer->addChild(object);
            if(type == Object::Mesh_Object) object->update(TIME_PER_FRAME);
            m_EngineUndo();
            return true;
        }
    }

    void ObjectManager::deleteObject(Object::Ptr object)
    {
        if(object->getFirstType() == Object::Mesh_Object)
        {
            m_MeshEditor.deleteMesh(object->getId());
        }

        else if(object->getSecondType() == Object::Meshed_Object)
        {
            m_MeshEditor.deleteMesh(object->getFirstChild()->getId());
        }

        m_SelectedLayer->removeChild(object);
        m_SelectedObject = nullptr;

        m_EngineUndo();
    }

    void ObjectManager::setMeshType(const sf::String& label)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
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
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshType(type);

        m_EngineUndo();
    }

    void ObjectManager::setMeshFixedRotation(bool flag)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFixedRotation(flag);

        m_EngineUndo();

    }

    void ObjectManager::setMeshSensor(bool flag)
    {
         if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshSensor(flag);

        m_EngineUndo();

    }

    void ObjectManager::setMeshAllowSleep(bool flag)
    {
       if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshAllowSleep(flag);

        m_EngineUndo();
    }

    void ObjectManager::setMeshDensity(float density)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshDensity(density);

        m_EngineUndo();
    }

    void ObjectManager::setMeshFriction(float friction)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshFriction(friction);

        m_EngineUndo();
    }

    void ObjectManager::setMeshRestitution(float restitution)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshRestitution(restitution);

        m_EngineUndo();
    }

    void ObjectManager::setMeshGravityScale(float gravityScale)
    {
        if(!m_SelectedObject)
            return;

        if(m_SelectedObject->getSecondType() != Object::Mesh_Object && m_SelectedObject->getSecondType() != Object::Meshed_Object)
            return;

        MeshObject::Ptr mesh_object;

        if(m_SelectedObject->getSecondType() == Object::Mesh_Object)
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject);
        else
            mesh_object = std::static_pointer_cast<MeshObject>(m_SelectedObject->getFirstChild());

        mesh_object->setMeshGravityScale(gravityScale);

        m_EngineUndo();
    }

    void ObjectManager::buildScene(Object::Ptr rootObject)
    {
        for(auto layer = m_LayerTab.begin(); layer != m_LayerTab.end(); layer++)
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
            }
        }
    }

    void ObjectManager::destroyAllPhysicObject(Object::Ptr rootObject)
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
                    m_World->DestroyBody(physic_object->getBody());
                }
            }
        }
    }

    void ObjectManager::setUpdateEngineFunction(std::function<void()>  f)
    {
        m_UpdateEngine = f;
    }

    void ObjectManager::setEngineUndoFunction(std::function<void()>  f)
    {
        m_EngineUndo = f;
        m_MeshEditor.setEngineUndoFunction(f);
    }

    Object::Ptr ObjectManager::getSelectedObject()
    {
        return m_SelectedObject;
    }

    void ObjectManager::setObjectName(const sf::String& name)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setName(name);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setName(name);

        m_EngineUndo();
    }

    void ObjectManager::setObjectCategory(const sf::String& category)
    {
        if(!m_SelectedObject)
            return;

        m_SelectedObject->setCategory(category);

        if(m_SelectedObject->getSecondType() == Object::Meshed_Object)
            m_SelectedObject->getFirstChild()->setCategory(category);

        m_EngineUndo();
    }

    SpriteObject::Ptr ObjectManager::loadSprite(nlohmann::json& json)
    {
        sf::Sprite sprite;
        sf::IntRect rect = m_TextureHolder.getSpriteRect(json["sprite"].get<std::string>());
        sprite.setTextureRect(rect);
        sprite.setTexture(m_TextureHolder.getSpriteTexture(json["sprite"].get<std::string>()));
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

    MeshObject::Ptr ObjectManager::loadMesh(nlohmann::json& json)
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

        m_MeshEditor.addMesh(mesh_object->getMesh());

        return mesh_object;
    }

    void ObjectManager::updateLayerOrder()
    {
        int i = 0;
        for(auto layer : m_LayerTab)
            layer->setOrder(i++);
    }

    sf::Color ObjectManager::getLayerColor()
    {
        /*if(m_SelectedLayer && !m_SelectedLayer->isEmpty())
            return m_SelectedLayer->getChild(0)->getColor();
        else
            return sf::Color();*/
    }

    nlohmann::json ObjectManager::saveScene()
    {
        std::vector<nlohmann::json> layer_table;

        for(auto layer = m_LayerTab.begin(); layer != m_LayerTab.end(); layer++)
            layer_table.push_back((*layer)->toJson());

        nlohmann::json scene;

        scene["layer_table"]    = layer_table;
        scene["layer_count"]    = m_LayerCount;
        scene["object_count"]   = m_ObjectCount;

        return scene;
    }

    void ObjectManager::loadScene(nlohmann::json scene)
    {
        m_LayerTab.clear();
        m_MeshEditor.destroyAllMesh();
        m_SelectedObject = nullptr;

        m_LayerCount    = scene["layer_count"];
        m_ObjectCount   = scene["object_count"];

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

                    m_LayerTab.push_back(layer_object);

                } break;

                case Object::Mesh_Object:
                {
                    nlohmann::json mesh_table = layer["mesh_table"];

                    for (auto& mesh : mesh_table)
                        layer_object->addChild(loadMesh(mesh));

                    m_LayerTab.push_back(layer_object);

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

                    m_LayerTab.push_back(layer_object);

                } break;

                case Object::None:
                {
                    m_LayerTab.push_back(layer_object);

                } break;
            }
        }
    }

    void ObjectManager::setWorld(b2World* world)
    {
        m_World = world;
        m_PhysicObjectManager.setWorld(world);
    }

    int ObjectManager::getNewId()
    {
        return m_ObjectCount++;
    }


}
