////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GENERICOBJECT_H
#define GENERICOBJECT_H
///////////////////////////HEADERS///////////////////////////
// STD
#include <memory>
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GenericObject
    {
      public: // Types definitions
        typedef std::shared_ptr<GenericObject> Ptr;

      public: // Object Types
        enum ObjectType
        {
            Generic      = 0,
            GameObject   = 1 << 0,
            LayerObject  = 1 << 1,
            SpriteObject = 1 << 2,
        };

      public: // Contructor
        GenericObject();
        virtual ~GenericObject();

        virtual void destroyObject();

      protected:
        // Node Attributes
        std::vector<GenericObject::Ptr> m_ChildTable;
        GenericObject*                  m_Parent;
        // Identifiers Attribures
        int                             m_ObjectId;
        std::string                     m_ObjectName;
        ObjectType                      m_ObjectType;
        std::string                     m_ObjectCategory;
        std::vector<std::string>        m_TagTable;
    };

} // namespace nero

#endif // GENERICOBJECT_H
