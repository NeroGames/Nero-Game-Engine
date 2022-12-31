////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORPROXY_H
#define EDITORPROXY_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/engine/Parameter.h>
//Std
#include <functional>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorProxy
    {
        public:
            using Ptr = std::shared_ptr<EditorProxy>;

        public:
                                    EditorProxy();

            void                    openProject(const std::string&  projectDirectory) const;
            void                    createProject(const Parameter&  projectParameter,
                                                  const unsigned int& projectCount) const;

        private:
            friend class                                                    EditorUI;
            std::function<void(const std::string&)>                         m_OpenProjectCallback;
            std::function<void(const Parameter&, const unsigned int&)>      m_CreateProjectCallback;
	};
}

#endif // EDITORPROXY_H
