////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCRIPTOBJECT_H
#define SCRIPTOBJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
/////////////////////////////////////////////////////////////

namespace nero
{
    class ScriptObject : public Object
    {
      public: // utility
        struct Context
        {
        };

      public:
        ScriptObject(Context context);

        void        setTargetObject(Object::Ptr target);
        Object::Ptr getTargetObject();

      private:
        Context     m_ScriptContext;
        Object::Ptr m_TargetObject;
    };

} // namespace nero

#endif // SCRIPTOBJECT_H
