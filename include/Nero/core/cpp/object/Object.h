////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef OBJECT_H
#define OBJECT_H
///////////////////////////HEADERS//////////////////////////
//SFML
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
////////////////////////////////////////////////////////////
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
//STD
#include <vector>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class Object : public sf::Transformable, public sf::Drawable
    {
        public:
            typedef std::shared_ptr<Object>     Ptr;
            static  Ptr                         Cast(Object::Ptr object);

        public:
			enum    Type {Sprite_Object, Mesh_Object, Physic_Object, Meshed_Object, Solid_Object, Layer_Object,
						  Animation_Object, Animation_Meshed_Object, Animation_Solid_Object, Text_Object,
						  UI_Object, Button_Object, Light_Object,
						  Game_Level_Script_Object, None};

        public:
                                            Object();

            void                            setId(const int& id);
            void                            setName(const sf::String& name);
            void                            setSecondType(const Type& type);
            void                            setCategory(const sf::String& category);

            void                            setIsVisible(bool flag);
            void                            setIsUpdateable(bool flag);
            void                            setIsSelectable(bool flag);
            void                            setIsSelected(bool flag);


			const int&                      getId()             const;
			const int&                      getObjectId()       const;
			const sf::String&               getName()           const;
            const sf::String&               getCategory()       const;
            const Type&                     getFirstType()      const;
            const Type&                     getSecondType()     const;
			std::string						getTypeString(const Type& type) const;

            bool                            isVisible()         const;
            bool                            isUpdateable()      const;
            bool                            isSelectable()      const;
            bool                            isSelected()        const;


            virtual sf::Vector2f            getCenter()  const;
            sf::Vector2f			        getWorldPosition()  const;
            sf::Transform			        getWorldTransform() const;
            virtual sf::FloatRect           getGlobalBounds()   const;

            void					        addChild(Object::Ptr child);
            Object::Ptr						removeChild(const Object::Ptr& child);
            void					        removeAllChild();
            Object::Ptr                     removeChild(const int& index);
            Object::Ptr                     removeFirstChild();
            const std::vector<Ptr>*		    getAllChild()                           const;
            std::vector<Ptr>*		        getAllChild();
            Object::Ptr		                getChild(const int& index)              const;
            Object::Ptr                     getFirstChild()                         const;
            const int                       getChildCount()                         const;

            virtual void                    setColor(const sf::Color& color);
            virtual const sf::Color&        getColor()                              const;

            virtual Object::Ptr             clone(sf::Vector2f& position)           const;
            virtual Object::Ptr             clone()                                 const;

            template <typename T>
            void                            clone(T& object)                        const;

            virtual nlohmann::json          toJson()                                const;
            virtual sf::String              toString()                              const;

            void					        update(sf::Time time_step);

        protected:
            void                            setFirstType(const Type& type);

        private:
            virtual void			        updateObject(sf::Time time_step);
            void					        updateAllChild(sf::Time time_step);

            virtual void			        draw(sf::RenderTarget& target, sf::RenderStates states)         const;
            virtual void			        drawObject(sf::RenderTarget& target, sf::RenderStates states)  const;
            void					        drawAllChild(sf::RenderTarget& target, sf::RenderStates states) const;

        protected:
            std::vector<Object::Ptr>		m_ChildTab;
            Object*                         m_Parent;

            int                             m_Id;
            sf::String                      m_Name;
            sf::String                      m_Category;
            Type                            m_FirstType;
            Type                            m_SecondType;

            bool                            m_IsVisible;
            bool                            m_IsUpdateable;
            bool                            m_IsSelectable;
            bool                            m_IsSelected;
            bool                            m_IsDisabled;
    };

    template <typename T>
    void Object::clone(T& object) const
    {
        object->setId(getId());
        object->setName(getName());
        object->setCategory(getCategory());
        object->setSecondType(getSecondType());
        object->setIsVisible(isVisible());
        object->setIsUpdateable(isUpdateable());
        object->setIsSelectable(isSelectable());
        object->setIsSelected(isSelected());
    }
}
#endif // OBJECT_H
