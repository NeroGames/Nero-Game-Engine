////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/ui/EditorProxy.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorProxy::EditorProxy()
    {

    }

    void EditorProxy::openProject(const std::string&  projectDirectory) const
    {
        m_OpenProjectCallback(projectDirectory);
    }

    void EditorProxy::createProject(const Parameter&  projectParameter,
                                    const unsigned int& projectCount) const
    {
        m_CreateProjectCallback(projectParameter, projectCount);
    }
}
