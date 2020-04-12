////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef LAYEROBJECT_H
#define LAYEROBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/Object.h>
//JSON
#include <json/json.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class LayerObject : public Object
    {
        public:
            typedef std::shared_ptr<LayerObject>    Ptr;
            typedef std::vector<LayerObject::Ptr>   Tab;
            static Ptr                              Cast(Object::Ptr object);

        public:
                                            LayerObject();
            virtual Object::Ptr             clone()         const;
            virtual nlohmann::json          toJson()        const;
			bool&							getVisibility();


            virtual void                    setColor(const sf::Color& color);
            virtual const sf::Color&        getColor()      const;

            bool                            isEmpty()       const;

            void                            setOrder(const int& order);
            int                             getOrder()      const;

        private:
            int                             m_Order;
            sf::Color                       m_Color;
    };
}
#endif // LAYEROBJECT_H
