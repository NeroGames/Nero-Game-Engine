////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
///////////////////////////HEADERS//////////////////////////
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class ScriptManager
    {
        public:
            typedef std::shared_ptr<ScriptManager>  Ptr;

        public:
            /** Default constructor */
            ScriptManager();
            /** Default destructor */
            virtual ~ScriptManager();

        protected:

        private:
    };
}
#endif // SCRIPTMANAGER_H
