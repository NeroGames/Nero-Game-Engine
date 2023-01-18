////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/scene/UndoManager.h>
// STD
#include <iostream>
#include <algorithm>
////////////////////////////////////////////////////////////
namespace nero
{
    UndoManager::UndoManager()
        : m_UndoState(-1)
        , m_UndoTab()
    {
        // ctor
    }

    void UndoManager::add(nlohmann::json undo)
    {
        if(m_UndoState == m_UndoTab.size() - 1) // Last undo
        {
            if(m_UndoTab.size() == 0)
            {
                m_UndoTab.push_back(undo);
                m_UndoState++;
            }
            else if(m_UndoTab.back() != undo)
            {
                m_UndoTab.push_back(undo);
                m_UndoState++;
            }
        }
        else
        {
            m_UndoTab.erase(m_UndoTab.begin() + m_UndoState + 1, m_UndoTab.end());
            if(m_UndoTab.back() != undo)
            {
                m_UndoTab.push_back(undo);
                m_UndoState++;
            }
        }
    }

    nlohmann::json UndoManager::undo()
    {
        if(m_UndoState == 0)
            return nlohmann::json();

        return m_UndoTab[--m_UndoState];
    }

    nlohmann::json UndoManager::redo()
    {
        if(m_UndoState == m_UndoTab.size() - 1)
            return nlohmann::json();

        return m_UndoTab[++m_UndoState];
    }
} // namespace nero
